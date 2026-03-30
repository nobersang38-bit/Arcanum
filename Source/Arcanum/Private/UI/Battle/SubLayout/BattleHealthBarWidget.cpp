// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleHealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBattleHealthBarWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (SubIcon1)
	{
		FSlateBrush SlatBrush = SubIcon1->GetBrush();
		SlatBrush.SetResourceObject(EditSubIcon1Texture);
		SlatBrush.TintColor = FSlateColor(EditSubIcon1Color);
		SubIcon1->SetBrush(SlatBrush);
	}
	if (SubIcon2)
	{
		FSlateBrush SlatBrush = SubIcon2->GetBrush();
		SlatBrush.SetResourceObject(EditSubIcon2Texture);
		SlatBrush.TintColor = FSlateColor(EditSubIcon2Color);
		SubIcon2->SetBrush(SlatBrush);
	}
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

	if (CurrentHealth <= 0.0f)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else if (GetVisibility() == ESlateVisibility::Hidden)
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}
