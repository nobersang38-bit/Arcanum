#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/InventoryData/Data/FInventoryRuleItemData.h"
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

	/* 타입별 규칙 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryRuleItem> Rules;

	/* SlotTag별 정렬 규칙 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySlotOrderItem> SlotOrders;
};
