#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemTooltipWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemDetailHelper.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USquareSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (BackgroundColor) BackgroundColor->SetBrushColor(BackColor);
	if (IconImage && IconImg) IconImage->SetBrushFromTexture(IconImg);
}

FReply USquareSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	OnSlotClicked.Broadcast(this, SlotIndex);

	return FReply::Handled();
}

void USquareSlotWidget::SetSquareBackgroundColor(FLinearColor NewColor)
{
	if (BackgroundColor)
	{
		BackgroundColor->SetBrushColor(NewColor);
	}
}

void USquareSlotWidget::SetItemIconImage(UTexture2D* ItemIcon)
{
	IconImage->SetBrushFromTexture(ItemIcon);
	IconImg = ItemIcon;
}

void USquareSlotWidget::SetItemName(FText InText)
{
	EquipNameTxt = InText;
}

void USquareSlotWidget::SetWeaponTag(FGameplayTag InTag)
{
	WeaponTag = InTag;
}

void USquareSlotWidget::SetWeaponGuid(FGuid InGuid)
{
	EquippedItemGuid = InGuid;
}

void USquareSlotWidget::SetUpgradeLevel(int32 InUpgradeLevel)
{
	if (UpgradeLevelText)
	{
		if (InUpgradeLevel > 0)
		{
			UpgradeLevelText->SetVisibility(ESlateVisibility::Visible);
			UpgradeLevelText->SetText(FText::FromString(FString::Printf(TEXT("+%d"), InUpgradeLevel)));
		}
		else
		{
			ClearUpgradeLevel();
		}
	}
}

void USquareSlotWidget::ClearUpgradeLevel()
{
	if (UpgradeLevelText)
	{
		UpgradeLevelText->SetText(FText::GetEmpty());
		UpgradeLevelText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USquareSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	RefreshTooltip();
}

void USquareSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	SetToolTip(nullptr);
}

void USquareSlotWidget::RefreshTooltip()
{

	UE_LOG(LogTemp, Warning, TEXT("Square=%s WidgetClass=%s TooltipClass=%s"), *GetNameSafe(this), *GetClass()->GetName(), *GetNameSafe(ItemTooltipWidgetClass));
	if (!EquippedItemGuid.IsValid() || !ItemTooltipWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("SquareTooltip GuidValid=%d ClassValid=%d"), EquippedItemGuid.IsValid(), ItemTooltipWidgetClass != nullptr);
		SetToolTip(nullptr);
		return;
	}

	UItemTooltipWidget* tooltipWidget = CreateWidget<UItemTooltipWidget>(this, ItemTooltipWidgetClass);
	if (!tooltipWidget)
	{
		SetToolTip(nullptr);
		return;
	}

	FItemDisplayViewData viewData;
	if (!FItemDetailHelper::BuildEquipmentDisplayViewData(this, EquippedItemGuid, viewData))
	{
		SetToolTip(nullptr);
		return;
	}

	tooltipWidget->ApplyDisplayData(viewData);
	SetToolTip(tooltipWidget);
}