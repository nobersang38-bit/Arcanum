// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/DataAssets/DAAction.h"
#include "DAAction_MoveSpeed.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UDAAction_MoveSpeed : public UDAAction
{
	GENERATED_BODY()
	virtual void StartAction(const FNonRegenStat& Value) override;
};
