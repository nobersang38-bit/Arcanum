// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataAssets/DAAction.h"
#include "MyDAAction_UnitAttackSpeed.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UMyDAAction_UnitAttackSpeed : public UDAAction
{
	GENERATED_BODY()
public:
	virtual void StartAction(const FNonRegenStat& Stat) override;
};
