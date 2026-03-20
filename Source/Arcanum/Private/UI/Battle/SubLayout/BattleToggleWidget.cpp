// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleToggleWidget.h"
#include "Components/CheckBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"

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
	TextSwitcher->SetActiveWidgetIndex(static_cast<int32>(IsChecked));
	if (IsChecked)
	{
		OnIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Glow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		if (Glow && GlowAnimation)
		{
			if (!IsAnimationPlaying(GlowAnimation))
			{
				PlayAnimation(GlowAnimation, 0.f, 0);
			}
		}
	}
	else
	{
		OnIcon->SetVisibility(ESlateVisibility::Collapsed);
		Glow->SetVisibility(ESlateVisibility::Collapsed);
		if (GlowAnimation)
		{
			if (IsAnimationPlaying(GlowAnimation))
			{
				StopAnimation(GlowAnimation);
			}
		}
	}
}
