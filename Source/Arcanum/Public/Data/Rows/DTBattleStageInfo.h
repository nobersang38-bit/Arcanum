// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NativeGameplayTags.h"
#include "Data/Types/BattleStageInfo.h"
#include "DTBattleStageInfo.generated.h"

// 김도현
// 스테이지 정보 데이터 테이블
USTRUCT(BlueprintType)
struct FDTBattleStageInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag StageTag;

	// 적 웨이브 정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBattleStageInfo EnemyWaveDataInfo;
};

