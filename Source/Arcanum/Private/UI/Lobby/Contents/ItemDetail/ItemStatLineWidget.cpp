#include "UI/Lobby/Contents/ItemDetail/ItemStatLineWidget.h"
#include "Components/TextBlock.h"

void UItemStatLineWidget::SetStatLineData(const FItemStatLineViewData& InStatLineData)
{
	if (StatNameText)
	{
		StatNameText->SetText(InStatLineData.StatNameText);
	}

	if (StatValueText)
	{
		StatValueText->SetText(InStatLineData.StatValueText);
	}

	SetVisibility(InStatLineData.bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UItemStatLineWidget::ClearStatLineData()
{
	if (StatNameText)
	{
		StatNameText->SetText(FText::GetEmpty());
	}

	if (StatValueText)
	{
		StatValueText->SetText(FText::GetEmpty());
	}

	SetVisibility(ESlateVisibility::Collapsed);
}
