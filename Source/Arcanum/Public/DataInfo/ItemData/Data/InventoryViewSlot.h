#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InventoryViewSlot.generated.h"

class UTexture2D;

/**
 * 추영호
 * - UI 표시용 인벤 슬롯 데이터
 * - layerData.Inventory + PlayerData.PotionCounts
 *   슬롯 단위로 변환해서 인벤/툴팁/UI에서 공통으로 사용
 */

UENUM(BlueprintType)
enum class EInventoryViewSlotType : uint8
{
    Empty,
    Equipment,
    StackItem,
};

USTRUCT(BlueprintType)
struct FInventoryViewSlot
{
    GENERATED_BODY()

    /* 이 슬롯이 빈칸/장비/포션 중 무엇인지 구분하는 타입 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInventoryViewSlotType Type = EInventoryViewSlotType::Empty;

    /* 장비면 이 Guid로 PlayerData.Inventory에서 찾는 키 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid ItemGuid;

    /* 어떤 종류인지 구분하는 태그 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag ItemTag;

    /* 슬롯 스택 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StackCount = 0;

    /* 장비/스택아이템 공통 아이콘 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Icon;

    /* 장비 강화 수치(+n). 포션이면 0 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UpgradeLevel = 0;
};