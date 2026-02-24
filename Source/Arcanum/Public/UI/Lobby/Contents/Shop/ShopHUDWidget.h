#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopHUDWidget.generated.h"

class UCurrencyWidget;
class UWrapBox;
class UShopItemSlotWidget;


/**
 * 추영호
 * - 상점 HUD 
 */

UCLASS()
class ARCANUM_API UShopHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

#pragma region 상점 슬롯 생성
protected:
	/* 상점 슬롯 위젯들 생성 */
	void CreateShopSlots();

protected:
	/* 상점 슬롯 컨테이너 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> ShopSlotContainer;

	/* 상점 슬롯 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UShopItemSlotWidget> ShopItemSlotWidgetClass;

	/* 생성할 상점 슬롯 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (ClampMin = "1", AllowPrivateAccess = "true"))
	int32 ShopSlotCount = 4;

	/* 생성된 상점 슬롯들 */
	UPROPERTY()
	TArray<TObjectPtr<UShopItemSlotWidget>> ShopSlots;
#pragma endregion

#pragma region 상점 슬롯 이벤트 바인딩
protected:
	/* 상점 슬롯 클릭 델리게이트 바인딩 */
	void BindShopSlotEvents();
#pragma endregion

#pragma region 상점 슬롯 선택
protected:
	/* 슬롯 클릭 처리 */
	UFUNCTION()
	void HandleShopSlotClicked(int32 InSlotIndex);

	/* 슬롯 선택 강조 갱신 */
	void RefreshShopSlotSelection();

protected:
	/* 현재 선택된 슬롯 인덱스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SelectedShopSlotIndex = INDEX_NONE;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UCurrencyWidget> CurrencyWidget;
	
};
