// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleToggleWidget.h"
#include "Components/CheckBox.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleToggleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Toggle->SetCheckedState(ECheckBoxState::Unchecked);
	Toggle->OnCheckStateChanged.AddDynamic(this, &UBattleToggleWidget::OnChangeToggle);
}

bool UBattleToggleWidget::IsAutoPlay()
{
	if (!Toggle) return false;
	return Toggle->IsChecked();
}

void UBattleToggleWidget::OnChangeToggle(bool IsChecked)
{
	OnToggle.Broadcast(IsChecked);
}
