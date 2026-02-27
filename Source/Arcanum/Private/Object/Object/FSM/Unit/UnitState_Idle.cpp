// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Idle.h"
#include "Component/UnitCombatComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"

void UUnitState_Idle::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;

	UnitCombatComponent->StateReset();
	if (!UnitCombatComponent->TargetBasement.IsValid())
	{
		UBattlefieldManagerSubsystem* BattlefieldManagerSubsystem = UnitCombatComponent->GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		if (BattlefieldManagerSubsystem)
		{
			//BattlefieldManagerSubsystem->G
		}
	}
	UnitCombatComponent->TargetAssigned(UnitCombatComponent->TargetBasement.Get());
}

void UUnitState_Idle::OnTick(float DeltaTime)
{
}

void UUnitState_Idle::OnExit()
{
}
