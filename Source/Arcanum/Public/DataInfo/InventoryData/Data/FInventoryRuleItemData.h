#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "FInventoryRuleItemData.generated.h"

UENUM(BlueprintType)
enum class EInventoryStackRule : uint8
{
	NonStackable UMETA(DisplayName = "NonStackable"),
	Stackable    UMETA(DisplayName = "Stackable"),
};

USTRUCT(BlueprintType)
struct FInventoryRuleItem
{
	GENERATED_BODY()

public:
	/* 이 룰이 적용될 아이템 루트 태그 (MatchesTag로 판정) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag RuleTag;

	/* UI 카테고리/정렬용(선택) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag CategoryTag;

	/* 스택 규칙 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EInventoryStackRule StackRule = EInventoryStackRule::NonStackable;

	/* 스택형일 때 한 슬롯 최대 (Stackable일 때만 사용) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack = 1;

	/* 룰 충돌 시 우선순위(큰 값 우선) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority = 0;

	/* 같은 우선순위 내 정렬용(선택) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SortOrder = 0;
};

USTRUCT(BlueprintType)
struct FInventorySlotOrderItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SlotTag;

	/* 부위 정렬 순서(작을수록 앞) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Order = 0;
};