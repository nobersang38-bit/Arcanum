#include "UI/Login/SubLayout/QuitGameUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include <Kismet/KismetSystemLibrary.h>

void UQuitGameUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (Btn) Btn->OnClicked.AddUniqueDynamic(this, &UQuitGameUserWidget::OnQuitButtonClicked);
}
void UQuitGameUserWidget::SetQuitMessage(const FString& Reason)
{
    if (Msg) Msg->SetText(FText::FromString(Reason));
}
void UQuitGameUserWidget::OnQuitButtonClicked()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC) {
        UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
    }
}