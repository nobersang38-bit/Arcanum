// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameFramework/Character.h"
#include "Object/Actor/BattlefieldManagerActor.h"
#include "GameplayTags/ArcanumTags.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"

#include "Core/ARPlayerAccountService.h"

void UBattlefieldManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	OnMatchEnded.AddUObject(this, &UBattlefieldManagerSubsystem::SetCurrentMatchData);

	/// 02/26 수정 : 서비스레이어 거치도록
	SetPlayerData(FPlayerAccountService::GetPlayerDataCopy(this), InBattleData);

	//UARGameInstance* GameInstance = nullptr;
	//GameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GameInstance)
	//{
	//	SetPlayerData(GameInstance->GetPlayerData(), InBattleData);
	//}
}

ACharacter* UBattlefieldManagerSubsystem::GetAllyNexus() const
{
	return AllyNexus.Get();
}

ACharacter* UBattlefieldManagerSubsystem::GetEnemyNexus() const
{
	return EnemyNexus.Get();
}

void UBattlefieldManagerSubsystem::SetAllyNexus(ACharacter* InNexus)
{
	AllyNexus = InNexus;
}

void UBattlefieldManagerSubsystem::SetEnemyNexus(ACharacter* InNexus)
{
	EnemyNexus = InNexus;
}

void UBattlefieldManagerSubsystem::SetABattlefieldManagerActor(ABattlefieldManagerActor* InBattlefieldManagerActor)
{
	BattlefieldManagerActor = InBattlefieldManagerActor;
}

void UBattlefieldManagerSubsystem::SetCurrentMatchData(const FMatchData& InData)
{
	CurrentMatchData = InData;
}

void UBattlefieldManagerSubsystem::SetPlayerData(const FPlayerData& InPlayerData, FInBattleData& OutInBattleData)
{
	for (int i = 0; i < InPlayerData.OwnedCharacters.Num(); i++)
	{
		if (InPlayerData.OwnedCharacters[i].bSelection)
		{
			OutInBattleData.BattleCharacterData = InPlayerData.OwnedCharacters[i];
			break;
		}
	}

	OutInBattleData.PlayerBattleData = InPlayerData.PlayerBattleData;
}