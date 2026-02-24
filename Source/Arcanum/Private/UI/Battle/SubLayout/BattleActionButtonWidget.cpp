// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleActionButtonWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleActionButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ActionButton)
	{
		ActionButton->OnClicked.AddDynamic(this, &UBattleActionButtonWidget::OnActionButtonClick);
	}
}

#if WITH_EDITOR
void UBattleActionButtonWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	{
		ActionText->SetText(IconText);
	}
}

void UBattleActionButtonWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UBattleActionButtonWidget, bIsDebugCoolTimeStart))
	{
		if (bIsDebugCoolTimeStart)
		{
			bIsDebugCoolTimeStart = false;
			SetProgress(DebugCoolTime, DebugMaxCoolTime);
		}
	}
}
#endif

void UBattleActionButtonWidget::SetProgress(float CurrentProgress, float MaxProgress)
{
	SetProgressesVisible(true);
	if (CoolTimeProgress)
	{
		CoolTimeProgress->SetPercent(CurrentProgress / MaxProgress);
	}
	if(ActionText)
	{
		FString Result = FString::Printf(TEXT("%d"), FMath::RoundToInt(CurrentProgress));
		ActionText->SetText(FText::FromString(Result));
	}

	if (CurrentProgress <= 0.0f)
	{
		SetProgressesVisible(false);
		ActionText->SetText(IconText);
	}
}

void UBattleActionButtonWidget::SetImage(UTexture2D* InImage)
{
	FButtonStyle ButtonStyle = ActionButton->GetStyle();

	FSlateBrush NormaSlateBrush = ButtonStyle.Normal;
	NormaSlateBrush.SetResourceObject(InImage);

	FSlateBrush HoveredSlateBrush = ButtonStyle.Hovered;
	HoveredSlateBrush.SetResourceObject(InImage);

	FSlateBrush PressedSlateBrush = ButtonStyle.Pressed;
	PressedSlateBrush.SetResourceObject(InImage);

	FSlateBrush DisabledSlateBrush = ButtonStyle.Disabled;
	DisabledSlateBrush.SetResourceObject(InImage);

	ButtonStyle.SetNormal(NormaSlateBrush);
	ButtonStyle.SetHovered(HoveredSlateBrush);
	ButtonStyle.SetPressed(PressedSlateBrush);
	ButtonStyle.SetDisabled(DisabledSlateBrush);
	ActionButton->SetStyle(ButtonStyle);
}

void UBattleActionButtonWidget::SetProgressesVisible(bool IsVisible)
{
	if (IsVisible)
	{
		if (CoolTimeProgress && CoolTimeProgress->GetVisibility() != ESlateVisibility::HitTestInvisible)
		{
			CoolTimeProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		if (CoolTimeProgress && CoolTimeProgress->GetVisibility() != ESlateVisibility::Hidden)
		{
			CoolTimeProgress->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBattleActionButtonWidget::OnActionButtonClick()
{
	OnButtonClick.Broadcast();
}
