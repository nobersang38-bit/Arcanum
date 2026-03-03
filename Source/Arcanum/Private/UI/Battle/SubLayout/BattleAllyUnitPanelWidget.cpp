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

UBattleAllyUnitSlotWidget* UBattleAllyUnitPanelWidget::AddUnitSlot(const FUnitData& UnitData)
{
	// 첫번째와 마지막 인덱스는 스페이서를 사용중이기에 제외 시키고 검사

	if (!UnitPanel || !UnitSlotWidgetClass) return nullptr;

	UBattleAllyUnitSlotWidget* UnitSlot = CreateWidget<UBattleAllyUnitSlotWidget>(this, UnitSlotWidgetClass);

	// 1. 기존 자식 위젯들과 그들의 '슬롯 설정'을 저장할 구조체/배열 준비
	struct FSlotInfo
	{
		UWidget* Widget;
		FSlateChildSize Size;
		FMargin Padding;
		EVerticalAlignment VAlign;
		EHorizontalAlignment HAlign;
	};
	TArray<FSlotInfo> CurrentChildrenInfo;

	// 2. 현재 자식들의 설정값 백업
	for (int32 i = 0; i < UnitPanel->GetChildrenCount(); ++i)
	{
		UWidget* Child = UnitPanel->GetChildAt(i);
		UHorizontalBoxSlot* HBoxSlot = Cast<UHorizontalBoxSlot>(Child->Slot);

		FSlotInfo Info;
		Info.Widget = Child;
		if (HBoxSlot)
		{
			Info.Size = HBoxSlot->GetSize();
			Info.Padding = HBoxSlot->GetPadding();
			Info.VAlign = HBoxSlot->GetVerticalAlignment();
			Info.HAlign = HBoxSlot->GetHorizontalAlignment();
		}
		CurrentChildrenInfo.Add(Info);
	}

	// 3. 새 유닛 슬롯을 위한 정보 세팅 (기본값)
	FSlotInfo NewSlotInfo;
	NewSlotInfo.Widget = UnitSlot;
	NewSlotInfo.Size.SizeRule = ESlateSizeRule::Automatic; // 필요에 따라 Fill로 변경 가능
	NewSlotInfo.Padding = FMargin(0.0f); // 새 유닛 슬롯의 여백 설정

	// 4. 원하는 위치(마지막 스페이서 앞)에 삽입
	int32 InsertIndex = CurrentChildrenInfo.Num() - 1;
	if (InsertIndex < 0) InsertIndex = 0;
	CurrentChildrenInfo.Insert(NewSlotInfo, InsertIndex);

	// 5. 패널 비우기 (이때 기존 슬롯 데이터가 파괴됨)
	UnitPanel->ClearChildren();

	// 6. 백업한 설정값들을 그대로 적용하며 다시 추가
	for (const FSlotInfo& Info : CurrentChildrenInfo)
	{
		UHorizontalBoxSlot* NewSlot = UnitPanel->AddChildToHorizontalBox(Info.Widget);

		// 기존 위젯의 설정값 복구 (새로 추가한 UnitSlot도 포함하여 설정 적용)
		if (NewSlot)
		{
			NewSlot->SetSize(Info.Size);
			NewSlot->SetPadding(Info.Padding);
			NewSlot->SetVerticalAlignment(Info.VAlign);
			NewSlot->SetHorizontalAlignment(Info.HAlign);
		}
	}
	if (UnitSlot)
	{
		UnitSlot->SetUnitInfo(UnitData.Info.InfoSetting.MeatCost, UnitData.Info.InfoSetting.Icon, UnitData.Info.InfoSetting.Tag);

		UE_LOG(LogTemp, Error, TEXT("아이콘 세팅"));
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
