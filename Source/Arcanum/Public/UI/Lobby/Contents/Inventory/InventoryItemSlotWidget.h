#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "InventoryItemSlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UBorder;
struct FDTEquipmentInfoRow;
struct FEquipmentInfo;

/* 슬롯 클릭시 슬롯 인덱스 전달 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotClicked, FGuid, InItemGuid);

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
	/* 슬롯 데이터 세팅 (장비 1개) */ 
	void SetItemData(const FEquipmentInfo& InItem, const FDTEquipmentInfoRow* InRow, int32 InSlotIndex);

	/* 빈 슬롯 처리 */
	UFUNCTION()
	void ClearSlot(int32 InSlotIndex);

	/* 선택 강조 */
	UFUNCTION()
	void SetSelected(bool InSelected);

	/* 판매 가능(빈슬롯) 여부 */
	bool IsValidItem() const { return !bEmpty; } 

	/* 인벤 인덱스 */
	UFUNCTION()
	int32 GetSlotIndex() const { return SlotIndex;  } 

	UFUNCTION()
	/* 장비 인스턴스 식별자(동일 ItemTag 여러 개 구분용) */
	FGuid GetItemGuid() const { return ItemGuid;  } 

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> UpgradeText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UBorder> SelectedBorder;

private:
	/* 슬롯 UI 표시용 DT 장비 Row 캐시(아이콘/이름) */
	const FDTEquipmentInfoRow* CachedRowPtr = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bEmpty = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bSelected = false;

	UPROPERTY()
	FGameplayTag ItemTag;

	UPROPERTY()
	FGuid ItemGuid;

	UPROPERTY()
	int32 CurrUpgradeLevel = 0;
};

