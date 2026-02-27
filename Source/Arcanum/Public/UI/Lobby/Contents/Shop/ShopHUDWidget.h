#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopHUDWidget.generated.h"

class UWrapBox;
class UShopItemSlotWidget;
class UTextBlock;
class UButton;
class UCommonBtnWidget;
struct FDTEquipmentInfoRow;

/* 구입 요청 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopBuyRequested, FName, InRowName);

/* 판매 요청 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopSellRequested);

/**
 * 추영호
 * - 상점 HUD 표시
 */

UCLASS()
class ARCANUM_API UShopHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

#pragma region 상점 슬롯
public:
	/* 로비HUD가 슬롯 개수 내려주면 여기서 슬롯 생성 */
	UFUNCTION()
	void InitShopSlots(int32 InShopSlotCount);

	/* 로비 HUD가 만든 상점 캐시 */
	void ApplyShopData(
		const TArray<FName>& InRowNames,
		const TArray<bool>& InSoldOutStates,
		const TArray<const FDTEquipmentInfoRow*>& InRowPtrs);

	/* 상점 갱신될 때 선택 초기화*/
	void ClearShopSelection();

	/* 남은 시간 텍스트 표시 */
	UFUNCTION()
	void SetShopRemainingSeconds(int32 InRemainingSeconds);

protected:
	/* 상점 슬롯 위젯들 생성 */
	void CreateShopSlots();

	/* 상점 슬롯 클릭 델리게이트 바인딩 */
	void BindShopSlotEvents();

	/* 슬롯 클릭 처리 */
	UFUNCTION()
	void HandleShopSlotClicked(int32 InSlotIndex);

	/* 슬롯 선택 강조 갱신 */
	void RefreshShopSlotSelection();

protected:
	/* 상점 슬롯 컨테이너 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> ShopSlotContainer;

	/* 상점 슬롯 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UShopItemSlotWidget> ShopItemSlotWidgetClass;

	/* 생성된 상점 슬롯들 */
	UPROPERTY()
	TArray<TObjectPtr<UShopItemSlotWidget>> ShopSlots;

	/* 현재 선택된 슬롯 인덱스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SelectedShopSlotIndex = INDEX_NONE;

private:
	/* 로비HUD에서 내려준 값 */
	UPROPERTY()
	int32 ShopSlotCount = 0;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ShopTimerText;
#pragma endregion

#pragma region 구매_판매_버튼
protected:
	// 버튼 클릭 처리
	UFUNCTION()
	void HandleBuyClicked();

	UFUNCTION()
	void HandleSellClicked();

public:
	/* 로비HUD가 바인딩해서 실제 구매/판매 실행 */
	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnShopBuyRequested OnBuyRequested;

	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnShopSellRequested OnSellRequested;

protected:
	/* 구입 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonBtnWidget> BuyButton;

	/* 판매 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCommonBtnWidget> SellButton;
#pragma endregion
};
