#include "Object/GachaLevel/ResultStarActor.h"
#include "Object/GachaLevel/ResultStarChild.h"
#include "Object/GachaLevel/ASplinePathActor.h"

#include "NiagaraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"

// ========================================================
// 생성자
// ========================================================
AResultStarActor::AResultStarActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetupAttachment(SceneRoot);

    bIsMoving = false;
    CurrentDistance = 0.f;
}

// ========================================================
// BeginPlay
// ========================================================
void AResultStarActor::BeginPlay()
{
    Super::BeginPlay();
}

// ========================================================
// Spline 설정
// ========================================================
void AResultStarActor::SetSplineActor(AASplinePathActor* InSpline)
{
    SplineActor = InSpline;
    Spline = InSpline ? InSpline->GetPathSpline() : nullptr;
}

// ========================================================
// 결과 세팅 (Child 생성)
// ========================================================
void AResultStarActor::SetGachaResults(const TArray<FGachaItemResult>& InResults)
{
    if (InResults.Num() == 0 || ChildStarClass.IsNull()) return;

    UClass* StarClass = ChildStarClass.LoadSynchronous();
    if (!StarClass) return;

    for (int32 i = 0; i < InResults.Num(); ++i)
    {
        const FGachaItemResult& Result = InResults[i];

        // Pivot 생성
        USceneComponent* Pivot = NewObject<USceneComponent>(this);
        Pivot->SetupAttachment(RootComponent);
        Pivot->RegisterComponent();

        float StartYaw = (360.0f / InResults.Num()) * i;
        Pivot->SetRelativeRotation(FRotator(0.f, StartYaw, 0.f));

        OrbitPivots.Add(Pivot);

        // Child Actor 생성
        AResultStarChild* StarActor = GetWorld()->SpawnActor<AResultStarChild>(
            StarClass,
            GetActorLocation(),
            FRotator::ZeroRotator
        );

        if (!StarActor) continue;

        StarActor->SetResultData(Result);

        // 머티리얼 적용
        for (auto& Pair : GradeMatMap) {
            if (Result.GradeTag.MatchesTag(Pair.Key)) {
                if (UMaterialInterface** Mat = Pair.Value.Materials.Find(Result.GradeTag)) {
                    StarActor->ApplyGradeMaterial(*Mat);
                    break;
                }
            }
        }

        StarActor->AttachToComponent(Pivot, FAttachmentTransformRules::KeepRelativeTransform);
        StarActor->SetActorRelativeLocation(FVector(OrbitRadius, 0.f, 0.f));

        StarActors.Add(StarActor);

        StarActor->OnStarClicked.RemoveDynamic(this, &AResultStarActor::OnChildStarClicked);
        StarActor->OnStarClicked.AddDynamic(this, &AResultStarActor::OnChildStarClicked);
    }
}
// ========================================================
// 연출 시작
// ========================================================
void AResultStarActor::StartResultSequence(float InDelay)
{
    if (InDelay > 0.f) {
        GetWorldTimerManager().SetTimer(DelayTimerHandle, [this]() { bIsMoving = true; SetActorTickEnabled(true); }, InDelay, false);
    }
    else {
        bIsMoving = true;
        SetActorTickEnabled(true);
    }
}
// ========================================================
// Tick (이동 + 회전)
// ========================================================
void AResultStarActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsMoving || !Spline) return;

    const float TotalLength = Spline->GetSplineLength();

    CurrentDistance += MoveSpeed * DeltaTime;

    if (CurrentDistance >= TotalLength) {
        CurrentDistance = TotalLength;
        bIsMoving = false;

        SetActorLocation(Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));
        SetActorTickEnabled(false);

        OnResultStarFinished.Broadcast(this);
        return;
    }

    FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
    SetActorLocation(NewLocation);

    float RotationAmount = RotationSpeed * DeltaTime;
    for (USceneComponent* Pivot : OrbitPivots) {
        if (Pivot) Pivot->AddRelativeRotation(FRotator(0.f, RotationAmount, 0.f));
    }
}
// ========================================================
// 클릭 처리
// ========================================================
void AResultStarActor::OnChildStarClicked(AResultStarChild* ClickedStar)
{
    if (!ClickedStar) return;

    ClickedStar->PlayOpenAnimation(ClickedStar->GetResultData());
}

// ========================================================
// Skip 강제 종료
// ========================================================
void AResultStarActor::ForceFinish()
{
    GetWorldTimerManager().ClearTimer(DelayTimerHandle);

    bIsMoving = false;
    SetActorTickEnabled(false);
    for (AResultStarChild* Star : StarActors) {
        if (Star) Star->SetActorHiddenInGame(true);
    }
}
// ========================================================
// Skip
// ========================================================
void AResultStarActor::SpawnAllAtEnd()
{
    if (!Spline) return;

    GetWorldTimerManager().ClearTimer(DelayTimerHandle);
    float EndDistance = Spline->GetSplineLength();
    FVector EndLocation = Spline->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::World);
    SetActorLocation(EndLocation);

    bIsMoving = false;
    SetActorTickEnabled(false);

    OnResultStarFinished.Broadcast(this);
}