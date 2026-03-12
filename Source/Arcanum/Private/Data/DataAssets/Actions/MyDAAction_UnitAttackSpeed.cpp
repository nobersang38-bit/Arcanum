// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/Actions/MyDAAction_UnitAttackSpeed.h"
#include "Character/BaseUnitCharacter.h"
#include "Component/UnitCombatComponent.h"

void UMyDAAction_UnitAttackSpeed::StartAction(const FNonRegenStat& Stat)
{
	Super::StartAction(Stat);
	if (ABaseUnitCharacter* Unit = Cast<ABaseUnitCharacter>(GetOuter()))
	{
		Unit->GetUnitCombatComponent()->SetAttackSpeed(Stat.GetTotalValue());
	}
}
