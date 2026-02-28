// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Object/FSM/UnitStateBase.h"
#include "UnitState_Attack.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UUnitState_Attack : public UUnitStateBase
{
	GENERATED_BODY()
public:
	virtual void OnEnter(class UUnitCombatComponent* UnitCombatComponent) override;
	virtual void OnTick(float DeltaTime) override;
	virtual void OnExit() override;

protected:
	UFUNCTION()
	void Attack();

	// 적을 향해 몸 돌리기
	UFUNCTION()
	void FocusTargetActor();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	FTimerHandle AttackTimerHandle;
	FTimerHandle FocusTargetActorTimerHandle;
	float RotateInterval = 0.01f;
	float RotateSpeed = 300.0f;
};
