// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/Types/CombatStageData.h"
#include "GameplayTagContainer.h"
#include "CombatStageDataRow.generated.h"

USTRUCT()
struct FCombatStageDataRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 찾을 때 사용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Tag;

	// 정보
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FCombatStageData StageData;
};

