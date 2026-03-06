// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleAllyUnitPanelWidget.h"
#include "UI/Battle/SubLayout/BattleCostBarWidget.h"
#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleAllyUnitPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
#if WITH_EDITOR
void UBattleAllyUnitPanelWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UBattleAllyUnitPanelWidget, bUseDebugAddUnitSlot))
	{
		if (bUseDebugAddUnitSlot)
		{
			bUseDebugAddUnitSlot = false;
			//AddUnitSlot();
		}
	}
}
#endif
void UBattleAllyUnitPanelWidget::SetMeatCostProgress(float CurrentMeat, float MaxMeat)
{
	MeatCostBar->SetCostProgress(CurrentMeat, MaxMeat);
}

void UBattleAllyUnitPanelWidget::SetManaCostProgress(float CurrentMana, float MaxMana)
{
	ManaCostBar->SetCostProgress(CurrentMana, MaxMana);
}

UBattleAllyUnitSlotWidget* UBattleAllyUnitPanelWidget::AddUnitSlot(const FUnitInfoSetting& UnitData)
{
	if (!UnitPanel || !UnitSlotWidgetClass) return nullptr;

	// 1. 마지막 자식(오른쪽 스페이서)을 미리 찾아둡니다.
	int32 ChildCount = UnitPanel->GetChildrenCount();
	UWidget* RightSpacer = (ChildCount > 0) ? UnitPanel->GetChildAt(ChildCount - 1) : nullptr;

	// 2. 마지막 스페이서가 있다면 잠시 패널에서 떼어냅니다. (전체 삭제 X)
	if (RightSpacer)
	{
		UnitPanel->RemoveChild(RightSpacer);
	}

	// 3. 새로운 유닛 슬롯 생성 및 추가 (이 시점에서는 마지막 위치에 붙음)
	UBattleAllyUnitSlotWidget* UnitSlot = CreateWidget<UBattleAllyUnitSlotWidget>(this, UnitSlotWidgetClass);
	if (UnitSlot)
	{
		UHorizontalBoxSlot* NewSlot = UnitPanel->AddChildToHorizontalBox(UnitSlot);
		if (NewSlot)
		{
			NewSlot->SetSize(ESlateSizeRule::Automatic);
			NewSlot->SetHorizontalAlignment(HAlign_Fill);
			NewSlot->SetVerticalAlignment(VAlign_Fill);
		}
		UnitSlot->SetUnitInfo(UnitData.MeatCost, UnitData.Icon, UnitData.Tag);
	}

	// 4. 아까 떼어냈던 마지막 스페이서를 다시 추가합니다. (이제 유닛 슬롯 오른쪽으로 감)
	if (RightSpacer)
	{
		UHorizontalBoxSlot* ReAddedSpacerSlot = UnitPanel->AddChildToHorizontalBox(RightSpacer);
		// 필요하다면 여기서 스페이서의 슬롯 설정(Size 등)을 다시 해줍니다.
		if (ReAddedSpacerSlot)
		{
			ReAddedSpacerSlot->SetSize(ESlateSizeRule::Fill); // 스페이서라면 보통 Fill
		}
	}

	return UnitSlot;
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

TArray<UBattleAllyUnitSlotWidget*> UBattleAllyUnitPanelWidget::GetUnitSlots() const
{
	TArray<UBattleAllyUnitSlotWidget*> ResultUnitSlots;
	if (!UnitPanel) return ResultUnitSlots;

	for (int i = 0; i < UnitPanel->GetChildrenCount(); i++)
	{
		if (UBattleAllyUnitSlotWidget* UnitSlot = Cast<UBattleAllyUnitSlotWidget>(UnitPanel->GetChildAt(i)))
		{
			ResultUnitSlots.Add(UnitSlot);
		}
	}
	return ResultUnitSlots;
}
