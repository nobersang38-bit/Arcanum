// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/Actions/MyDAAction_UnitAttackSpeed.h"
#include "Character/BaseUnitCharacter.h"
#include "Component/UnitCombatComponent.h"

void UMyDAAction_UnitAttackSpeed::StartAction(AActor* TargetActor, const FNonRegenStat& Stat)
{
	Super::StartAction(TargetActor, Stat);
	if (ABaseUnitCharacter* Unit = Cast<ABaseUnitCharacter>(TargetActor))
	{
		Unit->GetUnitCombatComponent()->SetAttackSpeed(Stat.GetTotalValue());
	}
}
