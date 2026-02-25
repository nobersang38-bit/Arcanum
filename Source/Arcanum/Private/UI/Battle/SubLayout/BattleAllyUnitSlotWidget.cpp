// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleAllyUnitSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

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

void UBattleAllyUnitSlotWidget::SetDisabled(bool InIsDisable)
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
