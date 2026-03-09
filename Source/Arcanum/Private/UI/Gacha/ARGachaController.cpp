#include "UI/Gacha/ARGachaController.h"
#include "UI/Gacha/GachaLevelHUD.h"

void AARGachaController::BeginPlay()
{
	Super::BeginPlay();

	if (GachaHUDClass) {
		GachaHUD = CreateWidget<UGachaLevelHUD>(this, GachaHUDClass);

		if (GachaHUD) {
			GachaHUD->AddToViewport();

			FInputModeGameAndUI InputMode;
			InputMode.SetHideCursorDuringCapture(false);
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);
		}
	}

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