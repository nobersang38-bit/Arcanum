#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "OptionPoolRow.generated.h"

/**
 * 추영호
 * 옵션 풀 설계용 DataTable Row
 * 옵션 개수
 * 옵션 후보 + 수비 범위
 */
USTRUCT(BlueprintType)
struct FOptionPoolRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 스탯 (Arcanum.Stat.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FGameplayTag StatTag;

	/* 옵션 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option|Count")
	int32 OptionCount = 3;

	/* 옵션 수치 범위 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	float MinValue = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	float MaxValue = 0.0f;
};
