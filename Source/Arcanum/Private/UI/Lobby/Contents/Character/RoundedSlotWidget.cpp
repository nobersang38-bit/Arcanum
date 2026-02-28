// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/RoundedSlotWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"

void URoundedSlotWidget::NativePreConstruct()
{
    if (BackgroundColor) BackgroundColor->SetBrushColor(RoundColor);
    if (IconImage && IconImg) IconImage->SetBrushFromTexture(IconImg);

    // 테스트용으로 보유 캐릭터 구분
    // 나중에 json 값으로 구분하기
    float TargetAlpha = bShowEmptySlotOverlay ? 0.8f : 0.0f;
    FLinearColor CurrentColor = SlotDimOverlay->ColorAndOpacity;
    CurrentColor.A = TargetAlpha;
    SlotDimOverlay->SetColorAndOpacity(CurrentColor);
}

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
