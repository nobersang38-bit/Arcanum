#include "Object/GachaLevel/ResultStarActor.h"
#include "Object/GachaLevel/ResultStarChild.h"
#include "Object/GachaLevel/ASplinePathActor.h"

#include "NiagaraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"

AResultStarActor::AResultStarActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;
    bIsMoving = false;
    CurrentDistance = 0.f;
}
void AResultStarActor::BeginPlay()
{
    Super::BeginPlay();
}
void AResultStarActor::SetGachaResults(const TArray<FGachaItemResult>& InResults)
{
    if (InResults.Num() == 0 || ChildStarClass.IsNull()) return;

    UClass* StarClass = ChildStarClass.LoadSynchronous();
    if (!StarClass) return;

    for (int32 i = 0; i < InResults.Num(); ++i) {
        const FGachaItemResult& Result = InResults[i];

        FName PivotName = *FString::Printf(TEXT("OrbitPivot_%d"), i);
        USceneComponent* NewPivot = NewObject<USceneComponent>(this, PivotName);

        NewPivot->SetupAttachment(RootComponent);
        NewPivot->RegisterComponent();

        float StartYaw = (360.0f / InResults.Num()) * i;
        NewPivot->SetRelativeRotation(FRotator(0.f, StartYaw, 0.f));
        OrbitPivots.Add(NewPivot);

        AResultStarChild* StarActor = GetWorld()->SpawnActor<AResultStarChild>(
            StarClass,
            GetActorLocation(),
            FRotator::ZeroRotator
        );

        if (!StarActor) continue;

        StarActor->SetResultData(Result);
        for (auto& Pair : GradeMatMap) {
            if (Result.GradeTag.MatchesTag(Pair.Key)) {
                if (UMaterialInterface** Mat = Pair.Value.Materials.Find(Result.GradeTag)) {
                    StarActor->ApplyGradeMaterial(*Mat);
                    break;
                }
            }
        }

        StarActor->AttachToComponent(NewPivot, FAttachmentTransformRules::KeepRelativeTransform);
        StarActor->SetActorRelativeLocation(FVector(OrbitRadius, 0.f, 0.f));
        StarActors.Add(StarActor);

        StarActor->OnStarClicked.RemoveDynamic(this, &AResultStarActor::OnChildStarClicked);
        StarActor->OnStarClicked.AddDynamic(this, &AResultStarActor::OnChildStarClicked);
    }
}
void AResultStarActor::StartResultSequence(float InDelay)
{
    if (InDelay > 0.f) {
        FTimerHandle DelayTimer;
        GetWorldTimerManager().SetTimer(DelayTimer, [this](){
                bIsMoving = true; SetActorTickEnabled(true);}, 
                InDelay, false);
    }
    else {
        bIsMoving = true;
        SetActorTickEnabled(true);
    }
}

void AResultStarActor::OnChildStarClicked(AResultStarChild* ClickedStar)
{
    if (!ClickedStar) return;

    ClickedStar->PlayOpenAnimation(ClickedStar->GetResultData());
}

void AResultStarActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsMoving || !SplineActor) return;

    float TotalLength = SplineActor->GetSplineLength();
    CurrentDistance += MoveSpeed * DeltaTime;

    if (CurrentDistance >= TotalLength) {
        CurrentDistance = TotalLength;
        bIsMoving = false;

        OnResultStarFinished.Broadcast(this);
        SetActorTickEnabled(false);
    }

    FVector NewLocation = SplineActor->GetLocationAtDistance(CurrentDistance);
    SetActorLocation(NewLocation);

    float RotationAmount = RotationSpeed * DeltaTime;
    for (USceneComponent* Pivot : OrbitPivots) {
        if (Pivot) Pivot->AddRelativeRotation(FRotator(0.f, RotationAmount, 0.f));
    }
}