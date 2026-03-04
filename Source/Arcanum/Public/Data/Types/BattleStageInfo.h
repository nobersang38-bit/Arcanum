// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NativeGameplayTags.h"
#include "Data/DataAssets/EnemyWaveData.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "BattleStageInfo.generated.h"

USTRUCT(BlueprintType)
struct FBasementStat
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FNonRegenStat CommandCenterCurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FNonRegenStat CommandCenterMaxHP;
};

/**
 * 김도현
 */
USTRUCT(BlueprintType)
struct FBattleStageInfo
{
	GENERATED_BODY()
public:
	// 사용할 월드 태그
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag WorldTag;

	// 적 웨이브
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FEnemyWaveDataInfo EnemyWaveDataInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBasementStat PlayerBasement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBasementStat EnemyBasement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StageLimitTime = 100.0f;
};
