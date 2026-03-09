// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Attack.h"
#include "Component/UnitCombatComponent.h"
#include "GameFramework/Character.h"
#include "Interface/RuntimeUnitDataInterface.h"
#include "AIController.h"

void UUnitState_Attack::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;
	Internal_UnitCombatComponent = UnitCombatComponent;
	Internal_UnitCombatComponent->MoveToTarget(nullptr);
	TargetActorBackup = Internal_UnitCombatComponent->TargetActor;

	if (AttackTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
	if (FocusTargetActorTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FocusTargetActorTimerHandle);
	}
	Attack();
}

void UUnitState_Attack::OnTick(float DeltaTime)
{
	if (!Internal_UnitCombatComponent.IsValid()) return;
	FocusTargetActor();
}

void UUnitState_Attack::OnExit()
{
	if (AttackTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
	if (FocusTargetActorTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FocusTargetActorTimerHandle);
	}

	Internal_UnitCombatComponent->OwnerCharacter->StopAnimMontage();
	TargetActorBackup = nullptr;
}


void UUnitState_Attack::Attack()
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->OwnerCharacter.IsValid() && Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks.Num() > 0)
	{
		Internal_UnitCombatComponent->MoveToTarget(nullptr);

		int32 IDX = FMath::RandRange(0, (Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks.Num() - 1));
		UAnimMontage* AttackMontage = Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks[IDX].Montage;
		NextAttackCoolTime = Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks[IDX].AttackRate;
		Internal_UnitCombatComponent->OwnerCharacter->PlayAnimMontage(AttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UUnitState_Attack::OnAttackMontageEnded);
		Internal_UnitCombatComponent->OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);

		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UUnitState_Attack::Attack, NextAttackCoolTime, false, NextAttackCoolTime);
		/*if (!FocusTargetActorTimerHandle.IsValid())
		{
			Internal_UnitCombatComponent->GetWorld()->GetTimerManager().ClearTimer(FocusTargetActorTimerHandle);
			Internal_UnitCombatComponent->GetWorld()->GetTimerManager().SetTimer(FocusTargetActorTimerHandle, this, &UUnitState_Attack::FocusTargetActor, RotateInterval, true, 0.0f);
		}*/
	}
	else
	{
		Internal_UnitCombatComponent->GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

		// 상태 변경
		Internal_UnitCombatComponent->StateChange(EUnitState::Idle);
	}
}

void UUnitState_Attack::FocusTargetActor()
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	//if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->OwnerCharacter.IsValid())
	//{
	//	FVector Direction = Internal_UnitCombatComponent->TargetActor->GetActorLocation() - Internal_UnitCombatComponent->GetOwner()->GetActorLocation();
	//	Direction = FVector(Direction.X, Direction.Y, 0);
	//	Internal_UnitCombatComponent->GetOwner()->SetActorRotation(FMath::RInterpConstantTo(Internal_UnitCombatComponent->GetOwner()->GetActorRotation(), Direction.Rotation(), RotateInterval, RotateSpeed));
	//}
	//else
	//{
	//	Internal_UnitCombatComponent->GetWorld()->GetTimerManager().ClearTimer(FocusTargetActorTimerHandle);
	//}

	if (Internal_UnitCombatComponent->TargetActor.IsValid())
	{
		FVector Direction = Internal_UnitCombatComponent->TargetActor->GetActorLocation() - Internal_UnitCombatComponent->GetOwner()->GetActorLocation();
		float Yaw = Direction.Rotation().Yaw;
		FRotator ControllerRotator = FRotator(0, Yaw, 0);
		Internal_UnitCombatComponent->OwnerAIC->SetControlRotation(ControllerRotator);
	}
}

void UUnitState_Attack::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	// 타겟은 있지만 거리가 멀다면 Move로 변경
	if (!Internal_UnitCombatComponent->IsCanAttackRange() && Internal_UnitCombatComponent->TargetActor.IsValid())
	{
		// 상태 변경
		Internal_UnitCombatComponent->StateChange(EUnitState::Move);
	}

	if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->TargetActor->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
	{
		auto Interface = Cast<IRuntimeUnitDataInterface>(Internal_UnitCombatComponent->TargetActor);
		if (Interface && Interface->GetIsDead())
		{
			// 상태 변경
			Internal_UnitCombatComponent->StateChange(EUnitState::Move);
		}
	}

	Internal_UnitCombatComponent->EnemyFind();
	if (Internal_UnitCombatComponent->DetectedActors.Num() > 0)
	{
		Internal_UnitCombatComponent->SelectBestTarget(Internal_UnitCombatComponent->DetectedActors);
		if (Internal_UnitCombatComponent->TargetActor != TargetActorBackup)
		{
			Internal_UnitCombatComponent->StateChange(EUnitState::Move);
		}
	}
}
