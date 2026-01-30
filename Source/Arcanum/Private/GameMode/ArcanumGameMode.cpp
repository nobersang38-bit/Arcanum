// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/ArcanumGameMode.h"
#include "Controller/ArcanumPlayerController.h"
#include "Character/BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArcanumGameMode::AArcanumGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AArcanumPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}