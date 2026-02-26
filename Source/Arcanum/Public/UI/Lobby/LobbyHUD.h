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
protected:
	/* 로비 공통 UI 갱신 (재화/인벤/상점 등) */
	void RefreshAllLobbyUI();

	/* GameInstance 델리게이트 바인딩 */
	void BindGameInstanceEvents();

	/* 재화 변경 알림 수신 */
	UFUNCTION()
	void HandleCurrencyChanged();

public:
	/* 로비에서만 쓰는 플레이어 데이터 캐시 */
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

	// 선택 아이템 구매
	UFUNCTION()
	void TryPurchaseSelectedItem(FName InItemRowName);

private:
	/* DT 조회로 상점 표시 캐시 */
	void BuildShopRuntimeCache();

protected:
	/* 상점 위젯 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UShopHUDWidget> ShopHUDWidget;

	/* 상점 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 ShopSlotCount = 4;

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
