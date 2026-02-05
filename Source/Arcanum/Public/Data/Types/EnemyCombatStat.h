// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/UnitCombatStat.h"
#include "EnemyCombatStat.generated.h"

// 김도현
// 적 유닛 전투 스탯
USTRUCT(BlueprintType)
struct FEnemyCombatStat
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FUnitCombatStat UnitStat;
};
