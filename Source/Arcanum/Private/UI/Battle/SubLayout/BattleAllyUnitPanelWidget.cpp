// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleAllyUnitPanelWidget.h"
#include "UI/Battle/SubLayout/BattleCostBarWidget.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleAllyUnitPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBattleAllyUnitPanelWidget::SetMeatCostProgress(float CurrentMeat, float MaxMeat)
{
	MeatCostBar->SetCostProgress(CurrentMeat, MaxMeat);
}

void UBattleAllyUnitPanelWidget::SetManaCostProgress(float CurrentMana, float MaxMana)
{
	ManaCostBar->SetCostProgress(CurrentMana, MaxMana);
}
