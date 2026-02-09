// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/UnitCombatStat.h"
#include "Data/Types/BaseUnitData.h"
#include "UnitData.generated.h"

// 김도현
// 아군 유닛 데이터
USTRUCT(BlueprintType)
struct FUnitData
{
	GENERATED_BODY()
public:
	// 기본정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBaseUnitData Info;

	// 전투 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUnitCombatStat Stat;
};
