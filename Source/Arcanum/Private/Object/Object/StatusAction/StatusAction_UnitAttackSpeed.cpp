// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/StatusAction/StatusAction_UnitAttackSpeed.h"
#include "Character/BaseUnitCharacter.h"
#include "Component/UnitCombatComponent.h"

UStatusAction_UnitAttackSpeed::UStatusAction_UnitAttackSpeed()
{
	EnableTagMergeType = EStatusActionTagType::OnlyStatusActionCompTags;
}

void UStatusAction_UnitAttackSpeed::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	if (ABaseUnitCharacter* Unit = Cast<ABaseUnitCharacter>(GetOuter()))
	{
		Unit->GetUnitCombatComponent()->SetAttackSpeed(NonRegenStat.GetTotalValue());
	}
}
