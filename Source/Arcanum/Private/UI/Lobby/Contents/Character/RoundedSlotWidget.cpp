// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/RoundedSlotWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"

void URoundedSlotWidget::SetRoundBackgroundColor(FLinearColor NewColor)
{
    if (BackgroundColor)
    {
        BackgroundColor->SetBrushColor(NewColor);
    }
}

void URoundedSlotWidget::SetIconImage(UTexture2D* CharacterIcon)
{
    if (!IconImage) return;

    FSlateBrush Brush;
    if (CharacterIcon)
    {
        Brush.SetResourceObject(CharacterIcon);
    }
    else
    {
        Brush.TintColor = FSlateColor(FLinearColor::Black);
    }
    IconImage->SetBrush(Brush);
}
