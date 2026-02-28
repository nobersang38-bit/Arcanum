// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"

void USquareSlotWidget::NativePreConstruct()
{
    if (BackgroundColor) BackgroundColor->SetBrushColor(RoundColor);
    if (IconImage && IconImg) IconImage->SetBrushFromTexture(IconImg);
}

FReply USquareSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    OnSlotClicked.Broadcast(this, SlotIndex);

    return FReply::Handled();
}
