#include "UI/Lobby/ARLobbyGameModeBase.h"
#include "UI/Lobby/ARLobbyController.h"

AARLobbyGameModeBase::AARLobbyGameModeBase()
{
	// 로비에서 사용할 플레이어 컨트롤러 지정
	PlayerControllerClass = AARLobbyController::StaticClass();
}

void AARLobbyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
