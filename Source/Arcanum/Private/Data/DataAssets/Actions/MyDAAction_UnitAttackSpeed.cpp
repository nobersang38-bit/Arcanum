// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/Actions/MyDAAction_UnitAttackSpeed.h"
#include "Character/BaseUnitCharacter.h"
#include "Component/UnitCombatComponent.h"

void UMyDAAction_UnitAttackSpeed::StartAction(const FNonRegenStat& Stat)
{
	if (!IsEnable(Stat)) return;

	if (ABaseUnitCharacter* Unit = Cast<ABaseUnitCharacter>(GetOuter()))
	{
		Unit->GetUnitCombatComponent()->SetAttackSpeed(Stat.GetTotalValue());
		UE_LOG(LogTemp, Error, TEXT("공격중!! %s, %f"), *GetOuter()->GetName(), Stat.GetTotalValue());
	}
}
