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

void UInventoryItemSlotWidget::SetSlotData(const FInventoryViewSlot& InSlot, int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;
	ViewSlot = InSlot;
	bSelected = false;

	RefreshSlotUI();
}
void UInventoryItemSlotWidget::ClearSlot(int32 InSlotIndex)
{
	SlotIndex = InSlotIndex;

	ViewSlot = FInventoryViewSlot();

	bSelected = false;

	RefreshSlotUI();
}

void UInventoryItemSlotWidget::SetSelected(bool InSelected)
{
	bSelected = InSelected;

	RefreshSlotUI();
}

void UInventoryItemSlotWidget::HandleSlotClicked()
{
	if (ViewSlot.Type != EInventoryViewSlotType::Empty)
	{
		OnInventorySlotClicked.Broadcast(SlotIndex);
	}
}

void UInventoryItemSlotWidget::RefreshSlotUI()
{
	const bool bIsEmpty = (ViewSlot.Type == EInventoryViewSlotType::Empty);

	if (SlotButton)
	{
		SlotButton->SetIsEnabled(!bIsEmpty);
	}

	if (SelectedBorder)
	{
		SelectedBorder->SetVisibility((!bIsEmpty && bSelected) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (bIsEmpty)
	{
		if (ItemIconImage)
		{
			ItemIconImage->SetBrushFromSoftTexture(nullptr);
			ItemIconImage->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (StackOrUpgradeText)
		{
			StackOrUpgradeText->SetText(FText::GetEmpty());
			StackOrUpgradeText->SetVisibility(ESlateVisibility::Collapsed);
		}

		return;
	}

	if (ItemIconImage)
	{
		if (ViewSlot.Icon.IsNull())
		{
			ItemIconImage->SetBrushFromSoftTexture(nullptr);
			ItemIconImage->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			ItemIconImage->SetVisibility(ESlateVisibility::Visible);
			ItemIconImage->SetBrushFromSoftTexture(ViewSlot.Icon);
		}
	}

	if (StackOrUpgradeText)
	{
		// 장비
		if (ViewSlot.Type == EInventoryViewSlotType::Equipment)
		{
			if (ViewSlot.UpgradeLevel > 0)
			{
				StackOrUpgradeText->SetVisibility(ESlateVisibility::Visible);
				StackOrUpgradeText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), ViewSlot.UpgradeLevel)));
			}
			else
			{
				StackOrUpgradeText->SetText(FText::GetEmpty());
				StackOrUpgradeText->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		// Potion
		else 
		{
			if (ViewSlot.PotionCount > 0)
			{
				StackOrUpgradeText->SetVisibility(ESlateVisibility::Visible);
				StackOrUpgradeText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), ViewSlot.PotionCount)));
			}
			else
			{
				StackOrUpgradeText->SetText(FText::GetEmpty());
				StackOrUpgradeText->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}
