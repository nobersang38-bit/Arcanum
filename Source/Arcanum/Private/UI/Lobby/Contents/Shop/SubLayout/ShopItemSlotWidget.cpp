#include "UI/Lobby/Contents/Shop/SubLayout/ShopItemSlotWidget.h"
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

void UShopItemSlotWidget::SetViewData(int32 InSlotIndex, FName InRowName, const FDTEquipmentInfoRow* InRowPtr, bool InSoldOut)
{
	if (InRowPtr && !InRowName.IsNone())
	{
		SetEquipmentData(*InRowPtr, InSlotIndex, InRowName);

		SetSoldOut(InSoldOut);
	}
	else
	{
		ClearSlot();
	}
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

	float opacity = bSoldOut ? SoldOutOpacity : 1.0f;

	if (SlotBorder)
	{
		SlotBorder->SetVisibility(!bEmpty ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
	if (SelectedBorder)
	{
		SelectedBorder->SetVisibility((!bEmpty && bSelected && !bSoldOut) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}

	// 아이템명
	if (ItemNameText)
	{
		ItemNameText->SetText(bEmpty ? FText::GetEmpty() : FText::FromName(RowName));
		ItemNameText->SetRenderOpacity(opacity);
	}

	// 설명
	if (DescText)
	{
		DescText->SetText(bEmpty ? FText::GetEmpty() : EquipmentRow.Desc);
		DescText->SetRenderOpacity(opacity);
	}

	// 가격
	if (PriceText)
	{
		PriceText->SetText(bEmpty ? FText::GetEmpty() : FText::AsNumber(EquipmentRow.BuyPrice));
		PriceText->SetRenderOpacity(opacity);
	}

	// 아이콘
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
			ItemIconImage->SetBrushFromSoftTexture(EquipmentRow.Icon.IsNull() ? nullptr : EquipmentRow.Icon);
			ItemIconImage->SetRenderOpacity(opacity);
		}
	}
}
