// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/UnitData.h"
#include "GameplayTagContainer.h"
#include "UnitsDataRow.generated.h"

// 김도현
// 유닛 만드는 테이블 
USTRUCT(BlueprintType)
struct FUnitsDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUnitInfoSetting UnitData;
	//FUnitData UnitData;
};
