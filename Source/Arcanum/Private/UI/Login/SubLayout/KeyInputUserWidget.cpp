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
    if (PressAnyKeyText) PressAnyKeyText->SetText(FText::FromString(TEXT("Press Any Key")));
}
void UKeyInputUserWidget::OnButtonClicked()
{
	HandleAnyInput();
}
void UKeyInputUserWidget::HandleAnyInput()
{
    if (!bInputReceived) {
        bInputReceived = true;
        OnAnyKeyPressed.Broadcast();
    }
}
FReply UKeyInputUserWidget::NativeOnKeyDown(const FGeometry& InGeometry,const FKeyEvent& InKeyEvent)
{
    HandleAnyInput();
    return FReply::Handled();
}