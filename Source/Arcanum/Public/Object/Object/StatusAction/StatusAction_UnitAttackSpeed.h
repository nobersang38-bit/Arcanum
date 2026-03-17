// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Object/StatusAction/StatusAction.h"
#include "StatusAction_UnitAttackSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ARCANUM_API UStatusAction_UnitAttackSpeed : public UStatusAction
{
	GENERATED_BODY()
public:
	UStatusAction_UnitAttackSpeed();
	virtual void StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) override;
};
