#include "UI/Lobby/Contents/Inventory/InventoryItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UInventoryItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.RemoveDynamic(this, &UInventoryItemSlotWidget::HandleSlotClicked);
		SlotButton->OnClicked.AddDynamic(this, &UInventoryItemSlotWidget::HandleSlotClicked);
	}

	RefreshSlotUI();
}

void UInventoryItemSlotWidget::SetItemData(const FEquipmentInfo& InItem, int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;

	ItemTag = InItem.ItemTag;
	ItemGuid = InItem.ItemGuid;

	bEmpty = false;

	RefreshSlotUI();
}
void UInventoryItemSlotWidget::ClearSlot(int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;

	ItemTag = FGameplayTag();
	ItemGuid = FGuid();
	CurrUpgradeLevel = 0;

	bEmpty = true;
	bSelected = false;

	RefreshSlotUI();
}

void UInventoryItemSlotWidget::SetSelected(bool InSelected)
{
	if (!bEmpty)
	{
		bSelected = InSelected;
	}
	else
	{
		bSelected = false;
	}

	RefreshSlotUI();
}

void UInventoryItemSlotWidget::HandleSlotClicked()
{
	if (!bEmpty)
	{
		OnInventorySlotClicked.Broadcast(SlotIndex);
	}
}
void UInventoryItemSlotWidget::RefreshSlotUI()
{
	if (SlotButton)
	{
		SlotButton->SetIsEnabled(!bEmpty);
	}

	if (SelectedBorder)
	{
		SelectedBorder->SetVisibility((!bEmpty && bSelected) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	
	//if (Ite

}
