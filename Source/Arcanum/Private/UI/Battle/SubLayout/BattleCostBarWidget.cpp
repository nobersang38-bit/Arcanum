// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleCostBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleCostBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
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
