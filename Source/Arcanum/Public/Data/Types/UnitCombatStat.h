// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/CommonCombatStat.h"
#include "UnitCombatStat.generated.h"

// 김도현
// 유닛 공용 스탯
USTRUCT(BlueprintType)
struct FUnitCombatStat
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCommonCombatStat CommonStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackPower = 10.0f;
};
