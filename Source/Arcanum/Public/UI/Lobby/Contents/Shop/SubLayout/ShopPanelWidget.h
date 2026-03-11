#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "ShopPanelWidget.generated.h"


class UScrollBox;
class UWrapBox;
class UShopItemSlotWidget;
class UTexture2D;
struct FShopViewSlot;

/* 슬롯 클릭 시 전달 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShopPanelSlotClicked, FGameplayTag, InCategoryTag, int32, InLocalIndex);

/**
 * 추영호
 * 상점 컨테이너
 */

UCLASS()
class ARCANUM_API UShopPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/* 패널 슬롯 생성 (슬롯 개수) */
	UFUNCTION()
	void InitSlots(int32 InSlotCount);

	/* 표시 데이터 적용 */
	void ApplyData(const TArray<FShopViewSlot>& InShopViewSlots);

	/* 선택 초기화 */
	void ClearSelection();

	/* 패널 슬롯 비우기 */
	void ClearSlots();

	/* 패널 선택 강조 갱신 */
	void RefreshSelection();

	/* 현재 선택된 로컬 Index */
	int32 GetSelectedLocalSlotIndex() const { return SelectedShopSlotIndex; }

private:
	/* 패널 슬롯 위젯 생성 */
	void CreateSlots();

	/* 패널 슬롯 클릭 바인딩 */
	void BindSlotEvents();

	/* 패널 슬롯 클릭 처리 */
	UFUNCTION()
	void HandleSlotClicked(int32 InGlobalSlotIndex);

public:
	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnShopPanelSlotClicked OnSlotClicked;

	/* 패널 카테고리 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	FGameplayTag CategoryTag;

protected:
	/* 슬롯 컨테이너 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> SlotContainer;

	/* 슬롯 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UShopItemSlotWidget> SlotWidgetClass;

private:
	UPROPERTY()
	TArray<TObjectPtr<UShopItemSlotWidget>> Slots;

	UPROPERTY()
	int32 SlotCount = 0;

	/* 현재 선택된 로컬 슬롯 인덱스 */
	UPROPERTY()
	int32 SelectedShopSlotIndex = INDEX_NONE;
};
