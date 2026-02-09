// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/AllyUnitData.h"
#include "GameplayTagContainer.h"
#include "AllyUnitsDataRow.generated.h"

// 김도현
// 아군 유닛 만드는 테이블 
USTRUCT(BlueprintType)
struct FAllyUnitsDataRow : public FTableRowBase
{
	GENERATED_BODY()
protected:
	// 해당 유닛의 태그(나중에 찾을때 사용)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FAllyUnitData UnitData;
};
