#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "FInventoryRuleItemData.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlotOrderItem
{
	GENERATED_BODY()

	int32 Capacity = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SlotTag;

	/* 부위 정렬 순서(작을수록 앞) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Order = 0;
};