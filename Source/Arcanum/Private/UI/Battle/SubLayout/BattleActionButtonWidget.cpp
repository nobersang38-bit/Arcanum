// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleActionButtonWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleActionButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ActionButton)
	{
		ActionButton->OnClicked.AddDynamic(this, &UBattleActionButtonWidget::OnActionButtonClick);

		ActionButton->OnPressed.AddDynamic(this, &UBattleActionButtonWidget::OnActionButtonPressed);
		ActionButton->OnReleased.AddDynamic(this, &UBattleActionButtonWidget::OnActionButtonReleased);
	}
	if (ActionText)
	{
		ActionText->SetText(IconText);
		//UE_LOG(LogTemp, Warning, TEXT("작동!!!!!!!!! %s"), *IconText.ToString());
	}
	if (SkillCooldownImage)
	{
		SkillCooldownMID = SkillCooldownImage->GetDynamicMaterial();
		SkillCooldownImage->SetVisibility(ESlateVisibility::Hidden);
	}

	SetProgressesVisible(false);

	if (!bUseDisableImage)
	{
		DisabledImage->SetVisibility(ESlateVisibility::Hidden);
	}
	if (!bUseCoolTimeProgressBar)
	{
		CoolTimeProgress->SetVisibility(ESlateVisibility::Hidden);
	}
	if (StackCountText)
	{
		StackCountText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (bUseDefaultIcon)
	{
		FButtonStyle ButtonStyle = ActionButton->GetStyle();
		auto UpdateBrush = [&](FSlateBrush& Brush) 
			{
				Brush.SetResourceObject(EditLockIcon);
				Brush.DrawAs = ESlateBrushDrawType::Image; // 무기 장착 안 했을때 아이콘 투명하게
			};

		UpdateBrush(ButtonStyle.Normal);
		UpdateBrush(ButtonStyle.Hovered);
		UpdateBrush(ButtonStyle.Pressed);
		UpdateBrush(ButtonStyle.Disabled);

		ActionButton->SetStyle(ButtonStyle);
	}
}

#if WITH_EDITOR
void UBattleActionButtonWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	{
		ActionText->SetText(IconText);
		CostText->SetText(EditCostText);
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
			SetCoolTimeProgress(DebugCoolTime, DebugMaxCoolTime);
		}
	}

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UBattleActionButtonWidget, bIsDebugCostDisableImage))
	{
		if (bIsDebugCostDisableImage)
		{
			bIsDebugCostDisableImage = false;
			SetActivateCost(bIsSetDebugCostDisableImage);
		}
	}
}
#endif

void UBattleActionButtonWidget::SetActivateCost(bool InIsDisable)
{
	if (bUseDisableImage)
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

}

void UBattleActionButtonWidget::SetCoolTimeProgress(float CurrentProgress, float MaxProgress)
{
	if (bUseCoolTimeProgressBar)
	{
		SetProgressesVisible(true);
		if (CoolTimeProgress)
		{
			CoolTimeProgress->SetPercent(CurrentProgress / MaxProgress);
		}
		if (ActionText)
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
}

void UBattleActionButtonWidget::SetImage(UTexture2D* InImage)
{
	FButtonStyle ButtonStyle = ActionButton->GetStyle();
	auto UpdateBrush = [&](FSlateBrush& Brush) {
		if (InImage == nullptr) {
			Brush.SetResourceObject(EditLockIcon);
			Brush.DrawAs = ESlateBrushDrawType::Image; // 무기 장착 안 했을때 아이콘 투명하게
		}
		else {
			Brush.SetResourceObject(InImage);
			Brush.DrawAs = ESlateBrushDrawType::Image; 
		}
	};
	/*FSlateBrush NormaSlateBrush = ButtonStyle.Normal;
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
	ButtonStyle.SetDisabled(DisabledSlateBrush);*/

	UpdateBrush(ButtonStyle.Normal);
	UpdateBrush(ButtonStyle.Hovered);
	UpdateBrush(ButtonStyle.Pressed);
	UpdateBrush(ButtonStyle.Disabled);

	ActionButton->SetStyle(ButtonStyle);
}

void UBattleActionButtonWidget::SetCostText(FText InText)
{
	if (CostText)
	{
		CostText->SetText(InText);
	}
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

void UBattleActionButtonWidget::OnActionButtonPressed()
{
	OnButtonPressed.Broadcast();
}

void UBattleActionButtonWidget::OnActionButtonReleased()
{
	OnButtonReleased.Broadcast();
}

void UBattleActionButtonWidget::SetSkillCooldownPercent(float InPercent)
{
	if (SkillCooldownImage)
	{
		if (!SkillCooldownMID)
		{
			SkillCooldownMID = SkillCooldownImage->GetDynamicMaterial();
		}

		if (SkillCooldownMID)
		{
			SkillCooldownMID->SetScalarParameterValue(TEXT("CooldownPercent"), InPercent);
		}

		if (InPercent > 0.0f)
		{
			SkillCooldownImage->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SkillCooldownImage->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBattleActionButtonWidget::SetStackCount(int32 InCount)
{
	if (StackCountText)
	{
		StackCountText->SetText(FText::AsNumber(InCount));
		StackCountText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UBattleActionButtonWidget::ClearStackCount()
{
	if (StackCountText)
	{
		StackCountText->SetText(FText::GetEmpty());
		StackCountText->SetVisibility(ESlateVisibility::Hidden);
	}
}
