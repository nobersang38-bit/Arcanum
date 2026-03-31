#include "UI/Lobby/Contents/Inventory/SubLayout/InventoryItemSlotWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemTooltipWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemDetailHelper.h"
#include "Core/ARPlayerAccountService.h"
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

	if (EquippedMarkImage)
	{
		EquippedMarkImage->SetVisibility(ESlateVisibility::Collapsed);
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
	OnInventorySlotClicked.Broadcast(SlotIndex);
}

void UInventoryItemSlotWidget::RefreshSlotUI()
{
	const bool bIsEmpty = (ViewSlot.Type == EInventoryViewSlotType::Empty);

	if (SlotButton)
	{
		SlotButton->SetIsEnabled(true);
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

        if (EquippedMarkImage)
        {
			EquippedMarkImage->SetVisibility(ESlateVisibility::Collapsed);
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
			
			if (EquippedMarkImage)
			{
				SetEquippedMarkVisible(FPlayerAccountService::IsItemEquipped(this, ViewSlot.ItemGuid));
			}
		}
		// Potion
		else
		{
			if (ViewSlot.StackCount > 0) {
				StackOrUpgradeText->SetVisibility(ESlateVisibility::Visible);
				StackOrUpgradeText->SetText(FText::FromString(FString::Printf(TEXT("x%d"), ViewSlot.StackCount)));
			}
			else
			{
				StackOrUpgradeText->SetText(FText::GetEmpty());
				StackOrUpgradeText->SetVisibility(ESlateVisibility::Collapsed);
			}

			if (EquippedMarkImage)
			{
				EquippedMarkImage->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

void UInventoryItemSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	RefreshTooltip();
}

void UInventoryItemSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetToolTip(nullptr);
}

void UInventoryItemSlotWidget::RefreshTooltip()
{
	if (ViewSlot.Type == EInventoryViewSlotType::Empty)
	{
		SetToolTip(nullptr);
		return;
	}

	if (!ItemTooltipWidgetClass)
	{
		SetToolTip(nullptr);
		return;
	}

	UItemTooltipWidget* tooltipWidget = CreateWidget<UItemTooltipWidget>(this, ItemTooltipWidgetClass);
	if (!tooltipWidget)
	{
		SetToolTip(nullptr);
		return;
	}

	FItemDisplayViewData viewData;

	if (ViewSlot.Type == EInventoryViewSlotType::Equipment)
	{
		if (!FItemDetailHelper::BuildEquipmentDisplayViewData(this, ViewSlot.ItemGuid, viewData))
		{
			SetToolTip(nullptr);
			return;
		}
	}
	else if (ViewSlot.Type == EInventoryViewSlotType::StackItem)
	{
		if (!FItemDetailHelper::BuildStackItemDisplayViewData(this, ViewSlot.ItemTag, viewData))
		{
			SetToolTip(nullptr);
			return;
		}
	}
	else
	{
		SetToolTip(nullptr);
		return;
	}

	tooltipWidget->ApplyDisplayData(viewData);
	SetToolTip(tooltipWidget);
}

void UInventoryItemSlotWidget::SetEquippedMarkVisible(bool bVisible)
{
	if (EquippedMarkImage)
	{
		EquippedMarkImage->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}
