#pragma once
#include "CoreMinimal.h"
#include "GameplayTags/ArcanumTags.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/PlayerData/FPlayerData.h"

#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"


class UARGameInstance;

/**
 * PlayerAccountService
 * - PlayerData를 수정하는 유일한 진입점
 * - UI 또는 외부 시스템은 PlayerData를 직접 수정하지 않는다.
 * - 추후 해당 로직 및 GameInstance(DB)는 서버로 그대로 옮기면 됨.
 */


class FPlayerAccountService
{
#pragma region Battle Widget 관련
public:

#pragma endregion

#pragma region Character Widget 관련
public:

#pragma endregion

#pragma region Enhancement Widget 관련

#pragma endregion

#pragma region Shop Widget 관련
public:
    static bool PurchaseEquipment(UARGameInstance* GameInstance, FName RowName);
    static const FDTEquipmentInfoRow* GetItemDefinition(UGameDataSubsystem* DataSubsystem, const FGameplayTag& ItemTag);

	/* 상점 진입 시 초기화 (저장시간 확인 후 유지/갱신 판정) */
	static void InitializeShop(UARGameInstance* InGameInstance, int32 InShopSlotCount);

	/* 상점 강제 갱신 (10분 만료시) */
	static void RefreshShop(UARGameInstance* InGameInstance, int32 InShopSlotCount);

	/* 상점 슬롯 데이터 조회 (UI 표시용) */
	static bool GetShopSlotData(UARGameInstance* InGameInstance, int32 InSlotIndex, FName& OutRowName, bool& OutSoldOut);

	/* 구매 성공 후 슬롯 품절 처리 */
	static bool SetShopSlotSoldOut(UARGameInstance* InGameInstance, int32 InSlotIndex);

	/* 다음 갱신 시각 기준 남은 초 계산 (UI 타이머 표시용) */
	static int32 GetShopRemainingSeconds(UARGameInstance* InGameInstance);

private:
	/* 저장된 상점 시간이 유효한지 확인 */
	static bool IsShopRefreshExpired(UARGameInstance* InGameInstance);

	/* 새 상점 아이템 뽑기 */
	// static void GenerateShopItems(UARGameInstance* InGameInstance, int32 InShopSlotCount);
	/* 상점 아이템 후보군 구성 (일반/세트/전설 분류) */
	//static void BuildShopItemPools
	/* 상점 등급 확률 판정 (전설/세트/일반) */
	//static EShopRarityType PickShopRarityType(UARGameInstance* InGameInstance);
	/* 상점 아이템 1개 선택 (fallback + 중복제거 포함) */
	//static FName PickShopItemRow
	/* 상점 슬롯 전체 생성 */
	//static void GenerateShopItems(UARGameInstance* InGameInstance, int32 InShopSlotCount);
	/*
	USTRUCT()
    struct FShopItemPools
    {
	GENERATED_BODY()

	UPROPERTY()
	TArray<FName> CommonRows;

	UPROPERTY()
	TArray<FName> SetRows;

	UPROPERTY()
	TArray<FName> LegendaryRows;
    };
	*/

	/* 상점 저장 데이터 초기화 */
	static void ResetShopSoldOutStates(UARGameInstance* InGameInstance, int32 InShopSlotCount);

	/* 다음 갱신 시각 설정 (현재시간 + 10분) */
	static void SetNextShopRefreshTime(UARGameInstance* InGameInstance);

	/* 현재 시간 반환 */
	static FDateTime GetCurrentTimeKST();
#pragma endregion

#pragma region Gacha Widget 관련
public:

#pragma endregion

};

//static bool PromoteCharacter(FPlayerData& Data, FGameplayTag CharacterID);
//static bool SpendCurrency(FPlayerData& Data, int32 GoldCost);
//static bool RollEquipmentGacha(FPlayerData& Data);

///** 특정 슬롯의 스택을 마일리지(Soul)로 환전하고 스택을 0으로 리셋 */
//static void SettleGachaStackToMileage(FPlayerData& Data, FGameplayTag SlotTag)
//{
//    if (int32* StackPtr = Data.GachaData.PityCounters.Find(SlotTag))
//    {
//        int32 StackCount = *StackPtr;
//        if (StackCount > 0) {
//            AddCurrency(Data, Arcanum::Currency::Soul, StackCount);
//            Data.GachaData.PityCounters.Add(SlotTag, 0);
//        }
//    }
//}

///** 가챠 실행 후 결과를 데이터에 기록. 
//  * bIsPickupHit가 true이면(주인공 뽑음) 스택 리셋, 아니면 +1
//  */
//static void ApplyGachaResult(FPlayerData& Data, FGameplayTag SlotTag, bool bIsPickupHit)
//{
//    int32& CurrentStack = Data.GachaData.PityCounters.FindOrAdd(SlotTag);

//    if (bIsPickupHit || CurrentStack >= 99) CurrentStack = 0;
//    else CurrentStack++;
//}