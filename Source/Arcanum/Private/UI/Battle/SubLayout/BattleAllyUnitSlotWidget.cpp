// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Blueprint/DragDropOperation.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleAllyUnitSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &UBattleAllyUnitSlotWidget::ClickUnitSlot);
	Button->OnPressed.AddDynamic(this, &UBattleAllyUnitSlotWidget::PressUnitSlot);
	Button->OnReleased.AddDynamic(this, &UBattleAllyUnitSlotWidget::ReleasedUnitSlot);

	/*Button->OnPressed.AddDynamic()
	Button->OnPressed.AddDynamic()*/
}

#if WITH_EDITOR
void UBattleAllyUnitSlotWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UBattleAllyUnitSlotWidget, bIsDebugCoolTimeStart))
	{
		if (bIsDebugCoolTimeStart)
		{
			bIsDebugCoolTimeStart = false;
			SetCoolTimeProgress(DebugCoolTime, DebugMaxCoolTime);
		}
	}

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UBattleAllyUnitSlotWidget, bIsDebugCostDisableImage))
	{
		if (bIsDebugCostDisableImage)
		{
			bIsDebugCostDisableImage = false;
			SetActivateCost(bIsSetDebugCostDisableImage);
		}
	}
}
#endif

void UBattleAllyUnitSlotWidget::SetUnitInfo(int32 InCost, UTexture2D* InImage, FGameplayTag InUnitTag)
{
	SetCost(InCost);
	SetImage(InImage);
	SetUnitTag(InUnitTag);
}

void UBattleAllyUnitSlotWidget::SetCost(int32 InCost)
{
	FString Result = FString::Printf(TEXT("%d"), InCost);
	CostText->SetText(FText::FromString(Result));
}

void UBattleAllyUnitSlotWidget::SetImage(UTexture2D* InImage)
{
	FButtonStyle ButtonStyle = Button->GetStyle();

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
	Button->SetStyle(ButtonStyle);
}

void UBattleAllyUnitSlotWidget::SetUnitTag(FGameplayTag InUnitTag)
{
	UnitTag = InUnitTag;
}

void UBattleAllyUnitSlotWidget::ClickUnitSlot()
{
	OnClickUnitSlot.Broadcast(UnitTag);
}

void UBattleAllyUnitSlotWidget::PressUnitSlot()
{
	OnPressUnitSlot.Broadcast(UnitTag);
}

void UBattleAllyUnitSlotWidget::ReleasedUnitSlot()
{
	OnReleasedUnitSlot.Broadcast(UnitTag);
}

void UBattleAllyUnitSlotWidget::SetActivateCost(bool InIsDisable)
{
	if (InIsDisable)
	{
		DisabledImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else
	{
		DisabledImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBattleAllyUnitSlotWidget::SetCoolTimeProgress(float CurrentProgress, float MaxProgress)
{
	SetProgressesVisible(true);
	if (CoolTimeProgress)
	{
		CoolTimeProgress->SetPercent(CurrentProgress / MaxProgress);
	}
	if (CoolTimeText)
	{
		FString Result = FString::Printf(TEXT("%d"), FMath::RoundToInt(CurrentProgress));
		CoolTimeText->SetText(FText::FromString(Result));
	}

	if (CurrentProgress <= 0.0f)
	{
		SetProgressesVisible(false);
	}
}

void UBattleAllyUnitSlotWidget::SetProgressesVisible(bool IsVisible)
{
	if (IsVisible)
	{
		if (CoolTimeProgress && CoolTimeProgress->GetVisibility() != ESlateVisibility::HitTestInvisible)
		{
			CoolTimeProgress->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		if (CoolTimeText && CoolTimeText->GetVisibility() != ESlateVisibility::HitTestInvisible)
		{
			CoolTimeText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	}
	else
	{
		if (CoolTimeProgress && CoolTimeProgress->GetVisibility() != ESlateVisibility::Hidden)
		{
			CoolTimeProgress->SetVisibility(ESlateVisibility::Hidden);
		}
		if (CoolTimeText && CoolTimeText->GetVisibility() != ESlateVisibility::Hidden)
		{
			CoolTimeText->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}