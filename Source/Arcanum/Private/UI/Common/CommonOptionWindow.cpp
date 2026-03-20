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

    // 1. 현재 메모리에 로드된 각 탭 위젯으로부터 데이터 수집
// (각 탭 위젯에 GetSettingsData() 같은 함수가 있다고 가정하거나 
// TabNavigation을 통해 직접 접근)

/* 예시:
if (UCommonTabList* TabList = TabNavigation)
{
    // 오디오 탭을 찾아서 값 가져오기
    // Settings->SetOverallVolume(AudioTab->GetSliderValue());
}
*/
    Settings->ApplySettings(true);
    Settings->SaveSettings();

    UE_LOG(LogTemp, Log, TEXT("Game Settings Saved!"));
}