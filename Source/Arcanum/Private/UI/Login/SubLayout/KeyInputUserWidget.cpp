#include "UI/Login/SubLayout/KeyInputUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UKeyInputUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    bInputReceived = false;
    if (AnyKeyButton) {
        AnyKeyButton->OnClicked.RemoveDynamic(this, &UKeyInputUserWidget::OnButtonClicked);
        AnyKeyButton->OnClicked.AddDynamic(this, &UKeyInputUserWidget::OnButtonClicked);
    }
    if (PressAnyKeyText) PressAnyKeyText->SetText(FText::FromString(AnyText));
}
void UKeyInputUserWidget::OnButtonClicked()
{
    if (bInputReceived) return;

    bInputReceived = true;
    if (AnyKeyButton) {
        AnyKeyButton->SetIsEnabled(false);
        AnyKeyButton->SetVisibility(ESlateVisibility::Collapsed);
    }
    PlayWidgetAnimation();
}
void UKeyInputUserWidget::PlayWidgetAnimation_Implementation()
{
}
void UKeyInputUserWidget::HandleAnyInput()
{
    if (bInputReceived) {
        OnAnyKeyPressed.Broadcast();
        if (AnyKeyButton) AnyKeyButton->SetIsEnabled(false);
    }
}