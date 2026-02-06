#include "UI/Login/SubLayout/CommonBtnWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UCommonBtnWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    UpdateVisuals();
}

void UCommonBtnWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼 클릭 이벤트 연결
    if (MainButton)
    {
        MainButton->OnClicked.RemoveDynamic(this, &UCommonBtnWidget::HandleButtonClicked);
        MainButton->OnClicked.AddDynamic(this, &UCommonBtnWidget::HandleButtonClicked);
    }

    UpdateVisuals();
}

#if WITH_EDITOR
void UCommonBtnWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // 에디터에서 속성 변경 시 즉시 반영
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

    if (ButtonIcon) {
        bool bShowIcon = (ButtonType == EButtonType::IconOnly || ButtonType == EButtonType::TextAndIcon);
        ButtonIcon->SetVisibility(bShowIcon ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
    }

    if (ButtonIcon) {
        if (IconTexture) ButtonIcon->SetBrushFromTexture(IconTexture);
        bool bShowIcon = (ButtonType == EButtonType::IconOnly || ButtonType == EButtonType::TextAndIcon);
        ButtonIcon->SetVisibility(bShowIcon ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
    }
}

void UCommonBtnWidget::SetButtonAppearance(FText InText, UTexture2D* InIcon, EButtonType InType)
{
    DisplayText = InText;
    ButtonType = InType;
    if (ButtonIcon && InIcon)
    {
        ButtonIcon->SetBrushFromTexture(InIcon);
    }
    UpdateVisuals();
}

void UCommonBtnWidget::SetButtonText(FText InText)
{
    DisplayText = InText;
    if (ButtonLabel) ButtonLabel->SetText(DisplayText);
}

void UCommonBtnWidget::SetButtonIcon(UTexture2D* InIcon)
{
    if (ButtonIcon && InIcon)
    {
        ButtonIcon->SetBrushFromTexture(InIcon);
    }
}

void UCommonBtnWidget::HandleButtonClicked()
{
    if (OnClicked.IsBound())
    {
        OnClicked.Broadcast();
    }
}