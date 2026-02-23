#include "UI/Lobby/Contents/Shop/ShopItemSlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UShopItemSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SlotButton)
	{
		SlotButton->OnClicked.RemoveDynamic(this, &UShopItemSlotWidget::HandleSlotButtonClicked);
		SlotButton->OnClicked.AddDynamic(this, &UShopItemSlotWidget::HandleSlotButtonClicked);
	}

	RefreshSlotUI();
}

void UShopItemSlotWidget::SetEquipmentData(const FDTEquipmentInfoRow& InRow, int32 InSlotIndex, FName InRowName)
{
	SlotIndex = InSlotIndex;
	RowName = InRowName;
	EquipmentRow = InRow;

	bEmpty = false;
	bSoldOut = false;
	bSelected = false;

	RefreshSlotUI();
}

void UShopItemSlotWidget::ClearSlot()
{
	SlotIndex = INDEX_NONE;
	RowName = NAME_None;
	EquipmentRow = FDTEquipmentInfoRow();

	bEmpty = true;
	bSoldOut = false;
	bSelected = false;

	RefreshSlotUI();
}

void UShopItemSlotWidget::SetSelected(bool InSelected)
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

void UShopItemSlotWidget::SetSoldOut(bool InSoldOut)
{
	if (!bEmpty)
	{
		bSoldOut = InSoldOut;
	}

	RefreshSlotUI();
}

bool UShopItemSlotWidget::IsPurchasable() const
{
	return (!bEmpty && !bSoldOut && RowName != NAME_None);
}

void UShopItemSlotWidget::HandleSlotButtonClicked()
{
	if (IsPurchasable())
	{
		OnShopItemSlotClicked.Broadcast(SlotIndex);
	}
}

void UShopItemSlotWidget::RefreshSlotUI()
{
	if (SlotButton)
	{
		SlotButton->SetIsEnabled(IsPurchasable());
	}

	if (SelectedBorder)
	{
		SelectedBorder->SetVisibility((!bEmpty && bSelected) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (SoldOutBorder)
	{
		SoldOutBorder->SetVisibility((!bEmpty && bSoldOut) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	if (ItemNameText)
	{
		if (bEmpty)
		{
			ItemNameText->SetText(FText::GetEmpty());
		}
		else
		{
			ItemNameText->SetText(FText::FromName(RowName));
		}
	}

	if (DescText)
	{
		if (bEmpty)
		{
			DescText->SetText(FText::GetEmpty());
		}
		else
		{
			DescText->SetText(EquipmentRow.Desc);
		}
	}

	if (PriceText)
	{
		if (bEmpty)
		{
			PriceText->SetText(FText::GetEmpty());
		}
		else
		{
			PriceText->SetText(FText::AsNumber(EquipmentRow.BuyPrice));
		}
	}

	if (ItemIconImage)
	{
		if (bEmpty)
		{
			ItemIconImage->SetVisibility(ESlateVisibility::Collapsed);
			ItemIconImage->SetBrushFromSoftTexture(nullptr);
		}
		else
		{
			ItemIconImage->SetVisibility(ESlateVisibility::Visible);

			if (EquipmentRow.Icon.IsNull())
			{
				ItemIconImage->SetBrushFromSoftTexture(nullptr);
			}
			else
			{
				ItemIconImage->SetBrushFromSoftTexture(EquipmentRow.Icon);
			}
		}
	}
}
