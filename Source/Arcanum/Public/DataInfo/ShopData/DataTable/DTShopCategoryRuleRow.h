#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/ShopData/Data/FShopRuntimeData.h"
#include "DTShopCategoryRuleRow.generated.h"

/**
 * 추영호
 * 상점 카테고리 테이블
 */

USTRUCT(BlueprintType)
struct FDTShopCategoryRuleRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FShopCategoryRule ShopRule;
};
