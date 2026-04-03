#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "DataInfo/ShopData/Data/FShopRuntimeData.h"
#include "ShopHUDWidget.generated.h"

class UTextBlock;
class UCommonBtnWidget;
class UPanelWidget;
class UShopPanelWidget;
class ULobbyHUD;
struct FShopViewSlot;

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
	/* 상점 갱신될 때 선택 초기화*/
	void ClearShopSelection();

	/* 남은 시간 텍스트 표시 */
	UFUNCTION()
	void SetShopRemainingSeconds(int32 InRemainingSeconds);

	/* 패널 슬롯 개수 세팅 */
	UFUNCTION()
	void InitPanels();

protected:
	/* 부모 컨테이너에서 상점 패널 목록 수집 및 바인딩 */
	void BindShopPanels();

protected:
	/* 상점 패널들을 담는 컨테이너 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UPanelWidget> ShopPanelContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ShopTimerText;

private:
	/* 부모 컨테이너에서 수집한 상점 패널 목록 */
	UPROPERTY()
	TArray<TObjectPtr<UShopPanelWidget>> ShopPanels;

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

	/* 상점 패널 슬롯 클릭 수신 */
	UFUNCTION()
	void HandleShopSlotClicked(FGameplayTag InCategoryTag, int32 InLocalIndex);

protected:
	/* 구입 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonBtnWidget> BuyButton;

	/* 판매 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonBtnWidget> SellButton;

private:
	/* 현재 선택된 상점 카테고리 */
	UPROPERTY()
	FGameplayTag SelectedCategoryTag;
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
	/* 카테고리별 상점 표시 캐시 */
	UPROPERTY()
	TMap<FGameplayTag, FShopViewSlotList> CachedShopSlotsByCategory;
#pragma endregion
};
