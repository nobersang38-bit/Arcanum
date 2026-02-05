// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/UnitCombatStat.h"
#include "AllyUnitCombatStat.generated.h"

// 김도현
// 아군 유닛 전투 스탯
USTRUCT(BlueprintType)
struct FAllyUnitCombatStat
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FUnitCombatStat UnitStat;
};
