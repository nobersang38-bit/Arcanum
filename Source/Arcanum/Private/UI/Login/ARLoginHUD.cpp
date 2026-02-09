#include "UI/Login/ARLoginHUD.h"
#include "UI/Login/SubLayout/KeyInputUserWidget.h"
#include "UI/Login/SubLayout/LoginUserWidget.h"
#include "UI/Login/SubLayout/SyncLoginUserWidget.h"
#include "UI/Login/SubLayout/QuitGameUserWidget.h"
#include "UI/Login/SubLayout/AnnouncetUserWidget.h"
#include "UI/Login/SubLayout/LoginPanelWidget.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Common/CommonDialog.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"

#include "Object/Camera/ARCineCamera.h"
#include "Object/Character/LoginCharacter.h"
#include "Core/ARGameInstance.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UARLoginHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (KeyInputWidget) {
		KeyInputWidget->OnAnyKeyPressed.RemoveDynamic(this, &UARLoginHUD::OnPressAnyKey);
		KeyInputWidget->OnAnyKeyPressed.AddDynamic(this, &UARLoginHUD::OnPressAnyKey);
	}
	if (SyncLoginWidget)		SyncLoginWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (AnnouncetUserWidget)	AnnouncetUserWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (LoginPanelWidget)		LoginPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (QuitGameWidget)			QuitGameWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (ExitCommonDialog)		ExitCommonDialog->SetVisibility(ESlateVisibility::Collapsed);

	if(VerticalBox)				VerticalBox->SetVisibility(ESlateVisibility::Collapsed);
	if (AnnounceBtn) {
		AnnounceBtn->OnClicked.RemoveDynamic(this, &UARLoginHUD::ClickAnnounceBtn);
		AnnounceBtn->OnClicked.AddDynamic(this, &UARLoginHUD::ClickAnnounceBtn);
	}
	if (SettingBtn) {
		SettingBtn->OnClicked.RemoveDynamic(this, &UARLoginHUD::ClickSettingBtn);
		SettingBtn->OnClicked.AddDynamic(this, &UARLoginHUD::ClickSettingBtn);
	}
	if (PlayBtn) {
		PlayBtn->OnClicked.RemoveDynamic(this, &UARLoginHUD::ClickPlayBtn);
		PlayBtn->OnClicked.AddDynamic(this, &UARLoginHUD::ClickPlayBtn);
	}
	if (ExitBtn) {
		ExitBtn->OnClicked.RemoveDynamic(this, &UARLoginHUD::ClickExitBtn);
		ExitBtn->OnClicked.AddDynamic(this, &UARLoginHUD::ClickExitBtn);
	}
	LoginCharacter = Cast<ALoginCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ALoginCharacter::StaticClass()));

	//if (GuestLoginButton)		GuestLoginButton->SetVisibility(ESlateVisibility::Collapsed);
	//if (LoginUserWidget)		LoginUserWidget->SetVisibility(ESlateVisibility::Collapsed);
}
// ========================================================
// PressAnyKey
// ========================================================
void UARLoginHUD::OnPressAnyKey()
{
	if (KeyInputWidget) KeyInputWidget->RemoveFromParent();

	if (SyncLoginWidget) {
		SyncLoginWidget->OnSyncFinished.RemoveDynamic(this, &UARLoginHUD::OnPreLoginSyncFinished);
		SyncLoginWidget->OnSyncFinished.AddDynamic(this, &UARLoginHUD::OnPreLoginSyncFinished);

		SyncLoginWidget->SetVisibility(ESlateVisibility::Visible);
		SyncText = TEXT("서버 동기화중");
		SyncLoginWidget->SetSyncPhase(ESyncPhase::PreLogin, FText::FromString(SyncText));
	}
}
// ========================================================
// 데이터 동기화용 위젯
// ========================================================
void UARLoginHUD::OnPreLoginSyncFinished(bool bIsSuccess, const FString& ErrorMessage)
{
	if (SyncLoginWidget) {
		SyncLoginWidget->OnSyncFinished.RemoveDynamic(this, &UARLoginHUD::OnPreLoginSyncFinished);
		SyncLoginWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if(VerticalBox) VerticalBox->SetVisibility(ESlateVisibility::Visible);
	if (LoginCharacter)  LoginCharacter->AppearCharacter();
}
void UARLoginHUD::OnPostLoginSyncFinished(bool bIsSuccess, const FString& ErrorMessage)
{

}
void UARLoginHUD::VisibleSyncWidget(bool bIsSuccess, const FString& Message)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !SyncLoginWidget) return;
	
	if (bIsSuccess) {
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(SyncLoginWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);

		PC->bShowMouseCursor = true;

		VerticalBox->SetVisibility(ESlateVisibility::Hidden);
		SyncLoginWidget->SetVisibility(ESlateVisibility::Visible);
		SyncLoginWidget->SetSyncPhase(ESyncPhase::ServerRun, FText::FromString(Message));
	}
	else {
		FInputModeGameAndUI InputMode;
		PC->SetInputMode(InputMode);

		SyncLoginWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
// ========================================================
// 공지사항
// ========================================================
void UARLoginHUD::ClickAnnounceBtn()
{
	//bool res = false;
	/// Todo : 추후 서버에서 정보 받아 와야 함. 현재는 타이머 이용
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
		/// Test True Start 
		bool res = true;
		AnnounceString = TEXT("서버 점검 안내:\n" "- 오늘 12시부터 1시간 점검\n" "- 점검 중에는 로그인 불가\n" "- 이용에 참고 바랍니다.");
		HandleAnnouncementOpen(res);
		/// Test True End

		///// Test false Start 
		//bool res = false;
		//AnnounceString = TEXT("서버 점검 중");
		//HandleAnnouncementOpen(res);
		///// Test false End

		}, 3.f, false);

	FString Text = TEXT("서버 접속 중");
	VisibleSyncWidget(true, Text);
	//HandleAnnouncementOpen(res);
}
void UARLoginHUD::HandleAnnouncementOpen(bool bIsSuccess)
{
	VisibleSyncWidget(false);

	if(bIsSuccess){	
		if (AnnouncetUserWidget) {
			AnnouncetUserWidget->SetAnnouncementText(FText::FromString(AnnounceString));
			AnnouncetUserWidget->OnCloseClicked.RemoveDynamic(this, &UARLoginHUD::HandleAnnouncementClose);
			AnnouncetUserWidget->OnCloseClicked.AddDynamic(this, &UARLoginHUD::HandleAnnouncementClose);
			AnnouncetUserWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else {
		QuitGameWidget->SetQuitMessage(AnnounceString);
		QuitGameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
void UARLoginHUD::HandleAnnouncementClose()
{
	if (AnnouncetUserWidget) {
		AnnouncetUserWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (VerticalBox) VerticalBox->SetVisibility(ESlateVisibility::Visible);
}
// ========================================================
// 설정
// ========================================================
void UARLoginHUD::ClickSettingBtn()
{

}
// ========================================================
// 플레이
// ========================================================
void UARLoginHUD::ClickPlayBtn()
{
	if (LoginPanelWidget) {
		if (VerticalBox) VerticalBox->SetVisibility(ESlateVisibility::Hidden);
		
		LoginPanelWidget->SetVisibility(ESlateVisibility::Visible);
		LoginPanelWidget->OnLoginStateChanged.RemoveDynamic(this, &UARLoginHUD::HandlePlayBtn);
		LoginPanelWidget->OnLoginStateChanged.AddDynamic(this, &UARLoginHUD::HandlePlayBtn);
	}
}
void UARLoginHUD::HandlePlayBtn(bool IsState)
{
	LoginPanelWidget->SetVisibility(ESlateVisibility::Hidden);
	if (IsState) {
		UGameplayStatics::OpenLevel(GetWorld(), NextMapName);
	}
	else {
		if (VerticalBox) VerticalBox->SetVisibility(ESlateVisibility::Visible);
	}
}
// ========================================================
// 종료
// ========================================================
void UARLoginHUD::ClickExitBtn()
{
	if (ExitCommonDialog) {
		ExitCommonDialog->SetVisibility(ESlateVisibility::Visible);
		ExitCommonDialog->OnResult.RemoveDynamic(this, &UARLoginHUD::OnExitCommonDialog);
		ExitCommonDialog->OnResult.AddDynamic(this, &UARLoginHUD::OnExitCommonDialog);

		if (VerticalBox) VerticalBox->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UARLoginHUD::OnExitCommonDialog(EDialogResult res)
{
	if (res == EDialogResult::OK) {
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC) UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
	}
	else if (res == EDialogResult::Cancel) {
		ExitCommonDialog->SetVisibility(ESlateVisibility::Hidden);
		if (VerticalBox) VerticalBox->SetVisibility(ESlateVisibility::Visible);
	}
}