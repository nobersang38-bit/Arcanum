#include "UI/Login/SubLayout/AnnouncetUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UI/Common/CommonBtnWidget.h"

void UAnnouncetUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseBtn) {
        CloseBtn->OnClicked.RemoveDynamic(this, &UAnnouncetUserWidget::HandleCloseButtonClicked);
        CloseBtn->OnClicked.AddDynamic(this, &UAnnouncetUserWidget::HandleCloseButtonClicked);
    }
}
void UAnnouncetUserWidget::SetAnnouncementText(const FText& NewText)
{
    if (AnnouncementText) {
        AnnouncementText->SetText(NewText);
        if (ScrollBox) ScrollBox->ScrollToStart();
    }
}
void UAnnouncetUserWidget::HandleCloseButtonClicked()
{
    OnCloseClicked.Broadcast();
}