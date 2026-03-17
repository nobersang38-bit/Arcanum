#include "UI/Lobby/Contents/ItemDetail/ItemTooltipWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemTitlePanelWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemStatPanelWidget.h"
#include "Components/TextBlock.h"

void UItemTooltipWidget::SetTooltipData(const FItemDisplayViewData& InViewData)
{
	if (ItemTitlePanelWidget)
	{
		ItemTitlePanelWidget->SetItemNameText(InViewData.ItemNameText);
		ItemTitlePanelWidget->SetUpgradeLevelText(InViewData.UpgradeLevelText);
	}

	if (CurrentStatPanelWidget)
	{
		CurrentStatPanelWidget->SetStatLines(InViewData.StatLines);
	}

	if (DescText)
	{
		DescText->SetText(InViewData.DescText);
	}

	if (SellPriceText)
	{
		SellPriceText->SetText(InViewData.SellPriceText);
	}
}

void UItemTooltipWidget::ClearTooltipData()
{
	if (ItemTitlePanelWidget)
	{
		ItemTitlePanelWidget->ClearTitleData();
	}

	if (CurrentStatPanelWidget)
	{
		CurrentStatPanelWidget->ClearStatLines();
	}

	if (DescText)
	{
		DescText->SetText(FText::GetEmpty());
	}

	if (SellPriceText)
	{
		SellPriceText->SetText(FText::GetEmpty());
	}
}

void UItemTooltipWidget::ApplyDisplayData(const FItemDisplayViewData& InViewData)
{
	if (ItemTitlePanelWidget)
	{
		ItemTitlePanelWidget->SetItemNameText(InViewData.ItemNameText);
		ItemTitlePanelWidget->SetUpgradeLevelText(InViewData.UpgradeLevelText);
	}

	if (DescText)
	{
		DescText->SetText(InViewData.DescText);
	}

	if (CurrentStatPanelWidget)
	{
		CurrentStatPanelWidget->SetStatLines(InViewData.StatLines);
	}

	if (SellPriceText)
	{
		SellPriceText->SetText(InViewData.SellPriceText);
	}

	if (EquippedCharacterText)
	{
		EquippedCharacterText->SetText(InViewData.EquippedCharacterText);
		EquippedCharacterText->SetVisibility(InViewData.EquippedCharacterText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible
		);
	}
}
