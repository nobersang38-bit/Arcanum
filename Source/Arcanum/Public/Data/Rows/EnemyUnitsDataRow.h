// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/UnitData.h"
#include "GameplayTagContainer.h"
#include "EnemyUnitsDataRow.generated.h"

// 김도현
// 아군 유닛 만드는 테이블 
USTRUCT(BlueprintType)
struct FEnemyUnitsDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 해당 유닛의 태그(나중에 찾을때 사용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUnitData UnitData;
};
