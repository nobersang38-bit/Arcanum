// Fill out your copyright notice in the Description page of Project Settings.
#include "Object/Object/StatusAction/StatusAction_UnitHealth.h"
#include "Character/BaseUnitCharacter.h"
#include "Component/UnitCombatComponent.h"

UStatusAction_UnitHealth::UStatusAction_UnitHealth()
{
	EnableTagMergeType = EStatusActionTagType::OnlyStatusActionCompTags;
}

void UStatusAction_UnitHealth::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	if (ABaseUnitCharacter* Unit = Cast<ABaseUnitCharacter>(GetOuter()))
	{
		if (RevertHealth > RegenStat.Current)
		{
			Unit->OuntLineStart(OutLineCurve, OutLineTime, 0.005f);
			Unit->GetUnitCombatComponent()->LightHitReaction(static_cast<int32>(RegenStat.Current - RevertHealth));
		}
		RevertHealth = RegenStat.Current;
	}
}
