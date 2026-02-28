// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame/UnitHealthWidget.h"
#include "Components/ProgressBar.h"
#include "GameplayTags/ArcanumTags.h"
#include "Components/TextBlock.h"

void UUnitHealthWidget::SetPercent(const FRegenStat& InHealthData)
{
	if (InHealthData.ParentTag == Arcanum::BattleStat::Character::Regen::Health::Root)
	{

		float CurrentHealth = InHealthData.Current;
		float TotalMaxHealth = InHealthData.GetTotalMax();
		HealthProgress->SetPercent(CurrentHealth / TotalMaxHealth);
		FString PercentText = FString::Printf(TEXT("%.0f/%.0f"), CurrentHealth, TotalMaxHealth);
		HealthText->SetText(FText::FromString(PercentText));
	}
}

void UUnitHealthWidget::SetPercent(float CurrentHealth, float MaxHealth)
{
	HealthProgress->SetPercent(CurrentHealth / MaxHealth);
	FString PercentText = FString::Printf(TEXT("%.0f/%.0f"), CurrentHealth, MaxHealth);
	HealthText->SetText(FText::FromString(PercentText));
}