// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleCostBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleCostBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBattleCostBarWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (Icon1)
	{
		FSlateBrush SlatBrush = Icon1->GetBrush();
		SlatBrush.SetResourceObject(EditIcon1Texture);
		SlatBrush.TintColor = FSlateColor(EditIcon1Color);
		Icon1->SetBrush(SlatBrush);
	}
	if (Icon2)
	{
		FSlateBrush SlatBrush = Icon2->GetBrush();
		SlatBrush.SetResourceObject(EditIcon2Texture);
		SlatBrush.TintColor = FSlateColor(EditIcon2Color);
		Icon2->SetBrush(SlatBrush);
	}
	if (CostProgress)
	{
		CostProgress->SetFillColorAndOpacity(EditProgressColor);
	}
}

void UBattleCostBarWidget::SetCostProgress(float CurrentCost, float MaxCost)
{
	if (CostProgress)
	{
		CostProgress->SetPercent(CurrentCost / MaxCost);
	}
	if (CostText)
	{
		FString ResultString = FString::Printf(TEXT("%d/%d"), FMath::RoundToInt(CurrentCost), FMath::RoundToInt(MaxCost));
		CostText->SetText(FText::FromString(ResultString));
	}
}
