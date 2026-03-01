// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "UI/Lobby/Contents/Character/CharacterHUDWidget.h"
#include "DataInfo/PlayerData/FPlayerData.h"
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

#pragma region 인벤토리
protected:
	/* 인벤토리 선택 알림 수신 */
	UFUNCTION()
	void HandleInventorySlotSelected(FGuid InItemGuid);

	/* 인벤 UI 갱신(표시용) */
	void RefreshInventoryUI();

	/* 장비 DT 캐시 구축(로비 진입 시 1회) */
	void BuildEquipmentRowCache();

	/* 캐시에서 ItemTag로 RowPtr 찾기(O(1)) */
	const FDTEquipmentInfoRow* FindEquipmentRowByTag(const FGameplayTag& InItemTag) const;

protected:
	/* 인벤토리 위젯 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryHUDWidget> InventoryHUDWidget;

	/* 인벤 슬롯 생성 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 InventorySlotCount = 50;

	/* 현재 선택된 인벤 아이템 */
	UPROPERTY()
	FGuid SelectedInventoryItemGuid;

private:
	/* ItemTag -> 장비 DT RowPtr 캐시 */
	TMap<FGameplayTag, const FDTEquipmentInfoRow*> EquipmentRowByTag;

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

protected:
	/* 상점 위젯 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UShopHUDWidget> ShopHUDWidget;

	/* 상점 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 ShopSlotCount = 5;

private:
	/* 상점 UI 표시용 RowName 카피 */
	UPROPERTY()
	TArray<FName> CachedShopRowNames;

	/* 상점 UI 표시용 품절 카피 */
	UPROPERTY()
	TArray<bool> CachedShopSoldOutStates;

	/* 상점 UI 표시용 DT row 포인터 캐시 */
	TArray<const FDTEquipmentInfoRow*> CachedShopRowPtrs;
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
