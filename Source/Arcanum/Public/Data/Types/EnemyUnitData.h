// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/BaseCharacterData.h"
#include "Data/Types/EnemyCombatStat.h"
#include "EnemyUnitData.generated.h"

// 김도현
// 적군 유닛 데이터
USTRUCT(BlueprintType)
struct FEnemyUnitData
{
	GENERATED_BODY()
protected:
	// 기본정보
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBaseCharacterData Info;

	// 전투 스탯
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FEnemyCombatStat Stat;
};
