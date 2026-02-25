#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARLobbyGameModeBase.generated.h"

UCLASS()
class ARCANUM_API AARLobbyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AARLobbyGameModeBase();

protected:
	virtual void BeginPlay() override;
};
