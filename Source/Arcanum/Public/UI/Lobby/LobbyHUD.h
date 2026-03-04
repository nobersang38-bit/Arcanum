#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "UI/Lobby/Contents/Character/CharacterHUDWidget.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "DataInfo/ItemData/Data/InventoryViewSlot.h"
#include "LobbyHUD.generated.h"

/*
 * 역할 : 김유진
 * 1. 메뉴 버튼 (전투, 캐릭터, 강화, 상점, 가챠)
 * 2. 설정, 종료 버튼 관리
 */

class UCommonBtnWidget;
class UCommonDialog;
class UHorizontalBox;
class UBackgroundBlur;
class UWidgetSwitcher;
class UCurrencyWidget;
class UShopHUDWidget;
class UARGameInstance;
class UInventoryHUDWidget;
struct FDTEquipmentInfoRow;
struct FDTPotionInfoRow;
struct FDTInventoryRuleItem;
class UDataTable;

UCLASS()
class ARCANUM_API ULobbyHUD : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region 데이터 캐시
public:
	/* 로비 공통 UI 갱신 (재화/인벤/상점 등) */
	void RefreshAllLobbyUI();

	/* GameInstance 델리게이트 바인딩 */
	void BindGameInstanceEvents();

	/* 재화 변경 알림 수신 */
	UFUNCTION()
	void HandleCurrencyChanged();

private:
	FPlayerData CachedPlayerData;
#pragma endregion

#pragma region 바인딩 메뉴 버튼
	// 상단 메뉴 : 전투, 캐릭터, 강화, 상점, 가챠
	// 설정, 종료

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> BattleMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> CharacterMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> EnhancementMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> ShopMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> GachaMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SettingBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> QuitBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> MenuHorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SettingUHorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBackgroundBlur> BackgroundBlur;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> CharacterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> CharacterBattleStatsDataTable;
	
private:
	UFUNCTION()
	void ClickBattleMenuBtn();

	UFUNCTION()
	void ClickCharacterMenuBtn();

	UFUNCTION()
	void ClickEnhancementMenuBtn();

	UFUNCTION()
	void ClickShopMenuBtn();

	UFUNCTION()
	void ClickGachaMenuBtn();

	UFUNCTION()
	void ClickSettingBtn();

	UFUNCTION()
	void ClickQuitBtn();

#pragma endregion

#pragma region 재화
protected:
	/* 재화 UI 갱신 */
	void RefreshLobbyCurrencyUI();

protected:
	/* 재화 위젯 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCurrencyWidget> CurrencyWidget;
#pragma endregion

#pragma region 전투

#pragma endregion

#pragma region 캐릭터
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
//	TSubclassOf<UCharacterHUDWidget> CharacterWidgetClass;
//
//private:
//	UCharacterHUDWidget* CharacterWidget = nullptr;
#pragma endregion

#pragma region 강화

#pragma endregion

#pragma region 인벤토리
protected:
	/* 인벤토리 선택 알림 수신 */
	UFUNCTION()
	void HandleInventorySlotSelected(const FInventoryViewSlot& InSlot);

	/* 인벤 UI 갱신(표시용) */
	void RefreshInventoryUI();

	/* 장비 DT 캐시 구축(로비 진입 시 1회) */
	void BuildEquipmentRowCache();

	/* 포션 DT 캐시 */
	void BuildPotionRowCache();

	/* 인벤 룰 DT 캐시 */
	void BuildInventoryRuleTableCache();

	/* 룰 테이블 기반 인벤 최대 슬롯 */
	int32 GetInventoryCapacityFromRuleTable() const;

	/* 아이템 태그 기반 MaxStack 조회 */
	bool FindMaxStackByItemTag(const FGameplayTag& InItemTag, int32& OutMaxStack) const;

	/* SlotTag의 정렬 순서 조회 */
	int32 GetSlotOrderFromRuleTable(const FGameplayTag& InSlotTag) const;

	/* 캐시에서 ItemTag로 RowPtr 찾기 */
	const FDTEquipmentInfoRow* FindEquipmentRowByTag(const FGameplayTag& InItemTag) const;

	/* 캐시에서 PotionTag로 RowPtr 찾기 */
	const FDTPotionInfoRow* FindPotionRowByTag(const FGameplayTag& InPotionTag) const;

