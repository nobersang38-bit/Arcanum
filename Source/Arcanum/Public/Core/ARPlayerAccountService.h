#pragma once
#include "CoreMinimal.h"
#include "GameplayTags/ArcanumTags.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "Core/Interfaces/IPlayerAccountService.h"
#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"
#include "DataInfo/BattleCharacter/CharacterInfo/DataTable/DTCharacterBaseInfo.h"
#include "DataInfo/GachaData/DataTable/DTGachaBannerData.h"

#include "DataInfo/ItemData/DataTable/DTPotionInfoRow.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "DataInfo/InventoryData/Data/InventoryViewSlot.h"

#include "ARPlayerAccountService.generated.h"

UENUM(BlueprintType)
enum class EShopRarityType : uint8
{
	Common,
	Set,
	Legendary
};

UENUM(BlueprintType)
enum class EDisposeType : uint8
{
	Sell,                // 판매(골드)
	DisassembleItem,     // 장비/무기 분해(소울)
	DisassembleCharacter // 캐릭터 분해(조각)
};

class UARGameInstance;
struct FGachaItemResult;
enum class EHUDIndex : uint8;

/**
 * PlayerAccountService
 * - PlayerData를 수정하는 유일한 진입점
 * - UI 또는 외부 시스템은 PlayerData를 직접 수정하지 않는다.
 * - 추후 해당 로직 및 GameInstance(DB)는 서버로 그대로 옮기면 됨.
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveCompleted, bool, bSuccess);

class FPlayerAccountService : public IPlayerAccountService
{
#pragma region 인터페이스(추후 서버 대비용 예시)
public:
	// 인터페이스 구현
	virtual void GetIPlayerDataCopy(const UObject* WorldContextObject, TFunction<void(bool)> OptionalCallback) override;
	virtual FOnPlayerDataReceived& OnPlayerDataReceived() override { return PlayerDataReceivedDelegate; }
private:
	FOnPlayerDataReceived PlayerDataReceivedDelegate;
#pragma endregion


#pragma region PlayerData Getter
public:
	/* 사용 예
	   const FPlayerCurrency Currency = FPlayerAccountService::GetPlayerCurrency(this);
	 */

	 // 플레이어 전체 데이터 카피
	static const FPlayerData GetPlayerDataCopy(const UObject* WorldContextObject);
	// 재화
	UFUNCTION(BlueprintCallable)
	static const FPlayerCurrency GetPlayerCurrency(const UObject* WorldContextObject);
	// 전투 기본 데이터
	UFUNCTION(BlueprintCallable)
	static const FPlayerBattleData GetPlayerBattleData(const UObject* WorldContextObject);
	// 보유 캐릭터
	UFUNCTION(BlueprintCallable)
	static const TArray<FBattleCharacterData> GetOwnedCharacters(const UObject* WorldContextObject);
	// 인벤토리
	UFUNCTION(BlueprintCallable)
	static const TArray<FEquipmentInfo> GetInventory(const UObject* WorldContextObject);
	// 스테이지 진행도
	UFUNCTION(BlueprintCallable)
	static const TMap<FGameplayTag, FStageProgressData> GetStageProgressMap(const UObject* WorldContextObject);
	// 가챠 상태
	UFUNCTION(BlueprintCallable)
	static const FGachaData GetGachaState(const UObject* WorldContextObject);
	// 퀘스트 상태
	UFUNCTION(BlueprintCallable)
	static const FPlayerQuest GetQuestState(const UObject* WorldContextObject);
#pragma endregion

#pragma region 레벨 변경 시 호출 함수
	/** 레벨 변경 후 되돌아올때, 현재 HUD 위치 저장하는 함수*/
	static void SetHUDIndex(const UObject* WorldContextObject, const int HudIndex);
	static void SetHUDIndex(const UObject* WorldContextObject, const EHUDIndex HudIndex);
	static int32 GetHUDIndex(const UObject* WorldContextObject);
#pragma endregion


#pragma region PlayerData Updater
public:
	/** 간편 재화 추가 */
	static const bool AddCurrency(const UObject* WorldContextObject, FGameplayTag Tag, int64 Amount);
	/** 플레이어 재화 변경할때*/
	static const FPlayerCurrency UpdateCurrency(const UObject* WorldContextObject, const FPlayerData& PlayerData, FGameplayTag Tag, int64 Amount);
private:
	/** 치트 방지용*/
	static bool VerifyCurrency(UARGameInstance* GI, FPlayerData CachedData);
	/** 세이브 삭제, 강제종료 */
	static void VerifiedFailure(UARGameInstance* GI);
#pragma endregion


#pragma region 로그인 관련
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	static FOnSaveCompleted OnSaveCompleted;
	static bool LoadPlayerData(const UObject* WorldContextObject);
	static bool SavePlayerData(const UObject* WorldContextObject);
private:
	static bool SavePlayerData(UARGameInstance* GI);
#pragma endregion


#pragma region Battle Widget 관련
public:
	/* 스테이지 입장 시 상점 타이머 정지 */
	static void StopShopOnBattleStart(const UObject* WorldContextObject);
#pragma endregion

#pragma region Character Widget 관련
public:

#pragma endregion

#pragma region Enhancement Widget 관련

#pragma endregion

