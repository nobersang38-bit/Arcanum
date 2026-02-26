#include "UI/Lobby/Contents/Inventory/InventoryItemSlotWidget.h"
#include "DataInfo\BattleCharacter\Equipment\Data\FEquipmentData.h"
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

void UInventoryItemSlotWidget::SetItemData(const FEquipmentInfo& InItem, const FDTEquipmentInfoRow& InRowPtr, int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;

	ItemTag = InItem.ItemTag;
	ItemGuid = InItem.ItemGuid;
	CurrUpgradeLevel = InItem.CurrUpgradeLevel;

	bEmpty = false;
	bSelected = false;

	// 아이콘 const 오류인듯
	if (ItemIconImage)
	{
		if (!InRowPtr || InRowPtr->Icon.IsNull())
		{
			ItemIconImage->SetVisibility(ESlateVisibility::Collapsed);
			ItemIconImage->SetBrushFromSoftTexture(nullptr);
		}
		else
		{
			ItemIconImage->SetVisibility(ESlateVisibility::Visible);
			ItemIconImage->SetBrushFromSoftTexture(InRowPtr->Icon.Get());
		}
	}

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

	// 강화 표시 텍스트
	if (UpgradeText)
	{
		if (!bEmpty && CurrUpgradeLevel > 0)
		{
			UpgradeText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), CurrUpgradeLevel)));
		}
		else
		{
			UpgradeText->SetText(FText::GetEmpty());
		}
	}
}
