#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
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
	WeaponGuid = InGuid;
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