#pragma region Shop Widget 관련
public:
	/** DT의 장비목록을 반환하는 함수*/
	TArray<FName> GetEquipmentList(const UObject* WorldContextObject);
	/** 상점에서 구매시 사용하는 함수*/
	static bool PurchaseEquipment(const UObject* WorldContextObject, FName RowName);
	/** */
	static const FDTEquipmentInfoRow* GetItemDefinition(UGameDataSubsystem* DataSubsystem, const FGameplayTag& ItemTag);
	/* 장비 DT에서 ItemTag로 Row를 찾는다 */
	//static const FDTEquipmentInfoRow* GetRowByItemTag(const UObject* WorldContextObject, const FGameplayTag& InItemTag);

	/* Guid(장비) 아이템 판매 */
	static bool SellItemByGuid(const UObject* WorldContextObject, const FGuid& InItemGuid);

	/* stack(물약) 아이템 판매 */
	static bool SellStackItemByTag(const UObject* WorldContextObject, const FGameplayTag& InItemTag, int32 InSellCount);

	/* 상점 진입 시 초기화 (저장시간 확인 후 유지/갱신 판정) */
	static void InitializeShop(const UObject* WorldContextObject, int32 InEquipmentSlotCount, int32 InPotionSlotCount);

	/* 상점 강제 갱신 (10분 만료시) */
	static void RefreshShop(const UObject* WorldContextObject, int32 InEquipmentSlotCount, int32 InPotionSlotCount);

	/* 상점 슬롯 구매(TableTag 기준으로 포션 / 장비 분기) */
	static bool PurchaseShopSlot(const UObject* WorldContextObject, int32 InSlotIndex);

	/* 구매 성공 후 슬롯 품절 처리 */
	static bool SetShopSlotSoldOut(const UObject* WorldContextObject, int32 InSlotIndex);

	/* 저장된 상점 시간이 유효한지 확인 */
	static bool IsShopRefreshExpired(const UObject* WorldContextObject);

	/* 다음 갱신 시각 기준 남은 초 계산 (UI 타이머 표시용) */
	static int32 GetShopRemainingSeconds(const UObject* WorldContextObject);

	/* 현재 시간 반환 */
	static FDateTime GetCurrentTimeKST();

	/* 스택 보유 수량 조회 */
	static int32 GetStackItemCountByTag(const UObject* WorldContextObject, const FGameplayTag& InItemTag);

private:
	/* 상점 슬롯 전체 생성 */
	static void GenerateShopItems(UARGameInstance* InGameInstance, int32 InEquipmentSlotCount, int32 InPotionSlotCount, bool bInRefreshEquipmentOnly);

	/* 상점 저장 데이터 초기화 */
	static void ResetShopSoldOutStates(UARGameInstance* InGameInstance, int32 InEquipmentSlotCount);

	/* 다음 갱신 시각 설정 (현재시간 + 10분) */
	static void SetNextShopRefreshTime(UARGameInstance* InGameInstance);

	/* ItemCatalog에서 StorePolicyTag로 RowName 목록 수집 */
	static void BuildCatalogRowNamesByStorePolicy(
		UARGameInstance* InGameInstance,
		const FGameplayTag& InStorePolicyTag,
		TArray<FName>& OutRowNames);

	/* RowName 목록에서 하나 랜덤 선택 후 목록에서 제거 */
	static FName PickCatalogRowNameFromRowNames(TArray<FName>& InOutRowNames);

	using FAddGuidHandler = bool(*)(const UObject* WorldContextObject, const FDTItemCatalogRow* InCatalogRow);

	/* DetailTableTag에 맞는 Guid 아이템 핸들러 */
	static FAddGuidHandler FindGuidAddHandler(const FGameplayTag& InDetailTableTag);

	/* Guid 아이템을 카탈로그 정보로 인스턴스 생성해 인벤에 추가 */
	static bool AddGuidByCatalog(const UObject* WorldContextObject, const FDTItemCatalogRow* InCatalogRow);

	/* Equipment 전용: 카탈로그 DetailRowName으로 장비 인스턴스 생성 후 Inventory에 추가 */
	static bool AddGuidFromEquipment(const UObject* WorldContextObject, const FDTItemCatalogRow* InCatalogRow);

	/* Guid 아이템 1개 추가 시 인벤 슬롯 여유가 있는지 확인 */
	static bool CanAddGuidItem(const FPlayerData& InPlayerData, UGameDataSubsystem* InDataSubsystem, int32 InInventoryCapacity);

	/* 스택 아이템 추가 시 인벤 슬롯 여유가 있는지 확인 */
	static bool CanAddStackItem(const FPlayerData& InPlayerData, UGameDataSubsystem* InDataSubsystem, const FGameplayTag& InItemTag, int32 InAddCount, int32 InInventoryCapacity);
	static int32 GetUsedStackSlotCount(const FPlayerData& InPlayerData, UGameDataSubsystem* InDataSubsystem);
	static int32 GetExtraStackSlotsNeeded(int32 InOldCount, int32 InAddCount, int32 InMaxStack);

#pragma endregion


#pragma region Gacha Widget 관련
public:
	static const FDTGachaBannerDataRow* GetGachaBannerData(const UObject* WorldContextObject, FGameplayTag InBannerTag);
	static void GetActiveGachaBannerRows(const UObject* WorldContextObject, TArray<const FDTGachaBannerDataRow*>& OutRows);
	static bool ExecuteGacha(const UObject* WorldContextObject, const FPlayerData& PlayerData, FGameplayTag BannerTag, FCurrencyCost Cost, int32 PullCount);
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