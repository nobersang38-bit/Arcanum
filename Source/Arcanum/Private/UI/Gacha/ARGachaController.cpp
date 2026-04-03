#include "UI/Gacha/ARGachaController.h"
#include "UI/Gacha/GachaLevelHUD.h"
#include "UI/Gacha/ARGachaGameModeBase.h"
#include "UI/Gacha/GachaHighgradeActor.h"
#include "UI/Gacha/GachaHighgradeWidget.h"
#include "UI/Gacha/GachaFinalResultWidget.h"
#include "Core/ARGameInstance.h"
#include "Object/GachaLevel/ResultStarActor.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"

void AARGachaController::BeginPlay()
{
	Super::BeginPlay();
	CachedGameMode = Cast<AARGachaGameModeBase>(UGameplayStatics::GetGameMode(this));

	if (GachaHUDClass) {
		GachaHUD = CreateWidget<UGachaLevelHUD>(this, GachaHUDClass);
		if (GachaHUD) {
			GachaHUD->AddToViewport();

			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);

			SetInputMode(InputMode);
		}
	}
	if (!GachaCamera && GachaCameraClass) GachaCamera = GetWorld()->SpawnActor<ACameraActor>(GachaCameraClass);
	

#if PLATFORM_WINDOWS
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

#elif PLATFORM_ANDROID || PLATFORM_IOS
	bShowMouseCursor = false;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	SetVirtualJoystickVisibility(false);
#endif
}

void AARGachaController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AARGachaController::RequestSkipGacha()
{
	AARGachaGameModeBase* GM = Cast<AARGachaGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM) GM->SkipGacha();
}
void AARGachaController::HandleSilhouetteStart()
{
	StartCameraSequence();
}
void AARGachaController::HandleSkipRequested()
{
	if (GachaCamera) {
		IsClicked = !IsClicked;
		StopGachaTimeline();
	}
}
void AARGachaController::StopGachaTimeline_Implementation()
{
}
void AARGachaController::HandleGachaFinished()
{
	if (SpawnedGachaActor) {
		SpawnedGachaActor->Destroy();
		SpawnedGachaActor = nullptr;
	}

	if (CachedGameMode) {
		SetViewTarget(CachedGameMode->GetActiveGachaCamera());
		AResultStarActor* ResultStarActor = CachedGameMode->GetActiveResultStar();
		ResultStarActor->NotifyOneResultFinished();
	}
	

	if (GachaHUD) {
		GachaHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(GachaHUD->GetCachedWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		SetInputMode(InputMode);
	}

	IsClicked = false;
}
void AARGachaController::ShowGachaHighgradeUI(const FGachaItemResult& ItemData, const FText& Dialog)
{
	if (GachaHUD) GachaHUD->SetVisibility(ESlateVisibility::Collapsed);
	if (!HighgradeActorClass) return;
	if (SpawnedGachaActor) {
		SpawnedGachaActor->Destroy();
		SpawnedGachaActor = nullptr;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	FVector GachaLocation = FVector(-3000.f, -10000.f, 0.f);
	SpawnedGachaActor = GetWorld()->SpawnActor<AGachaHighgradeActor>(HighgradeActorClass, GachaLocation, FRotator::ZeroRotator, Params);

	if (GachaCamera && SpawnedGachaActor) {
		FVector CameraLocation = FVector(-2120.f, -9940.f, 10.f);
		FRotator LookAtRot = FRotator(0.f, -180.f, 0.f);
		GachaCamera->SetActorLocation(CameraLocation);
		GachaCamera->SetActorRotation(LookAtRot);
		SetViewTargetWithBlend(GachaCamera);
	}

	if (SpawnedGachaActor) {
		IsClicked = false;
		SpawnedGachaActor->OnSilhouetteStart.RemoveDynamic(this, &AARGachaController::HandleSilhouetteStart);
		SpawnedGachaActor->OnSilhouetteStart.AddDynamic(this, &AARGachaController::HandleSilhouetteStart);
		SpawnedGachaActor->InitializeWidget(ItemData, Dialog);

		if (UGachaHighgradeWidget* Widget = SpawnedGachaActor->GetWidget()) {
			Widget->OnSkipRequested.RemoveDynamic(this, &AARGachaController::HandleSkipRequested);
			Widget->OnSkipRequested.AddDynamic(this, &AARGachaController::HandleSkipRequested);

			Widget->OnGachaFinished.RemoveDynamic(this, &AARGachaController::HandleGachaFinished);
			Widget->OnGachaFinished.AddDynamic(this, &AARGachaController::HandleGachaFinished);
		}
	}
}
void AARGachaController::EndCameraSequence()
{
	if (SpawnedGachaActor) {
		if (UGachaHighgradeWidget* Widget = SpawnedGachaActor->GetWidget())
			Widget->OnSilhouetteCameraMovingFinished();
	}
}
void AARGachaController::ShowFinalResultUI(TArray<FGachaItemResult> ItemRes)
{
	if (GachaHUD) GachaHUD->SetVisibility(ESlateVisibility::Collapsed);

	if (!GachaFinalResultWidgetClass) return;

	GachaFinalResultWidget = CreateWidget<UGachaFinalResultWidget>(this, GachaFinalResultWidgetClass);

	if (!GachaFinalResultWidget) return;

	GachaFinalResultWidget->SetGachaResults(ItemRes);
	GachaFinalResultWidget->AddToViewport();
	GachaFinalResultWidget->OnReturnLobbyClicked.RemoveDynamic(this, &AARGachaController::OnClickReturnLobby);
	GachaFinalResultWidget->OnReturnLobbyClicked.AddDynamic(this, &AARGachaController::OnClickReturnLobby);
}
void AARGachaController::OnClickReturnLobby()
{
	if (CachedGameMode) CachedGameMode->ReturnLobby();
}