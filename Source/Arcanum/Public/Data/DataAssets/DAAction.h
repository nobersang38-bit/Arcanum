// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "DAAction.generated.h"

/**
 * 
 */
UCLASS()
class ARCANUM_API UDAAction : public UDataAsset
{
	GENERATED_BODY()
public:
	virtual void StartAction(const FRegenStat& Stat);
	virtual void StartAction(const FNonRegenStat& Stat);
};
