// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameFramework/Character.h"
#include "Object/Actor/BattlefieldManagerActor.h"
#include "GameplayTags/ArcanumTags.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"

void UBattlefieldManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	OnMatchEnded.AddUObject(this, &UBattlefieldManagerSubsystem::SetCurrentMatchData);

	UARGameInstance* GameInstance = nullptr;
	GameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		SetInBattleData(GameInstance->GetPlayerData(), InBattleData);
	}
}

AActor* UBattlefieldManagerSubsystem::GetBasement(FGameplayTag InTeamTag) const
{
	return Basements.FindRef(InTeamTag);
}

void UBattlefieldManagerSubsystem::AddBasement(AActor* InNexus, FGameplayTag InTeamTag)
{
	if (!InNexus || !InTeamTag.IsValid()) return;

	if (Basements.Contains(InTeamTag))
	{
		UE_LOG(LogTemp, Error, TEXT("같은 팀의 기지가 하나 더 있습니다"));
	}
	else
	{
		Basements.Add(InTeamTag, InNexus);
	}
}

void UBattlefieldManagerSubsystem::SetABattlefieldManagerActor(ABattlefieldManagerActor* InBattlefieldManagerActor)
{
	BattlefieldManagerActor = InBattlefieldManagerActor;
}

void UBattlefieldManagerSubsystem::SetCurrentMatchData(const FMatchData& InData)
{
	CurrentMatchData = InData;
}

void UBattlefieldManagerSubsystem::SetInBattleData(const FPlayerData& InPlayerData, FInBattleData& OutInBattleData)
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