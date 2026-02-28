// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Idle.h"
#include "Component/UnitCombatComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"

void UUnitState_Idle::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;

	Internal_UnitCombatComponent = UnitCombatComponent;
	Internal_UnitCombatComponent->StateReset();

	UE_LOG(LogTemp, Warning, TEXT("UUnitState_Idle::OnEnter"));
}

void UUnitState_Idle::OnTick(float DeltaTime)
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	if (!Internal_UnitCombatComponent->TargetBasement.IsValid())
	{
		UBattlefieldManagerSubsystem* BattlefieldManagerSubsystem = Internal_UnitCombatComponent->GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		if (BattlefieldManagerSubsystem)
		{
			//BattlefieldManagerSubsystem->G
		}
	}

	Internal_UnitCombatComponent->TargetAssigned(Internal_UnitCombatComponent->TargetBasement.Get());

	// 상태 변경
	Internal_UnitCombatComponent->StateChange(EUnitState::Move);
}

void UUnitState_Idle::OnExit()
{
}
