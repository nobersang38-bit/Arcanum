// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/Common/FloatingDamageTextWidget.h"
#include "Components/TextBlock.h"

void UFloatingDamageTextWidget::SetText(FText InText)
{
	FloatingText->SetText(InText);
}