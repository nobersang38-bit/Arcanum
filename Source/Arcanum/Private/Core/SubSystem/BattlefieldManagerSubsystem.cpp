// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameFramework/Character.h"

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
