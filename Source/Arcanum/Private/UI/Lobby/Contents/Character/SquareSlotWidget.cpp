// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"

void USquareSlotWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (BackgroundColor) BackgroundColor->SetBrushColor(BackColor);
    if (IconImage && IconImg) IconImage->SetBrushFromTexture(IconImg);
}

FReply USquareSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    OnSlotClicked.Broadcast(this, SlotIndex);

    return FReply::Handled();
}

void USquareSlotWidget::SetSquareBackgroundColor(FLinearColor NewColor)
{
    if (BackgroundColor)
    {
        BackgroundColor->SetBrushColor(NewColor);
    }
}

void USquareSlotWidget::SetItemIconImage(UTexture2D* ItemIcon, bool bIsEquipped)
{
}
