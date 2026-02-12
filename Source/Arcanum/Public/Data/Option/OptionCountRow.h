#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "OptionCountRow.generated.h"

/**
 * 추영호
 * 이 부위/장비는 옵션을 몇 줄 뽑는다는 설정
 */
USTRUCT(BlueprintType)
struct FOptionCountRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 적용 대상 장비 태그(부위/무기) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OptionCount")
	FGameplayTag EquipmentTag;

	/* 장비 옵션 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OptionCount")
	int32 OptionCount = 3;
};

