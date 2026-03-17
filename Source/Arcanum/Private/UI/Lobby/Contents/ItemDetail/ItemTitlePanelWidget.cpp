#include "UI/Lobby/Contents/ItemDetail/ItemTitlePanelWidget.h"
#include "Components/TextBlock.h"

void UItemTitlePanelWidget::SetItemNameText(const FText& InItemNameText)
{
	if (ItemNameText)
	{
		ItemNameText->SetText(InItemNameText);
	}
}

void UItemTitlePanelWidget::SetUpgradeLevelText(const FText& InUpgradeLevelText)
{
	if (UpgradeLevelText)
	{
		UpgradeLevelText->SetText(InUpgradeLevelText);
		UpgradeLevelText->SetVisibility(
			InUpgradeLevelText.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible
		);
	}
}

void UItemTitlePanelWidget::ClearTitleData()
{
	if (ItemNameText)
	{
		ItemNameText->SetText(FText::GetEmpty());
	}

	if (UpgradeLevelText)
	{
		UpgradeLevelText->SetText(FText::GetEmpty());
		UpgradeLevelText->SetVisibility(ESlateVisibility::Collapsed);
	}
}