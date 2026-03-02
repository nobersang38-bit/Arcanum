#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopPanelWidget.generated.h"

/**
 * 추영호
 * 상점 컨테이너
 */

 /* 슬롯 클릭 시 전달 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopPanelSlotClicked, int32, InSlotIndex);

class UScrollBox;
class UWrapBox;
class UShopItemSlotWidget;
class UTexture2D;

UCLASS()
class ARCANUM_API UShopPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/* 패널 슬롯 생성 (슬롯 개수) */
	UFUNCTION()
	void InitSlots(int32 InSlotCount, int32 InStartIndex);

	/* 표시 데이터 적용 */
	void ApplyData(
		const TArray<FName>& InRowNames,
		const TArray<bool>& InSoldOutStates,
		const TArray<TSoftObjectPtr<UTexture2D>>& InIcons,
		const TArray<FText>& InNames,
		const TArray<FText>& InDescs,
		const TArray<int64>& InPrices
	);

	/* 선택 초기화 */
	void ClearSelection();

	/* 현재 선택된 Index */
	int32 GetSelectedGlobalSlotIndex() const { return SelectedShopSlotIndex; }

private:
	/* 패널 슬롯 위젯 생성 */
	void CreateSlots();

	/* 패널 슬롯 클릭 바인딩 */
	void BindSlotEvents();

	/* 패널 슬롯 클릭 처리 (구매 가능일 때만 선택) */
	UFUNCTION()
	void HandleSlotClicked(int32 InGlobalSlotIndex);

	/* 패널 선택 강조 갱신 */
	void RefreshSelection();

public:
	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnShopPanelSlotClicked OnSlotClicked;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UScrollBox> Scroll;

	/* 슬롯 컨테이너 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> SlotContainer;

	/* 슬롯 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UShopItemSlotWidget> SlotWidgetClass;

private:
	UPROPERTY()
	TArray<TObjectPtr<UShopItemSlotWidget>> Slots;

	/* CurrentShopKeys 상점 전체 배열중 시작 인덱스 
	   장비 패널 StartIndex=0, 물약 패널 StartIndex=EquipCount */
	UPROPERTY()
	int32 StartIndex = 0;

	UPROPERTY()
	int32 SlotCount = 0;

	UPROPERTY()
	int32 SelectedShopSlotIndex = INDEX_NONE;
};
