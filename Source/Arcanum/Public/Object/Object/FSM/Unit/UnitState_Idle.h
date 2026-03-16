// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Object/FSM/UnitStateBase.h"
#include "UnitState_Idle.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UUnitState_Idle : public UUnitStateBase
{
	GENERATED_BODY()
public:
	virtual void OnEnter(class UUnitCombatComponent* UnitCombatComponent) override;
	virtual void OnTick(float DeltaTime) override;
	virtual void OnExit() override;
	virtual void OnAbort() override;

protected:
	UFUNCTION()
	void IdleExit();

	FTimerHandle IdleTimer;

	FVector2D ExitRange = FVector2D(0.1f, 0.3f);
};
