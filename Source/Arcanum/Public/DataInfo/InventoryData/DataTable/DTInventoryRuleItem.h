#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DTInventoryRuleItem.generated.h"

/**
 * 추영호
 * 인벤토리 DT
 */

USTRUCT(BlueprintType)
struct FDTInventoryRuleItem : public FTableRowBase
{
	GENERATED_BODY()

public:
	/* 인벤 총 슬롯 수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Capacity = 100;
};
