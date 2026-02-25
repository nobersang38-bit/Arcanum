// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleAllyUnitPanelWidget.h"
#include "UI/Battle/SubLayout/BattleCostBarWidget.h"
#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "Components/HorizontalBox.h"

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

void UBattleAllyUnitPanelWidget::AddUnitSlot()
{
	// 첫번째와 마지막 인덱스는 스페이서를 사용중이기에 제외 시키고 검사

	if (!UnitPanel || !UnitSlotWidgetClass) return;
	UBattleAllyUnitSlotWidget* UnitSlot = CreateWidget<UBattleAllyUnitSlotWidget>(GetOwningPlayer(), UnitSlotWidgetClass);
	UnitPanel->InsertChildAt(UnitPanel->GetChildrenCount() - 2, UnitSlot);
}

bool UBattleAllyUnitPanelWidget::RemoveUnitSlot(int32 RemoveIDX)
{
	// 첫번째와 마지막 인덱스는 스페이서를 사용중이기에 제외 시키고 검사

	if (!UnitPanel) return false;

	if (RemoveIDX == -1 && UnitPanel->GetChildrenCount() > 2)
	{
		UnitPanel->RemoveChildAt(UnitPanel->GetChildrenCount() - 2);
		return true;
	}

	int32 ResultRemoveIDX = RemoveIDX + 1;
	if (ResultRemoveIDX > 0 && ResultRemoveIDX < UnitPanel->GetChildrenCount() - 1)
	{
		UnitPanel->RemoveChildAt(ResultRemoveIDX);
		return true;
	}
	else return false;
}
