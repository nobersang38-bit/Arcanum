#include "UI/Lobby/Contents/ItemDetail/ItemStatPanelWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemStatLineWidget.h"
#include "Components/VerticalBox.h"

void UItemStatPanelWidget::SetStatLines(const TArray<FItemStatLineViewData>& InStatLines)
{
	ClearStatLines();

	if (StatLineBox)
	{
		if (ItemStatLineWidgetClass)
		{
			for (const FItemStatLineViewData& statLine : InStatLines)
			{
				if (statLine.bVisible)
				{
					if (UItemStatLineWidget* statLineWidget = CreateWidget<UItemStatLineWidget>(this, ItemStatLineWidgetClass))
					{
						statLineWidget->SetStatLineData(statLine);
						StatLineBox->AddChildToVerticalBox(statLineWidget);
					}
				}
			}
		}
	}
}

void UItemStatPanelWidget::ClearStatLines()
{
	if (StatLineBox)
	{
		StatLineBox->ClearChildren();
	}
}