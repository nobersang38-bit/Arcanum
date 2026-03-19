// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Object/StatusAction/StatusAction.h"
#include "StatusAction_MoveSpeed.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UStatusAction_MoveSpeed : public UStatusAction
{
	GENERATED_BODY()
public:
	UStatusAction_MoveSpeed();
	virtual void StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) override;
};
