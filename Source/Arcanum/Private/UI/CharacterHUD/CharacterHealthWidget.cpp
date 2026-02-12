// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterHUD/CharacterHealthWidget.h"
#include "Components/ProgressBar.h"

void UCharacterHealthWidget::SetPercent(const FRegenStat& InHealthData)
{
	HealthProgress->SetPercent(InHealthData.Current/InHealthData.GetTotalMax());
}
