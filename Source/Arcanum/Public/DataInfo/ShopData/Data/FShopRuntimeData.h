#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FShopRuntimeData.generated.h"

/*
 * 추영호
 * - 상점 카테고리 규칙 데이터
 */
USTRUCT(BlueprintType)
struct FShopCategoryRule
{
	GENERATED_BODY()

public:
	/* 상점 카테고리 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag CategoryTag;

	/* 카테고리 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotCount = 0;

	/* 타이머 만료 시 자동 갱신 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRefreshOnTimer = true;

	/* 이 카테고리에서 뽑을 저장 정책 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag StorePolicyTag;

	/* 이 카테고리에서 뽑을 아이템 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTypeTag;

	bool IsValidRule() const
	{
		if (!CategoryTag.IsValid()) return false;
		if (SlotCount <= 0) return false;
		if (!StorePolicyTag.IsValid()) return false;
		if (!ItemTypeTag.IsValid()) return false;

		return true;
	}
};


/*
 * 상점 상품 1칸 런타임 데이터
 */
USTRUCT(BlueprintType)
struct FShopProductEntry
{
	GENERATED_BODY()

public:
	/* 상품 아이템 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag;

	/* 품절 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSoldOut = false;
};

/*
 * 상점 UI 표시용
 */
USTRUCT(BlueprintType)
struct FShopViewSlot
{
	GENERATED_BODY()

public:
	/* 상품 아이템 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag;

	/* 품절 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSoldOut = false;

	/* 표시 아이콘 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	/* 표시 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	/* 표시 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Desc;

	/* 표시 가격 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 Price = 0;
};

USTRUCT(BlueprintType)
struct FShopProductList
{
	GENERATED_BODY()

public:
	/* 상점 카테고리 상품 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FShopProductEntry> ShopProducts;
};

USTRUCT(BlueprintType)
struct FShopViewSlotList
{
	GENERATED_BODY()

public:
	/* 상점 카테고리 표시 슬롯 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FShopViewSlot> ShopViewSlots;
};