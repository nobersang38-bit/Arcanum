#include "UI/Common/CommonOptionWindow.h"
#include "UI/Common/CommonTabList.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "Core/Settings/GameOptionSettings.h"

#include "Components/Button.h"

void UCommonOptionWindow::NativeConstruct()
{
    Super::NativeConstruct();

    if (CloseBtn) {
        CloseBtn->OnClicked.RemoveDynamic(this, &UCommonOptionWindow::OnCloseButtonClicked);
        CloseBtn->OnClicked.AddDynamic(this, &UCommonOptionWindow::OnCloseButtonClicked);
    }
    if (SaveBtn) {
        SaveBtn->OnClicked.RemoveDynamic(this, &UCommonOptionWindow::OnSaveButtonClicked);
        SaveBtn->OnClicked.AddDynamic(this, &UCommonOptionWindow::OnSaveButtonClicked);
    }
    if (TabNavigation) TabNavigation->InitializeTabs();
}
void UCommonOptionWindow::OnCloseButtonClicked()
{
    RemoveFromParent();
}
void UCommonOptionWindow::OnSaveButtonClicked()
{
    UGameOptionSettings* Settings = UGameOptionSettings::Get();
    if (!Settings) return;

    Settings->ApplySettings(true);
    Settings->SaveSettings();

    UE_LOG(LogTemp, Log, TEXT("Game Settings Saved!"));
}