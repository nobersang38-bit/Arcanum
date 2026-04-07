#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "DTItemCatalogRow.generated.h"

UENUM(BlueprintType)
enum class EItemStackRule : uint8
{
	NonStackable UMETA(DisplayName = "NonStackable"),
	Stackable    UMETA(DisplayName = "Stackable"),
};

/**
 * 추영호
 * - UI/상점/인벤/판매/드롭 등 공통 Row
 * - 장비/포션/재료 등의 전용 데이터는 DetailTableTag + DetailRowName으로 연결
 */
USTRUCT(BlueprintType)
struct FDTItemCatalogRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/* 아이템 고유 키(전역) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag;

	/* 아이템 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTypeTag;

	/* UI 표시명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	/* 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Desc;

	/* 아이콘 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	/* 구매 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 BuyPrice = 0;
	/* 판매 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 SellPrice = 0;

	/* 스택 규칙 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemStackRule StackRule = EItemStackRule::NonStackable;

	/* 스택형일 때 한 슬롯 최대(NonStackable이면 1로 취급) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStack = 1;

	/* 인벤 정렬용(작을수록 앞) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SortOrder = 0;

	/* 전용 데이터가 들어있는 마스터 테이블 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag DetailTableTag;

	/* 인벤 저장 정책 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag StorePolicyTag;

	/* 전용 데이터 RowName (DetailTableTag의 RowName) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DetailRowName = NAME_None;

public:
	/* 공통 유효성 */
	bool IsValidCatalogRow() const
	{
		if (!ItemTag.IsValid())	return false;

		if (!ItemTypeTag.IsValid()) return false;

		if (StackRule == EItemStackRule::Stackable)
		{
			return MaxStack > 0;
		}
		return true;
	}

	///* 전용 데이터 링크 유효성 */
	//bool HasDetailLink() const
	//{
	//	return DetailTableTag.IsValid() && !DetailRowName.IsNone();
	//}

	///* 스택 최대값 */
	//int32 GetMaxStack() const
	//{
	//	if (StackRule == EItemStackRule::Stackable)
	//	{
	//		return FMath::Max(1, MaxStack);
	//	}

	//	return 1;
	//}
};