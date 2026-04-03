#include "UI/Options/OptionItemWidget.h"
#include "UI/Options/Data/OptionDataObject.h"
#include "UI/Options/Data/OptionDataBinary.h"
#include "UI/Options/Data/OptionDataScalar.h"
#include "UI/Options/Data/OptionDataDiscrete.h"

#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"

void UOptionItemWidget::Setup(UOptionDataObject* InData)
{
	if (!InData) return;

	AssociatedData = InData;

	if (TitleText) TitleText->SetText(InData->DisplayName);

	if (OptionSlider) OptionSlider->SetVisibility(ESlateVisibility::Collapsed);
	if (OptionCheckBox) OptionCheckBox->SetVisibility(ESlateVisibility::Collapsed);
	if (OptionComboBox) OptionComboBox->SetVisibility(ESlateVisibility::Collapsed);
	if (ValueText) ValueText->SetVisibility(ESlateVisibility::Collapsed);

	if (UOptionDataScalar* ScalarData = Cast<UOptionDataScalar>(InData)) {
		if (OptionSlider) {
			OptionSlider->SetVisibility(ESlateVisibility::Visible);
			OptionSlider->SetMinValue(ScalarData->MinValue);
			OptionSlider->SetMaxValue(ScalarData->MaxValue);
			OptionSlider->SetValue(ScalarData->Value);

			OptionSlider->OnValueChanged.RemoveAll(this);
			OptionSlider->OnValueChanged.AddDynamic(this, &UOptionItemWidget::HandleSliderChanged);

			if (ValueText) {
				ValueText->SetVisibility(ESlateVisibility::Visible);
				int32 PercentValue = FMath::RoundToInt(ScalarData->Value * 100.f);
				ValueText->SetText(FText::AsNumber(PercentValue));
			}
		}
	}
	else if (UOptionDataBinary* BinaryData = Cast<UOptionDataBinary>(InData)) {
		if (OptionCheckBox) {
			OptionCheckBox->SetVisibility(ESlateVisibility::Visible);
			OptionCheckBox->SetIsChecked(BinaryData->bValue);

			OptionCheckBox->OnCheckStateChanged.RemoveAll(this);
			OptionCheckBox->OnCheckStateChanged.AddDynamic(this, &UOptionItemWidget::HandleCheckChanged);
		}
	}
	else if (UOptionDataDiscrete* EnumData = Cast<UOptionDataDiscrete>(InData)) {
		if (OptionComboBox) {
			OptionComboBox->SetVisibility(ESlateVisibility::Visible);
			OptionComboBox->ClearOptions();

			for (const FText& Opt : EnumData->Options) {
				OptionComboBox->AddOption(Opt.ToString());
			}

			if (EnumData->Options.IsValidIndex(EnumData->Index)) {
				OptionComboBox->SetSelectedIndex(EnumData->Index);
			}

			OptionComboBox->OnSelectionChanged.RemoveAll(this);
			OptionComboBox->OnSelectionChanged.AddDynamic(this, &UOptionItemWidget::HandleSelectionChanged);
		}
	}
}

void UOptionItemWidget::HandleSliderChanged(float NewValue)
{
	if (AssociatedData) AssociatedData->UpdateValue(NewValue);
	if (ValueText) {
		int32 PercentValue = FMath::RoundToInt(NewValue * 100.f);
		ValueText->SetText(FText::AsNumber(PercentValue));
	}
}
void UOptionItemWidget::HandleCheckChanged(bool bIsChecked)
{
	if (AssociatedData) AssociatedData->UpdateValue(bIsChecked);
}
void UOptionItemWidget::HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (AssociatedData && OptionComboBox) AssociatedData->UpdateValue(OptionComboBox->GetSelectedIndex());
}
