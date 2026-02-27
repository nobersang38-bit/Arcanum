#include "UI/Lobby/Contents/Inventory/InventoryItemSlotWidget.h"
#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"
#include "DataInfo/BattleCharacter/Equipment/Data/FEquipmentData.h"
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

void UInventoryItemSlotWidget::SetItemData(const FEquipmentInfo& InItem, const FDTEquipmentInfoRow* InRowPtr, int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;

	ItemTag = InItem.ItemTag;
	ItemGuid = InItem.ItemGuid;
	CurrUpgradeLevel = InItem.CurrUpgradeLevel;

	CachedRowPtr = InRowPtr;
	bEmpty = !ItemGuid.IsValid();

	bSelected = false;

	RefreshSlotUI();
}
void UInventoryItemSlotWidget::ClearSlot(int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;

	ItemTag = FGameplayTag();
	ItemGuid.Invalidate();
	CurrUpgradeLevel = 0;

	bEmpty = true;
	bSelected = false;

	CachedRowPtr = nullptr;

	RefreshSlotUI();
}

void UInventoryItemSlotWidget::SetSelected(bool InSelected)
{
	bSelected = InSelected;

	RefreshSlotUI();
}

void UInventoryItemSlotWidget::HandleSlotClicked()
{
	if (!bEmpty)
	{
		OnInventorySlotClicked.Broadcast(ItemGuid);
	}
}

void UInventoryItemSlotWidget::RefreshSlotUI()
{
	if (SlotButton)
	{
		SlotButton->SetIsEnabled(!bEmpty && CachedRowPtr);
	}

	if (SelectedBorder)
	{
		SelectedBorder->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (bEmpty || !CachedRowPtr)
	{
		if (ItemIconImage)
		{
			ItemIconImage->SetBrushFromSoftTexture(nullptr);
			ItemIconImage->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (UpgradeText)
		{
			UpgradeText->SetText(FText::GetEmpty());
			UpgradeText->SetVisibility(ESlateVisibility::Collapsed);
		}

		return;
	}

	if (ItemIconImage)
	{
		ItemIconImage->SetVisibility(ESlateVisibility::Visible);
		ItemIconImage->SetBrushFromSoftTexture(CachedRowPtr->Icon.IsNull() ? nullptr : CachedRowPtr->Icon);
	}

	if (UpgradeText)
	{
		if (CurrUpgradeLevel > 0)
		{
			UpgradeText->SetVisibility(ESlateVisibility::Visible);
			UpgradeText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), CurrUpgradeLevel)));
		}
		else
		{
			UpgradeText->SetText(FText::GetEmpty());
			UpgradeText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
