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

void UShopItemSlotWidget::SetSlotData(
	int32 InSlotIndex, FName InRowName,
	TSoftObjectPtr<UTexture2D> InIcon,
	const FText& InName,
	const FText& InDesc,
	int64 InPrice,
	bool InSoldOut)
{
	if (!InRowName.IsNone())
	{
		SlotIndex = InSlotIndex;
		RowName = InRowName;

		ViewIcon = InIcon;
		ViewName = InName;
		ViewDesc = InDesc;
		ViewPrice = InPrice;

		bEmpty = false;
		bSoldOut = InSoldOut;
		bSelected = false;

		RefreshSlotUI();
	}
	else
	{
		ClearSlot();
	}
}

void UShopItemSlotWidget::ClearSlot()
{
	SlotIndex = INDEX_NONE;
	RowName = NAME_None;

	ViewIcon = nullptr;
	ViewName = FText::GetEmpty();
	ViewDesc = FText::GetEmpty();
	ViewPrice = 0;

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
		ItemNameText->SetText(bEmpty ? FText::GetEmpty() : ViewName);
		ItemNameText->SetRenderOpacity(opacity);
	}

	// 설명
	if (DescText)
	{
		DescText->SetText(bEmpty ? FText::GetEmpty() : ViewDesc);
		DescText->SetRenderOpacity(opacity);
	}

	// 가격
	if (PriceText)
	{
		if (!bEmpty)
		{
			PriceText->SetText(FText::AsNumber(ViewPrice));
		}
		else
		{
			PriceText->SetText(FText::GetEmpty());
		}

		PriceText->SetRenderOpacity(opacity);
	}

	// 아이콘
	if (ItemIconImage)
	{
		if (bEmpty || ViewIcon.IsNull())
		{
			ItemIconImage->SetVisibility(ESlateVisibility::Collapsed);
			ItemIconImage->SetBrushFromSoftTexture(nullptr);
		}
		else
		{
			ItemIconImage->SetVisibility(ESlateVisibility::Visible);
			ItemIconImage->SetBrushFromSoftTexture(ViewIcon);
			ItemIconImage->SetRenderOpacity(opacity);
		}
	}
}
