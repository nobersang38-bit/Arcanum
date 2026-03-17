// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Object/Operation/UnitDragDropOperation.h"
#include "Components/Border.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleAllyUnitSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.RemoveDynamic(this, &UBattleAllyUnitSlotWidget::ClickUnitSlot);
	Button->OnClicked.AddDynamic(this, &UBattleAllyUnitSlotWidget::ClickUnitSlot);

	Button->OnPressed.RemoveDynamic(this, &UBattleAllyUnitSlotWidget::PressUnitSlot);
	Button->OnPressed.AddDynamic(this, &UBattleAllyUnitSlotWidget::PressUnitSlot);

	Button->OnReleased.RemoveDynamic(this, &UBattleAllyUnitSlotWidget::ReleasedUnitSlot);
	Button->OnReleased.AddDynamic(this, &UBattleAllyUnitSlotWidget::ReleasedUnitSlot);
	SetSelectSlot(false);
	/*Button->OnPressed.AddDynamic()
	Button->OnPressed.AddDynamic()*/
}

FReply UBattleAllyUnitSlotWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Error, TEXT("마우스 올라감!!"));
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
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
	// 버튼 설정
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

	// 보더 설정
	FSlateBrush SlotBorderStyle = SlotBorder->Background;
	SlotBorderStyle.SetResourceObject(InImage);

	SlotBorder->SetBrush(SlotBorderStyle);

	// 프로그래스 바 설정
	FProgressBarStyle CoolTimeProgressStyle = CoolTimeProgress->WidgetStyle;
	FSlateBrush CoolTimeProgressBrush = CoolTimeProgressStyle.FillImage;
	CoolTimeProgressBrush.SetResourceObject(InImage);
	CoolTimeProgressStyle.SetFillImage(CoolTimeProgressBrush);

	CoolTimeProgress->SetWidgetStyle(CoolTimeProgressStyle);
}

void UBattleAllyUnitSlotWidget::SetUnitTag(FGameplayTag InUnitTag)
{
	UnitTag = InUnitTag;
}

void UBattleAllyUnitSlotWidget::SetSelectSlot(bool InIsSelected)
{
	if (InIsSelected)
	{
		SlotBorder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SlotBorder->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBattleAllyUnitSlotWidget::ClickUnitSlot()
{
	OnClickUnitSlot.Broadcast(UnitTag, this);
}

void UBattleAllyUnitSlotWidget::PressUnitSlot()
{
	OnPressUnitSlot.Broadcast(UnitTag, this);
}

void UBattleAllyUnitSlotWidget::ReleasedUnitSlot()
{
	OnReleasedUnitSlot.Broadcast(UnitTag, this);
}

void UBattleAllyUnitSlotWidget::SetActivateCost(bool InIsDisable)
{
	if (InIsDisable && DisabledImage->GetVisibility() == ESlateVisibility::Hidden)
	{
		DisabledImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if(!InIsDisable && DisabledImage->GetVisibility() == ESlateVisibility::HitTestInvisible)
	{
		DisabledImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBattleAllyUnitSlotWidget::SetCoolTimeProgress(float CurrentProgress, float MaxProgress)
{
	SetProgressesVisible(true);
	CurrentProgress = FMath::Clamp(CurrentProgress, 0.0f, MaxProgress);
	if (CoolTimeProgress)
	{
		//UE_LOG(LogTemp, Error, TEXT("current : %f, Max : %f"), CurrentProgress, MaxProgress);
		CoolTimeProgress->SetPercent(CurrentProgress / MaxProgress + 0.0001f);
	}
	if (CoolTimeText)
	{
		FString Result = FString::Printf(TEXT("%d"), FMath::CeilToInt32(CurrentProgress));
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