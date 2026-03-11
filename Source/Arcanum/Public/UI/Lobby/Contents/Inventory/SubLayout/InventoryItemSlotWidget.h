#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "DataInfo/InventoryData/Data/FInventoryViewSlot.h"
#include "InventoryItemSlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UBorder;

/* 슬롯 클릭시 슬롯 인덱스 전달 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotClicked, int32, InSlotIndex);

/**
 * 추영호
 * 인벤토리 하나 슬롯
 */
UCLASS()
class ARCANUM_API UInventoryItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/* 슬롯 데이터 세팅 */ 
	void SetSlotData(const FInventoryViewSlot& InSlot, int32 InSlotIndex);

	/* 빈 슬롯 처리 */
	UFUNCTION()
	void ClearSlot(int32 InSlotIndex);

	/* 선택 강조 */
	UFUNCTION()
	void SetSelected(bool InSelected);
	
	/* 인벤 인덱스 */
	UFUNCTION()
	int32 GetSlotIndex() const { return SlotIndex; } 

private:
	/* 슬롯 버튼 눌렀을 때 호출 */
	UFUNCTION()
	void HandleSlotClicked();

	/* 현재 상태 갱신 */
	void RefreshSlotUI();


public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventorySlotClicked OnInventorySlotClicked;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> SlotButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> ItemIconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> StackOrUpgradeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UBorder> SelectedBorder;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex = INDEX_NONE;

	/* 이 슬롯이 장비/포션/빈칸 중 무엇인지 (장비면 Guid, 포션이면 Tag/Count) */
	UPROPERTY()
	FInventoryViewSlot ViewSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bSelected = false;
};

