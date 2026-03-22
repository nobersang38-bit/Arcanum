#include "UI/Lobby/Contents/Character/SetEffectPanelWidget.h"
#include "Components/TextBlock.h"

void USetEffectPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SetNameText)
	{
		DefaultSetNameColor = SetNameText->GetColorAndOpacity();
		DefaultSetNameFont = SetNameText->GetFont();
	}
	if (SetCountText)
	{
		DefaultSetCountColor = SetCountText->GetColorAndOpacity();
		DefaultSetCountFont = SetCountText->GetFont();
	}
	if (SetDescText)
	{
		DefaultSetDescColor = SetDescText->GetColorAndOpacity();
		DefaultSetDescFont = SetDescText->GetFont();
	}
}

void USetEffectPanelWidget::SetEffectText(const FSetEffectViewData& InViewData)
{
	SetVisibility(InViewData.bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (SetNameText)
	{
		SetNameText->SetText(InViewData.SetNameText);

		FSlateFontInfo fontInfo = DefaultSetNameFont;

		if (InViewData.bActivated)
		{
			SetNameText->SetColorAndOpacity(ActivatedColor);
			fontInfo.OutlineSettings.OutlineSize = 1;
			fontInfo.OutlineSettings.OutlineColor = FLinearColor::Black;
		}
		else
		{
			SetNameText->SetColorAndOpacity(DefaultSetNameColor);
			fontInfo.OutlineSettings.OutlineSize = 0;
		}

		SetNameText->SetFont(fontInfo);
	}

	if (SetCountText)
	{
		SetCountText->SetText(InViewData.SetCountText);

		FSlateFontInfo fontInfo = DefaultSetCountFont;

		if (InViewData.bActivated)
		{
			SetCountText->SetColorAndOpacity(ActivatedColor);
			fontInfo.OutlineSettings.OutlineSize = 1;
			fontInfo.OutlineSettings.OutlineColor = FLinearColor::Black;
		}
		else
		{
			SetCountText->SetColorAndOpacity(DefaultSetCountColor);
			fontInfo.OutlineSettings.OutlineSize = 0;
		}

		SetCountText->SetFont(fontInfo);
	}

	if (SetDescText)
	{
		SetDescText->SetText(InViewData.SetDescText);

		FSlateFontInfo fontInfo = DefaultSetDescFont;

		if (InViewData.bActivated)
		{
			SetDescText->SetColorAndOpacity(ActivatedColor);
			fontInfo.OutlineSettings.OutlineSize = 1;
			fontInfo.OutlineSettings.OutlineColor = FLinearColor::Black;
		}
		else
		{
			SetDescText->SetColorAndOpacity(DefaultSetDescColor);
			fontInfo.OutlineSettings.OutlineSize = 0;
		}

		SetDescText->SetFont(fontInfo);
	}
}

void USetEffectPanelWidget::ClearEffectText()
{
	SetVisibility(ESlateVisibility::Collapsed);

	if (SetNameText)
	{
		SetNameText->SetText(FText::GetEmpty());
		SetNameText->SetColorAndOpacity(DefaultSetNameColor);
		SetNameText->SetFont(DefaultSetNameFont);
	}

	if (SetCountText)
	{
		SetCountText->SetText(FText::GetEmpty());
		SetCountText->SetColorAndOpacity(DefaultSetCountColor);
		SetCountText->SetFont(DefaultSetCountFont);
	}

	if (SetDescText)
	{
		SetDescText->SetText(FText::GetEmpty());
		SetDescText->SetColorAndOpacity(DefaultSetDescColor);
		SetDescText->SetFont(DefaultSetDescFont);
	}
}
