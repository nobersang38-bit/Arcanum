#include "UI/Lobby/ARLobbyController.h"
#include "UI/Lobby/LobbyHUD.h"

AARLobbyController::AARLobbyController()
{

}

void AARLobbyController::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyHUDClass)
	{
		if ((LobbyHUD = CreateWidget<ULobbyHUD>(this, LobbyHUDClass)))
		{
			LobbyHUD->AddToViewport();

			FInputModeGameAndUI inputMode;
			inputMode.SetHideCursorDuringCapture(false);
			inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(inputMode);
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

void AARLobbyController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
