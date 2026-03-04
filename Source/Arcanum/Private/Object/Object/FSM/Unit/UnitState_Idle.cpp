// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Idle.h"
#include "Component/UnitCombatComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameFramework/Character.h"

void UUnitState_Idle::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;

	Internal_UnitCombatComponent = UnitCombatComponent;
	Internal_UnitCombatComponent->TargetAssigned(nullptr);
	Internal_UnitCombatComponent->MoveToTarget(nullptr);
	if (Internal_UnitCombatComponent->OwnerCharacter.IsValid())
	{
		Internal_UnitCombatComponent->OwnerCharacter->StopAnimMontage();
	}

	float ExitTime = FMath::FRandRange(ExitRange.X, ExitRange.Y);
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
	GetWorld()->GetTimerManager().SetTimer(IdleTimer, this, &UUnitState_Idle::IdleExit, ExitTime, false, ExitTime);
}

void UUnitState_Idle::OnTick(float DeltaTime)
{
	if (!Internal_UnitCombatComponent.IsValid()) return;
}

void UUnitState_Idle::OnExit()
{
	GetWorld()->GetTimerManager().ClearTimer(IdleTimer);
}

void UUnitState_Idle::IdleExit()
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	// 상태 변경
	Internal_UnitCombatComponent->StateChange(EUnitState::Move);
}
