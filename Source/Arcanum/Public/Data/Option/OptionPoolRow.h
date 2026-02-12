#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "OptionPoolRow.generated.h"

/**
 * 추영호
 * 옵션 풀 설계용 DataTable Row
 * 옵션 후보 1개(스탯 1종)
 */
USTRUCT(BlueprintType)
struct FOptionPoolRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 스탯 (Arcanum.Stat.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FGameplayTag StatTag;

	/* 옵션 수치 범위 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option|Value")
	float MinValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option|Value")
	float MaxValue = 0.0f;

	/* 적용 가능한 장비 태그들(부위) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option|Filter")
	FGameplayTagContainer EquipmentFilterTags;
};
