#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopHUDWidget.generated.h"

class UTextBlock;
class UCommonBtnWidget;
class UShopPanelWidget;
class ULobbyHUD;

/**
 * 추영호
 * - 상점 HUD 표시
 */

UCLASS()
class ARCANUM_API UShopHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetParentLobby(ULobbyHUD* InLobby) { ParentLobby = InLobby; }
private:
	UPROPERTY() 
	TObjectPtr<ULobbyHUD> ParentLobby;

protected:
	virtual void NativeConstruct() override;


#pragma region 상점 UI 갱신
public:
	/* 상점 UI 갱신 */
	void RefreshShopUI();

private:
	/* DT 조회로 상점 표시 캐시 */
	void BuildShopRuntimeCache();
#pragma endregion

#pragma region 상점 슬롯
public:
	/* 상점 캐시 */
	void ApplyShopData(
		const TArray<FName>& InRowNames,
		const TArray<bool>& InSoldOutStates,
		const TArray<TSoftObjectPtr<UTexture2D>>& InIcons,
		const TArray<FText>& InNames,
		const TArray<FText>& InDescs,
		const TArray<int64>& InPrices);

	/* 상점 갱신될 때 선택 초기화*/
	void ClearShopSelection();

	/* 남은 시간 텍스트 표시 */
	UFUNCTION()
	void SetShopRemainingSeconds(int32 InRemainingSeconds);

	/* 패널 슬롯 개수 세팅 */
	UFUNCTION()
	void InitPanels();

protected:
	/* 장비 패널 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UShopPanelWidget> EquipmentPanel;

	/* 물약 패널 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UShopPanelWidget> PotionPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ShopTimerText;

	/* 장비 상점 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 EquipmentShopSlotCount = 5;

	/* 물약 상점 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	int32 PotionShopSlotCount = 6;

private:
	/* 현재 선택 인덱스 */
	UPROPERTY()
	int32 SelectedSlotIndex = INDEX_NONE;
#pragma endregion

#pragma region 구매 판매 버튼
protected:
	// 버튼 클릭 처리
	UFUNCTION()
	void HandleBuyClicked();

	UFUNCTION()
	void HandleSellClicked();

	/* 패널 슬롯 클릭 수신 */
	UFUNCTION()
	void HandleEquipmentSlotClicked(int32 InSlotIndex);
	UFUNCTION()
	void HandlePotionSlotClicked(int32 InSlotIndex);

protected:
	/* 구입 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonBtnWidget> BuyButton;

	/* 판매 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonBtnWidget> SellButton;
#pragma endregion

#pragma region 상점 타이머
public: 
	/* 상점 초기화 */
	void InitShop();

	/* 로비 복귀 시 타이머 재개 */
	void RestartShopTimer();

	/* 상점 타이머 델리게이트 바인딩 */
	void BindShopTimer();

protected:
	/* 상점 타이머 수신 */
	UFUNCTION()
	void HandleShopSecondChanged(int32 InRemainingSeconds);
#pragma endregion

#pragma region 상점 표시 캐시
private:
	UPROPERTY()
	TArray<FName> CachedShopRowNames;

	UPROPERTY()
	TArray<bool> CachedShopSoldOutStates;

	UPROPERTY()
	TArray<TSoftObjectPtr<UTexture2D>> CachedShopIcons;

	UPROPERTY()
	TArray<FText> CachedShopNames;

	UPROPERTY()
	TArray<FText> CachedShopDescs;

	UPROPERTY()
	TArray<int64> CachedShopPrices;
#pragma endregion
};
