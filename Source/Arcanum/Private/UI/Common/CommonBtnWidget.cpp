#include "UI/Common/CommonBtnWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "CommonButtonBase.h"

void UCommonBtnWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    UpdateVisuals();
}

void UCommonBtnWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MainButton) {
        MainButton->OnClicked.RemoveDynamic(this, &UCommonBtnWidget::HandleButtonClicked);
        MainButton->OnClicked.AddDynamic(this, &UCommonBtnWidget::HandleButtonClicked);
    }
    if (IconButton) {
        IconButton->OnClicked.RemoveDynamic(this, &UCommonBtnWidget::HandleButtonClicked);
        IconButton->OnClicked.AddDynamic(this, &UCommonBtnWidget::HandleButtonClicked);
    }

    UpdateVisuals();
}

#if WITH_EDITOR
void UCommonBtnWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    UpdateVisuals();
}
#endif

void UCommonBtnWidget::UpdateVisuals()
{
    if (ButtonLabel) {
        ButtonLabel->SetText(DisplayText);
        bool bShowText = (ButtonType == EButtonType::TextOnly || ButtonType == EButtonType::TextAndIcon);
        ButtonLabel->SetVisibility(bShowText ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
    }

    if (IconButton) {
        bool bShowIcon = (ButtonType == EButtonType::IconOnly || ButtonType == EButtonType::TextAndIcon);
        IconButton->SetVisibility(bShowIcon ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        if (bShowIcon) ApplyButtonStyle(IconButton, NormalTexture, HoverTexture, ClickTexture);
    }
}

void UCommonBtnWidget::ApplyButtonStyle(UButton* InButton, UTexture2D* Normal, UTexture2D* Hover, UTexture2D* Click)
{
    if (!InButton || !Normal) return;

    FButtonStyle Style = InButton->GetStyle();

    Style.Normal.SetResourceObject(Normal);
    Style.Hovered.SetResourceObject(Hover ? Hover : Normal);
    Style.Pressed.SetResourceObject(Click ? Click : Normal);

    InButton->SetStyle(Style);
}

void UCommonBtnWidget::SetButtonAppearance(FText InText, UTexture2D* InIcon, EButtonType InType)
{
    DisplayText = InText;
    ButtonType = InType;

    UpdateVisuals();
}

void UCommonBtnWidget::SetButtonText(FText InText)
{
    DisplayText = InText;
    if (ButtonLabel) ButtonLabel->SetText(DisplayText);
}

void UCommonBtnWidget::SetSelectedState_Implementation(bool bIsSelected)
{
    bIsSelectedState = bIsSelected;

    if (HQUI_Button)
    {
        HQUI_Button->SetIsSelected(bIsSelected);
    }
}

void UCommonBtnWidget::HandleButtonClicked()
{
    if (OnClicked.IsBound())
    {
        OnClicked.Broadcast();
    }
}