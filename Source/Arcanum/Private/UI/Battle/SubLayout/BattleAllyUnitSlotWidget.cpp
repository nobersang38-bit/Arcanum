// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Object/Operation/UnitDragDropOperation.h"

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
FReply UBattleAllyUnitSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 좌클릭일 경우 드래그 감지 시작
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// 블루프린트의 'Detect Drag if Pressed' 와 동일한 역할
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("클릭 유닛 슬롯"));
		FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		return Reply.NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
void UBattleAllyUnitSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	// 1. Operation 객체 생성
	UUnitDragDropOperation* DragOp = NewObject<UUnitDragDropOperation>();
	DragOp->UnitTag = UnitTag;
	DragOp->Slot = this;
	// 2. 마우스를 따라다닐 Drag Visual 생성 (설정되어 있다면)
	/*if (DragVisualClass)
	{
		UUserWidget* DragVisual = CreateWidget<UUserWidget>(GetWorld(), DragVisualClass);
		DragOp->DefaultDragVisual = DragVisual;
	}*/

	// 3. OutOperation에 할당하여 엔진에 전달
	OnPressUnitSlot.Broadcast(UnitTag);
	OutOperation = DragOp;
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
		CoolTimeProgress->SetPercent(CurrentProgress / MaxProgress + 0.00001f);
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