// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Attack.h"
#include "Component/UnitCombatComponent.h"
#include "GameFramework/Character.h"

void UUnitState_Attack::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;
	Internal_UnitCombatComponent = UnitCombatComponent;
	UE_LOG(LogTemp, Warning, TEXT("UUnitState_Attack::OnEnter"));
}

void UUnitState_Attack::OnTick(float DeltaTime)
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	FTimerDelegate AttackDelegate;

	AttackDelegate.BindWeakLambda(this, [this]()
		{
			if (!Internal_UnitCombatComponent.IsValid()) return;

			if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->OwnerCharacter.IsValid() && Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks.Num() > 0)
			{
				int32 IDX = FMath::RandRange(0, (Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks.Num() - 1));
				UAnimMontage* AttackMontage = Internal_UnitCombatComponent->UnitData.Info.AnimSetting.Attacks[IDX];
				Internal_UnitCombatComponent->OwnerCharacter->PlayAnimMontage(AttackMontage);

				Internal_UnitCombatComponent->RotateDelegate.Unbind();
				Internal_UnitCombatComponent->RotateDelegate.BindWeakLambda(this, [this]()
					{
						if (!Internal_UnitCombatComponent.IsValid()) return;

						if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->OwnerCharacter.IsValid())
						{
							FVector Direction = Internal_UnitCombatComponent->TargetActor->GetActorLocation() - Internal_UnitCombatComponent->GetOwner()->GetActorLocation();
							Direction = FVector(Direction.X, Direction.Y, 0);
							Internal_UnitCombatComponent->GetOwner()->SetActorRotation(FMath::RInterpConstantTo(Internal_UnitCombatComponent->GetOwner()->GetActorRotation(), Direction.Rotation(), Internal_UnitCombatComponent->RotateInterval, Internal_UnitCombatComponent->RotateSpeed));
						}
						else
						{
							Internal_UnitCombatComponent->GetWorld()->GetTimerManager().ClearTimer(Internal_UnitCombatComponent->RotateTimerHandle);
						}
					});

				Internal_UnitCombatComponent->GetWorld()->GetTimerManager().ClearTimer(Internal_UnitCombatComponent->RotateTimerHandle);
				Internal_UnitCombatComponent->GetWorld()->GetTimerManager().SetTimer(Internal_UnitCombatComponent->RotateTimerHandle, Internal_UnitCombatComponent->RotateDelegate, Internal_UnitCombatComponent->RotateInterval, true, 0.0f);
			}
			else
			{
				Internal_UnitCombatComponent->GetWorld()->GetTimerManager().ClearTimer(Internal_UnitCombatComponent->AttackTimerHandle);
				Internal_UnitCombatComponent->StateChange(EUnitState::Idle);
			}
		});
	if (!Internal_UnitCombatComponent->AttackTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(Internal_UnitCombatComponent->AttackTimerHandle, AttackDelegate, Internal_UnitCombatComponent->UnitAISetting.AttackRate, true, 0.0f);
	}
}

void UUnitState_Attack::OnExit()
{
}
