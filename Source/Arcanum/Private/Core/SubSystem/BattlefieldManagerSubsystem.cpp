// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameFramework/Character.h"

void UBattlefieldManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	OnMatchEnded.AddUObject(this, &UBattlefieldManagerSubsystem::SetCurrentMatchData);
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

void UBattlefieldManagerSubsystem::SetCurrentMatchData(const FMatchData& InData)
{
	CurrentMatchData = InData;
}
