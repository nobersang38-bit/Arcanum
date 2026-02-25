// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleHealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBattleHealthBarWidget::SetHealthProgress(float CurrentHealth, float MaxHealth)
{
	if (HealthProgress)
	{
		HealthProgress->SetPercent(CurrentHealth / MaxHealth);
	}
	if (HealthText)
	{
		FString ResultString = FString::Printf(TEXT("%d/%d"), FMath::RoundToInt(CurrentHealth), FMath::RoundToInt(MaxHealth));
		HealthText->SetText(FText::FromString(ResultString));
	}
}
