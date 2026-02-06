// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/AllyUnitCombatStat.h"
#include "Data/Types/BaseCharacterData.h"
#include "AllyUnitData.generated.h"

// 김도현
// 아군 유닛 데이터
USTRUCT(BlueprintType)
struct FAllyUnitData
{
	GENERATED_BODY()
protected:
	// 기본정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBaseCharacterData Info;

	// 전투 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAllyUnitCombatStat Stat;
};
