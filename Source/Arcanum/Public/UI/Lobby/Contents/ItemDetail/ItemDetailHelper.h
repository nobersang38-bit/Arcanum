#pragma once

#include "CoreMinimal.h"
#include "DataInfo/BattleCharacter/Equipment/Data/FEquipmentData.h"
#include "ItemDetailHelper.generated.h"

class UGameDataSubsystem;
struct FDTStatDisplayRow;

/**
 * 추영호
 * 아이템 상세 정보 UI 표시 데이터 생성 헬퍼
 * 데이터 -> UI 표시용 텍스트 캐시
 */

/* 능력치 1줄 표시용 */
USTRUCT(BlueprintType)
struct FItemStatLineViewData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StatValueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVisible = false;
};

/* 개별 아이템 툴팁 전체 표시용 */
USTRUCT(BlueprintType)
struct FItemDisplayViewData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText UpgradeLevelText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemStatLineViewData> StatLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DescText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SellPriceText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowUpgradeLevel = false;
};

/* 강화 상세 패널 표시용 */
USTRUCT(BlueprintType)
struct FEnhancementNextStatViewData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemStatLineViewData> NextStatLines;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EnhanceChanceText;
};

/* 장착 전체 능력치 패널 표시용 */
USTRUCT(BlueprintType)
struct FEquippedTotalStatViewData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemStatLineViewData> StatLines;
};

class ARCANUM_API FItemDetailHelper
{
#pragma region 공용 아이템 표시 데이터
public:
	/* 장비 Guid 툴팁 데이터 생성 */
	static bool BuildEquipmentDisplayViewData(
		const UObject* WorldContextObject,
		const FGuid& InItemGuid,
		FItemDisplayViewData& OutViewData
	);

	/* 스택 ItemTag 기준 툴팁 데이터 생성 */
	static bool BuildStackItemDisplayViewData(
		const UObject* WorldContextObject,
		const FGameplayTag& InItemTag,
		FItemDisplayViewData& OutViewData
	);

	/* 툴팁 데이터 초기화 */
	static void ClearDisplayViewData(FItemDisplayViewData& OutViewData);
#pragma endregion

#pragma region 강화 후 패널
public:
	/* 강화 후 패널 데이터 생성 */
	static bool BuildEnhancementNextStatViewData(
		const UObject* WorldContextObject,
		const FGuid& InItemGuid,
		FEnhancementNextStatViewData& OutViewData
	);

	/* 강화 후 패널 데이터 초기화 */
	static void ClearEnhancementNextStatViewData(FEnhancementNextStatViewData& OutViewData);

private:
	/* 강화 후 수치 텍스트 생성 */
	static FText BuildNextStatValueText(const FStatRangeDefinition& InRange, bool bInUsePercent);

	/* 성공 확률 텍스트 생성 */
	static FText BuildEnhanceChanceText(int32 InSuccessRate);
#pragma endregion

#pragma region 장착 전체 능력치 패널
public:
	/* 선택 캐릭터 장착 전체 능력치 패널 데이터 생성 */
	static bool BuildEquippedTotalStatsViewData(
		const UObject* WorldContextObject,
		const FName& InCharacterName,
		FEquippedTotalStatViewData& OutViewData
	);

	/* 장착 전체 능력치 패널 데이터 초기화 */
	static void ClearEquippedTotalStatViewData(FEquippedTotalStatViewData& OutViewData);
#pragma endregion

#pragma region 찾기 헬퍼
private:
	/* Guid로 인벤토리 장비 찾기 */
	static bool FindEquipmentByGuid(const UObject* WorldContextObject, const FGuid& InItemGuid,	FEquipmentInfo& OutEquipmentInfo);

	/* 스탯 표시 Row 찾기 */
	static const FDTStatDisplayRow* FindStatDisplayRowByTag(UGameDataSubsystem* InDataSubsystem, const FGameplayTag& InStatTag);
#pragma endregion

#pragma region 텍스트
private:
	/* 스탯 이름 텍스트 생성 */
	static FText BuildStatNameText(
		UGameDataSubsystem* InDataSubsystem,
		const FGameplayTag& InStatTag
	);

	/* 현재 스탯 값 텍스트 생성 */
	static FText BuildCurrentStatValueText(const FDerivedStatModifier& InStat, bool bInUsePercent);

	/* 판매 가격 텍스트 생성 */
	static FText BuildSellPriceText(int64 InSellPrice);

	/* 쿨타임 텍스트 생성 */
	static FText BuildCooldownText(float InCooldownSeconds);

	/* 강화 수치 텍스트 생성 */
	static FText BuildUpgradeLevelText(int32 InUpgradeLevel);
#pragma endregion
};