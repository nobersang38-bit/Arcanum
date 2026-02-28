// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UnitStateBase.generated.h"

/**
 * 김도현
 * 유닛 전용 상태머신
 */
class UUnitCombatComponent;
UCLASS(Abstract, BlueprintType)
class ARCANUM_API UUnitStateBase : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnEnter(class UUnitCombatComponent* UnitCombatComponent) {};
	virtual void OnTick(float DeltaTime) {};
	virtual void OnExit() {};

protected:
	UPROPERTY()
	TWeakObjectPtr<UUnitCombatComponent> Internal_UnitCombatComponent = nullptr;
};
