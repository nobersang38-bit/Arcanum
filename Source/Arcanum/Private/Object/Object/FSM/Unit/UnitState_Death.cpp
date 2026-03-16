// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Death.h"
#include "Component/UnitCombatComponent.h"

void UUnitState_Death::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;
	Internal_UnitCombatComponent = UnitCombatComponent;
}

void UUnitState_Death::OnTick(float DeltaTime)
{
}

void UUnitState_Death::OnExit()
{
}

void UUnitState_Death::OnAbort()
{

}
