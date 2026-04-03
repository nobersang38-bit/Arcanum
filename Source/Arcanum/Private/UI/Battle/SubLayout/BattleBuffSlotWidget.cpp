#include "UI/Battle/SubLayout/BattleBuffSlotWidget.h"
#include "Components/Image.h"

void UBattleBuffSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BuffDuration)
	{
		BuffDurationMID = BuffDuration->GetDynamicMaterial();
	}
}

void UBattleBuffSlotWidget::SetBuffIcon(UTexture2D* InIcon)
{
	if (BuffIcon && InIcon)
	{
		BuffIcon->SetBrushFromTexture(InIcon);
	}
}

void UBattleBuffSlotWidget::SetDurationPercent(float InPercent)
{
	if (!BuffDurationMID && BuffDuration)
	{
		BuffDurationMID = BuffDuration->GetDynamicMaterial();
	}

	if (BuffDurationMID)
	{
		BuffDurationMID->SetScalarParameterValue(TEXT("CooldownPercent"), InPercent);
	}
}
