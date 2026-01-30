// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/ArcanumGameMode.h"
#include "Controller/ArcanumPlayerController.h"
#include "Character/BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArcanumGameMode::AArcanumGameMode()
{
	PlayerControllerClass = AArcanumPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}