// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Object/FSM/UnitStateBase.h"
#include "UnitState_Move.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UUnitState_Move : public UUnitStateBase
{
	GENERATED_BODY()
public:
	virtual void OnEnter(class UUnitCombatComponent* UnitCombatComponent) override;
	virtual void OnTick(float DeltaTime) override;
	virtual void OnExit() override;
	virtual void OnAbort() override;

};
