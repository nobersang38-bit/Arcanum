#include "UI/Gacha/ARGachaGameModeBase.h"
#include "UI/Gacha/ARGachaController.h"
#include "Object/GachaLevel/ShootingStarActor.h"
#include "Object/GachaLevel/ASplinePathActor.h"

#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"
#include "Object/GachaLevel/ResultStarActor.h"
#include "Object/GachaLevel/ResultStarChild.h"

#include "Camera/CameraComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// ========================================================
// 생성자
// ========================================================
AARGachaGameModeBase::AARGachaGameModeBase()
{
	DefaultPawnClass = nullptr;
}
// ========================================================
// BeginPlay
// ========================================================
void AARGachaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GachaItemResult = FPlayerAccountService::GetGachaItemResult(this);
	if (GachaItemResult.Num() == 0) {
		ReturnLobby();
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AASplinePathActor::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors) {
		if (AASplinePathActor* SplineActor = Cast<AASplinePathActor>(Actor)) {
			SplineMap.Add(SplineActor->PathType, SplineActor);
		}
	}

	if (SplineMap.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("Spline NOT FOUND"));
		ReturnLobby();
		return;
	}

	FPlayerAccountService::SetGachaItemEmpty(this);
	StartGachaSequence();
}
// ========================================================
// 시작
// ========================================================
void AARGachaGameModeBase::StartGachaSequence()
{
	EnterIntroState();
}
// ========================================================
// 상태 진입
// ========================================================
void AARGachaGameModeBase::EnterIntroState()
{
	CurrentState = EGachaSequenceState::Intro;
	ActiveShootingStar = SpawnShootingStar();

	if (ActiveShootingStar) {
		ActiveShootingStar->OnShootingStarShrink.RemoveDynamic(this, &AARGachaGameModeBase::OnShootingStarShrink);
		ActiveShootingStar->OnShootingStarShrink.AddDynamic(this, &AARGachaGameModeBase::OnShootingStarShrink);

		ActiveShootingStar->OnShootingStarFinished.RemoveDynamic(this, &ThisClass::OnShootingStarImpact);
		ActiveShootingStar->OnShootingStarFinished.AddDynamic(this, &ThisClass::OnShootingStarImpact);

		SetupGachaCamera(ActiveShootingStar);
		ActiveShootingStar->StartShootingStar();
	}
}
void AARGachaGameModeBase::EnterResultState()
{
	CurrentState = EGachaSequenceState::Result;
	ActiveResultStar = SpawnResultActor();
	if (ActiveResultStar) ActiveResultStar->StartResultSequence();
}
void AARGachaGameModeBase::EnterFinishedState()
{
	CurrentState = EGachaSequenceState::Finished;
}
// ========================================================
// Spawn
// ========================================================
AShootingStarActor* AARGachaGameModeBase::SpawnShootingStar()
{
	if (!ShootingStarClass) return nullptr;

	AShootingStarActor* Star = GetWorld()->SpawnActor<AShootingStarActor>(ShootingStarClass);

	if (Star) {
		AASplinePathActor* Spline = GetSpline(ESplinePathType::ShootingStar);
		if (Spline) Star->SetSplineActor(Spline);
	}

	return Star;
}
AResultStarActor* AARGachaGameModeBase::SpawnResultActor()
{
	if (!ResultStarClass) return nullptr;

	ActiveResultStar = GetWorld()->SpawnActor<AResultStarActor>(ResultStarClass);
	if (ActiveResultStar) {
		AASplinePathActor* Spline = GetSpline(ESplinePathType::ResultStar);
		if (Spline) {
			ActiveResultStar->SetSplineActor(Spline);
			ActiveResultStar->SetGachaResults(GachaItemResult);
			SetupStaticSplineCamera(Spline);
		}

		BindResultActorDelegates(ActiveResultStar);
	}
	return ActiveResultStar;
}
// ========================================================
// 이벤트
// ========================================================
void AARGachaGameModeBase::OnShootingStarShrink()
{
	if (ActiveGachaCamera) ActiveGachaCamera->StopFlyingShake();
}
void AARGachaGameModeBase::OnShootingStarImpact()
{
	EnterResultState();
}
void AARGachaGameModeBase::OnSingleResultFinished(AResultStarActor* FinishedActor)
{
	EnterFinishedState();
}
void AARGachaGameModeBase::OnAllResultFinished()
{
	AARGachaController* PC = Cast<AARGachaController>(GetWorld()->GetFirstPlayerController());
	if (PC) PC->ShowFinalResultUI(GachaItemResult);
}
void AARGachaGameModeBase::BindResultActorDelegates(AResultStarActor* ResultActor)
{
	if (!ResultActor) return;

	ResultActor->OnResultStarFinished.RemoveDynamic(this, &ThisClass::OnSingleResultFinished);
	ResultActor->OnResultStarFinished.AddDynamic(this, &ThisClass::OnSingleResultFinished);

	ResultActor->OnAllResultFinished.RemoveDynamic(this, &ThisClass::OnAllResultFinished);
	ResultActor->OnAllResultFinished.AddDynamic(this, &ThisClass::OnAllResultFinished);
}
// ========================================================
// Skip
// ========================================================
void AARGachaGameModeBase::SkipGacha()
{
	if (CurrentState == EGachaSequenceState::OpenAllActor) return;
	if (CurrentState == EGachaSequenceState::Finished) {
		CurrentState = EGachaSequenceState::OpenAllActor;
		OpenChildActorAll();
		return;
	}

	SkipToFinal();
}
void AARGachaGameModeBase::SkipToFinal()
{
	CleanupAllActors();
	if (ActiveGachaCamera) ActiveGachaCamera->StopFlyingShake();

	EnterFinishedState();
	SpawnFinalResultInstant();
}
void AARGachaGameModeBase::OpenChildActorAll()
{
	if (!ActiveResultStar) return;

	for (AResultStarChild* Star : ActiveResultStar->GetStarActors()) {
		if (Star) {
			if (CurrentState == EGachaSequenceState::Finished) Star->OpenStar(true);
			else Star->OpenStar();
		}
	}
}
// ========================================================
// 즉시 결과 생성 (핵심)
// ========================================================
void AARGachaGameModeBase::SpawnFinalResultInstant()
{
	ActiveResultStar = SpawnResultActor();
	if (ActiveResultStar) {
		AASplinePathActor* SplineActor = GetSpline(ESplinePathType::ResultStar);
		if (SplineActor) {
			float MaxDistance = SplineActor->GetSplineLength();
			FVector FinalLoc = SplineActor->GetLocationAtDistance(MaxDistance);
			FRotator FinalRot{ 0,0,0 };
			ActiveResultStar->SetActorLocationAndRotation(FinalLoc, FinalRot);
		}
	}
}
// ========================================================
// 정리
// ========================================================
void AARGachaGameModeBase::CleanupAllActors()
{
	if (ActiveShootingStar) {
		ActiveShootingStar->ForceFinish();
		ActiveShootingStar->Destroy();
		ActiveShootingStar = nullptr;
	}

	if (ActiveResultStar) {
		ActiveResultStar->Destroy();
		ActiveResultStar = nullptr;
	}
}
// ========================================================
// 카메라
// ========================================================
void AARGachaGameModeBase::SetupGachaCamera(AActor* Target, float DistanceScale)
{
	if (!Target) return;

	if (!ActiveGachaCamera) {
		UClass* LoadedClass = GachaCameraClass.LoadSynchronous();
		if (LoadedClass) ActiveGachaCamera = GetWorld()->SpawnActor<AGachaCamera>(LoadedClass);
	}

	if (!ActiveGachaCamera) return;

	ActiveGachaCamera->SetTargetActor(Target);
	FVector StarLoc = Target->GetActorLocation();
	FVector Forward = Target->GetActorForwardVector();

	FVector CamLoc = StarLoc + (Forward * ActiveGachaCamera->RelativeOffset.X * DistanceScale) + (FVector::UpVector * ActiveGachaCamera->RelativeOffset.Z * DistanceScale);

	ActiveGachaCamera->SetActorLocation(CamLoc);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(CamLoc, StarLoc);
	ActiveGachaCamera->SetActorRotation(Rot);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) PC->SetViewTargetWithBlend(ActiveGachaCamera, 0.0f);

	ActiveGachaCamera->PlayFlyingShake();
}
void AARGachaGameModeBase::SetupStaticSplineCamera(AASplinePathActor* SplineActor)
{
	if (!ActiveGachaCamera) return;

	FVector FixedLoc = FVector(1442.f, -290.f, 550.f);
	FRotator FixedRot = FRotator(-29.f, 180.f, 0.f);

	ActiveGachaCamera->SetActorLocation(FixedLoc);
	ActiveGachaCamera->SetActorRotation(FixedRot);

	UCameraComponent* CamComp = ActiveGachaCamera->FindComponentByClass<UCameraComponent>();
	if (CamComp) CamComp->FieldOfView = 45.f;

	ActiveGachaCamera->SetTargetActor(nullptr);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) PC->SetViewTargetWithBlend(ActiveGachaCamera, 0.0f);
}
// ========================================================
// 스플라인
// ========================================================
AASplinePathActor* AARGachaGameModeBase::GetSpline(ESplinePathType Type)
{
	if (AASplinePathActor** Found = SplineMap.Find(Type)) return *Found;
	else return nullptr;
}
// ========================================================
// 로비
// ========================================================
void AARGachaGameModeBase::ReturnLobby()
{
	if (!LobbyMap.IsNull()) UGameplayStatics::OpenLevelBySoftObjectPtr(this, LobbyMap);
}