private:
	/* 물약 앞쪽부터 그 뒤 장비 정렬 */
	void BuildInventoryViewSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const;

	/* 정렬 버튼 클릭 토글 */
	UFUNCTION()
	void ClickInventorySortBtn();

	/* 포션 스택(20) */
	void AppendPotionSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const;

	/* 장비 원본순으로 */
	void AppendEquipmentSlotsRaw(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const;

	/* 장비 정렬(강화순 투구/갑옷/장갑/신발) */
	void AppendEquipmentSlotsSorted(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const;


protected:
	/* 인벤토리 위젯 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryHUDWidget> InventoryHUDWidget;

	/* 현재 선택된 인벤 아이템 */
	UPROPERTY()
	FGuid SelectedInventoryItemGuid;

	/* 인벤 정렬 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> InventorySortBtn;

	/* InventoryRule DT의 단일 Row 캐시 */
	const FDTInventoryRuleItem* InventoryRuleRow = nullptr;

	/* SlotTag -> Order 캐시 */
	TMap<FGameplayTag, int32> SlotOrderByTag;

private:
	/* 인벤 룰 DT 포인터 캐시 */
	UPROPERTY()
	TObjectPtr<UDataTable> InventoryRuleTable;

	/* ItemTag -> 장비 DT RowPtr 캐시 */
	TMap<FGameplayTag, const FDTEquipmentInfoRow*> EquipmentRowByTag;

	/* PotionTag -> 포션 DT RowPtr 캐시 */
	TMap<FGameplayTag, const FDTPotionInfoRow*> PotionRowByTag;

	/* 정렬 토글 상태 (true 정렬)*/
	UPROPERTY()
	bool bInventorySortedView = false;
#pragma endregion

#pragma region 상점
protected:
	/* 상점 초기화 */
	void InitShop();

	/* 상점 UI 갱신 */
	void RefreshShopUI();

	/* 상점 타이머 델리게이트 바인딩 */
	void BindShopTimer();

	/* 상점 타이머 수신 */
	UFUNCTION()
	void HandleShopSecondChanged(int32 InRemainingSeconds);

	/* 아이템 구매 */
	UFUNCTION()
	void TryPurchaseSelectedItem(int32 InSlotIndex);

	/* 아이템 판매 */
	UFUNCTION()
	void TrySellSelectedItem();

private:
	/* DT 조회로 상점 표시 캐시 */
	void BuildShopRuntimeCache();

	/* 로비 복귀 시 타이머 재개 */
	void RestartShopTimer();

protected:
	/* 상점 위젯 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UShopHUDWidget> ShopHUDWidget;

	/* 장비 상점 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 EquipmentShopSlotCount = 5;

	/* 물약 상점 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 PotionShopSlotCount = 6;

private:
	/* 상점 슬롯 상품 키(TableTag+RowName) 표시 캐시 */
	UPROPERTY()
	TArray<FGameplayTag> CachedShopTableTags;

	/* 상점 UI 표시용 RowName 카피 */
	UPROPERTY()
	TArray<FName> CachedShopRowNames;

	/* 상점 UI 표시용 품절 카피 */
	UPROPERTY()
	TArray<bool> CachedShopSoldOutStates;

	/* 상점 슬롯 표시용 공통 캐시 */
	UPROPERTY()
	TArray<TSoftObjectPtr<UTexture2D>> CachedShopIcons;
	UPROPERTY()
	TArray<FText> CachedShopNames;
	UPROPERTY()
	TArray<FText> CachedShopDescs;
	UPROPERTY()
	TArray<int64> CachedShopPrices;
#pragma endregion


#pragma endregion

#pragma region 가챠

#pragma endregion

#pragma region 설정

#pragma endregion

#pragma region 종료
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonDialog> ExitCommonDialog;
private:
	UFUNCTION()
	void OnExitCommonDialog(EDialogResult res);
#pragma endregion

};
