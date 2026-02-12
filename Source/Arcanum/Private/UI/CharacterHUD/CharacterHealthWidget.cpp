// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterHUD/CharacterHealthWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameplayTags/ArcanumTags.h"

void UCharacterHealthWidget::SetPercent(const FRegenStat& InHealthData)
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

void UCharacterHealthWidget::SetPercent(float CurrentHealth, float MaxHealth)
{
	HealthProgress->SetPercent(CurrentHealth / MaxHealth);
	FString PercentText = FString::Printf(TEXT("%.0f/%.0f"), CurrentHealth, MaxHealth);
	HealthText->SetText(FText::FromString(PercentText));
}
