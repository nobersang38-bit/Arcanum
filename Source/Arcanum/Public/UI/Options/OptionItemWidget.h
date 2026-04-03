#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionItemWidget.generated.h"

/// 260318 추가
class UOptionDataObject;

class USlider;
class UCheckBox;
class UTextBlock;
class UComboBoxString;

UCLASS()
class ARCANUM_API UOptionItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void Setup(UOptionDataObject* InData);

protected:
	UPROPERTY(meta = (BindWidget)) TObjectPtr<USlider> OptionSlider;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCheckBox> OptionCheckBox;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UComboBoxString> OptionComboBox;
	
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> ValueText;
	
	UPROPERTY() TObjectPtr<UOptionDataObject> AssociatedData;

private:
	UFUNCTION() void HandleSliderChanged(float NewValue);
	UFUNCTION() void HandleCheckChanged(bool bIsChecked);
	UFUNCTION() void HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
};