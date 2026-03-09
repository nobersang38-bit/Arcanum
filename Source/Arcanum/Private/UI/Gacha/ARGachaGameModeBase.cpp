#include "UI/Gacha/ARGachaGameModeBase.h"
#include "Object/GachaLevel/ShootingStarActor.h"
#include "Object/GachaLevel/ASplinePathActor.h"
#include "Object/GachaLevel/GachaCamera.h"

#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"
#include "Object/GachaLevel/ResultStarActor.h"

#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

AARGachaGameModeBase::AARGachaGameModeBase()
{
	DefaultPawnClass = nullptr;
}
// ========================================================
// 가챠 결과 데이터
// ========================================================
void AARGachaGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GachaItemResult = FPlayerAccountService::GetGachaItemResult(this);
	if (GachaItemResult.Num() == 0) {
		ReturnLobby();
		return;
	}

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AASplinePathActor::StaticClass(), OutActors);
	for (AActor* Actor : OutActors) {
		if (AASplinePathActor* SplineActor = Cast<AASplinePathActor>(Actor)) {
			SplineMap.Add(SplineActor->PathType, SplineActor);
		}
	}

	if (SplineMap.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("Gacha Spline NOT FOUND in Level!"));
		ReturnLobby();
		return;
	}

	FPlayerAccountService::SetGachaItemEmpty(this);
	StartGachaSequence();
}
FName AARGachaGameModeBase::GetSplinePathTypeName(ESplinePathType InType)
{
	FString EnumName = StaticEnum<ESplinePathType>()->GetValueAsString(InType);
	return FName(*EnumName);
}
// ========================================================
// 가챠 연출 시작
// ========================================================
void AARGachaGameModeBase::SetupGachaCamera(AActor* Target)
{
	if (!Target) return;

	if (!ActiveGachaCamera) {
		UClass* LoadedClass = GachaCameraClass.LoadSynchronous();
		if (LoadedClass) ActiveGachaCamera = GetWorld()->SpawnActor<AGachaCamera>(LoadedClass, FVector::ZeroVector, FRotator::ZeroRotator);
	}

	if (ActiveGachaCamera) {
		ActiveGachaCamera->SetTargetActor(Target);
		FVector StarLoc = Target->GetActorLocation();
		FVector StarForward = Target->GetActorForwardVector();

		FVector InitialCameraLoc = StarLoc + (StarForward * ActiveGachaCamera->RelativeOffset.X) + (FVector::UpVector * ActiveGachaCamera->RelativeOffset.Z);
		ActiveGachaCamera->SetActorLocation(InitialCameraLoc);

		FRotator InitialRot = UKismetMathLibrary::FindLookAtRotation(InitialCameraLoc, StarLoc);
		ActiveGachaCamera->SetActorRotation(InitialRot);

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC) PC->SetViewTargetWithBlend(ActiveGachaCamera, 0.0f);
		ActiveGachaCamera->PlayFlyingShake();
	}
}
void AARGachaGameModeBase::StartGachaSequence()
{
	TArray<AActor*> ExistingStars;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ShootingStarClass, ExistingStars);
	if (ExistingStars.Num() > 0) return;
	AShootingStarActor* Star = GetWorld()->SpawnActor<AShootingStarActor>(ShootingStarClass, FVector::ZeroVector, FRotator::ZeroRotator);
	if (Star) {
		if (AASplinePathActor** FoundSpline = SplineMap.Find(ESplinePathType::ShootingStar)) {
			Star->SetSplineActor(*FoundSpline);
			Star->OnShootingStarFinished.RemoveDynamic(this, &AARGachaGameModeBase::OnShootingStarImpact);
			Star->OnShootingStarFinished.AddDynamic(this, &AARGachaGameModeBase::OnShootingStarImpact);

			SetupGachaCamera(Star);
			Star->StartShootingStar();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ShootingStar Spline not found in SplineMap!"));
			ReturnLobby();
		}
	}
}
void AARGachaGameModeBase::OnShootingStarImpact()
{
	if (ActiveGachaCamera) ActiveGachaCamera->StopFlyingShake();

	AASplinePathActor** ResultSplinePtr = SplineMap.Find(ESplinePathType::ResultStar);
	if (!ResultSplinePtr || !*ResultSplinePtr) return;

	AResultStarActor* ResultManager = GetWorld()->SpawnActor<AResultStarActor>(ResultStarClass, FVector::ZeroVector, FRotator::ZeroRotator);

	if (ResultManager) {
		ResultManager->SetSplineActor(*ResultSplinePtr);
		ResultManager->SetGachaResults(GachaItemResult);

		ResultManager->OnResultStarFinished.RemoveDynamic(this, &AARGachaGameModeBase::OnSingleResultFinished);
		ResultManager->OnResultStarFinished.AddDynamic(this, &AARGachaGameModeBase::OnSingleResultFinished);

		ResultManager->StartResultSequence();
	}
}
void AARGachaGameModeBase::OnSingleResultFinished(AResultStarActor* FinishedActor)
{
	UE_LOG(LogTemp, Log, TEXT("Star Arrived! Remaining: %d"), ActiveResultStars);
}
// ========================================================
// 별 낙하 이후 결과 표시
// ========================================================
void AARGachaGameModeBase::ShowResult()
{
	UE_LOG(LogTemp, Log, TEXT("Show Gacha Result"));

	StartResultSequence();
}
// ========================================================
// 로비 복귀
// ========================================================
void AARGachaGameModeBase::ReturnLobby()
{
	if (!LobbyMap.IsNull()) UGameplayStatics::OpenLevelBySoftObjectPtr(this, LobbyMap);
	else UE_LOG(LogTemp, Error, TEXT("LobbyMap not set!"));
}