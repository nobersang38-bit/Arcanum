#include "UI/Login/ARLoginController.h"
#include "UI/Login/ARLoginHUD.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
AARLoginController::AARLoginController()
{
}
void AARLoginController::BeginPlay()
{
	Super::BeginPlay();

	if (LoginHUDClass) {
		LoginHUD = CreateWidget<UARLoginHUD>(this, LoginHUDClass);
		if (LoginHUD) {
			LoginHUD->AddToViewport();

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
	SetVirtualJoystickVisibility(false);
#endif
}
void AARLoginController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
