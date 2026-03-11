#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "DTStatDisplayRow.generated.h"

/**
 * 추영호
 * 스탯 태그별 UI 표시명/표현 정보를 관리
 */

USTRUCT(BlueprintType)
struct FDTStatDisplayRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/* 표시 대상 스탯 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag StatTag;

	/* UI 표시 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	/* 퍼센트 표기 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUsePercent = false;

	/* UI 정렬 순서 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SortOrder = 0;

	/* 툴팁/설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Desc;
};