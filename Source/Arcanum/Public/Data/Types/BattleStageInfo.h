// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NativeGameplayTags.h"
#include "Data/DataAssets/EnemyWaveData.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "DataInfo/StageData/EnemyData/Data/FEnemyData.h"
#include "BattleStageInfo.generated.h"

USTRUCT(BlueprintType)
struct FBasementStat
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FNonRegenStat CommandCenterHP;
};

/**
 * 김도현
 */
USTRUCT(BlueprintType)
struct FBattleStageInfo
{
	GENERATED_BODY()
public:
	// 적 웨이브
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FEnemyWaveDataInfo EnemyWaveDataInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StageLimitTime = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D SpawnPossibleSize = FVector2D(1500.0f, 900.0f);
};
