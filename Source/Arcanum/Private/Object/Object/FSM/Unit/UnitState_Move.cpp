// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Move.h"
#include "Component/UnitCombatComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "AIController.h"

void UUnitState_Move::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;
	Internal_UnitCombatComponent = UnitCombatComponent;
	UE_LOG(LogTemp, Warning, TEXT("UUnitState_Move::OnEnter"));
}

void UUnitState_Move::OnTick(float DeltaTime)
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	FVector Velocity = Internal_UnitCombatComponent->OwnerCharacter->GetVelocity();
	if (Velocity.SquaredLength() > 10.0f)
	{
		float Yaw = Velocity.Rotation().Yaw;
		FRotator ControllerRotator = FRotator(0, Yaw, 0);
		Internal_UnitCombatComponent->OwnerAIC->SetControlRotation(ControllerRotator);
	}

	Internal_UnitCombatComponent->EnemyFind();

	if (Internal_UnitCombatComponent->DetectedActors.Num() > 0)
	{
		Internal_UnitCombatComponent->SelectBestTarget(Internal_UnitCombatComponent->DetectedActors);
		Internal_UnitCombatComponent->MoveToTarget(Internal_UnitCombatComponent->TargetActor.Get());
	}
	else
	{
		Internal_UnitCombatComponent->TargetAssigned(Internal_UnitCombatComponent->TargetBasement.Get());
		Internal_UnitCombatComponent->MoveToTarget(Internal_UnitCombatComponent->TargetActor.Get());
	}

	if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->IsCanAttackRange())
	{
		Internal_UnitCombatComponent->StateChange(EUnitState::Attack);
	}
}

void UUnitState_Move::OnExit()
{
}
