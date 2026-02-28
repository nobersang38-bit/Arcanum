// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Attack.h"
#include "Component/UnitCombatComponent.h"
#include "GameFramework/Character.h"
#include "Interface/RuntimeUnitDataInterface.h"

void UUnitState_Attack::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;
	Internal_UnitCombatComponent = UnitCombatComponent;
	Internal_UnitCombatComponent->MoveToTarget(nullptr);

	if (AttackTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	}
	if (FocusTargetActorTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(FocusTargetActorTimerHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UUnitState_Attack::Attack, Internal_UnitCombatComponent->UnitData.Info.AISetting.AttackRate, true, 0.0f);

	UE_LOG(LogTemp, Warning, TEXT("UUnitState_Attack::OnEnter"));
}

void UUnitState_Attack::OnTick(float DeltaTime)
{
	if (!Internal_UnitCombatComponent.IsValid()) return;
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
}


void UUnitState_Attack::Attack()
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->OwnerCharacter.IsValid() && Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks.Num() > 0)
	{
		Internal_UnitCombatComponent->MoveToTarget(nullptr);

		int32 IDX = FMath::RandRange(0, (Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks.Num() - 1));
		UAnimMontage* AttackMontage = Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks[IDX];
		Internal_UnitCombatComponent->OwnerCharacter->PlayAnimMontage(AttackMontage);

		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &UUnitState_Attack::OnAttackMontageEnded);
		Internal_UnitCombatComponent->OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(EndDelegate, AttackMontage);

		if (!FocusTargetActorTimerHandle.IsValid())
		{
			Internal_UnitCombatComponent->GetWorld()->GetTimerManager().ClearTimer(FocusTargetActorTimerHandle);
			Internal_UnitCombatComponent->GetWorld()->GetTimerManager().SetTimer(FocusTargetActorTimerHandle, this, &UUnitState_Attack::FocusTargetActor, RotateInterval, true, 0.0f);
		}
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

	if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->OwnerCharacter.IsValid())
	{
		FVector Direction = Internal_UnitCombatComponent->TargetActor->GetActorLocation() - Internal_UnitCombatComponent->GetOwner()->GetActorLocation();
		Direction = FVector(Direction.X, Direction.Y, 0);
		Internal_UnitCombatComponent->GetOwner()->SetActorRotation(FMath::RInterpConstantTo(Internal_UnitCombatComponent->GetOwner()->GetActorRotation(), Direction.Rotation(), RotateInterval, RotateSpeed));
	}
	else
	{
		Internal_UnitCombatComponent->GetWorld()->GetTimerManager().ClearTimer(FocusTargetActorTimerHandle);
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
}
