// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CombatStage/BasementHealthBar.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBasementHealthBar::UpdateBasementHealthBar(float CurrentHealth, float MaxHealth)
{
	if(CurrentHealthText)
	CurrentHealthText->SetText(FText::AsNumber(FMath::RoundToInt(CurrentHealth)));

	if(MaxHealthText)
	MaxHealthText->SetText(FText::AsNumber(FMath::RoundToInt(MaxHealth)));
	
	if (BasementHealthBar)
	{
		BasementHealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}
