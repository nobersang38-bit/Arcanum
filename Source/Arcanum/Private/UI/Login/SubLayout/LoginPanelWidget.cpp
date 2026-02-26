#include "UI/Login/SubLayout/LoginPanelWidget.h"

#include "UI/Common/CommonBtnWidget.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Login/SubLayout/LoginUserWidget.h"
#include "UI/Login/SubLayout/SyncLoginUserWidget.h"

#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"

#include "Components/Image.h"
#include "Components/Overlay.h"
// ========================================================
// 언리얼 기본 생성
// ========================================================
void ULoginPanelWidget::NativeConstruct()
{
	if (ExitBtn) {
		ExitBtn->OnClicked.RemoveDynamic(this, &ULoginPanelWidget::ClickExitBtn);
		ExitBtn->OnClicked.AddDynamic(this, &ULoginPanelWidget::ClickExitBtn);
	}

	if (GuestBtn) {
		GuestBtn->OnClicked.RemoveDynamic(this, &ULoginPanelWidget::OnGuestLoginClicked);
		GuestBtn->OnClicked.AddDynamic(this, &ULoginPanelWidget::OnGuestLoginClicked);
	}

	if (LoginUserWidget) LoginUserWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (SyncLoginWidget) SyncLoginWidget->SetVisibility(ESlateVisibility::Collapsed);
	if (ErrCommonDialog) ErrCommonDialog->SetVisibility(ESlateVisibility::Collapsed);
}
// ========================================================
// 공통
// ========================================================
void ULoginPanelWidget::ClickExitBtn()
{
	HandleIDLoginState(false);
}
void ULoginPanelWidget::HandleIDLoginState(bool IsState)
{
	OnLoginStateChanged.Broadcast(IsState);
}
// ========================================================
// 게스트 로그인
// ========================================================
void ULoginPanelWidget::OnGuestLoginClicked()
{
	if (LoginOverlay && LoginUserWidget) {
		LoginOverlay->SetVisibility(ESlateVisibility::Hidden);

		LoginUserWidget->SetVisibility(ESlateVisibility::Visible);
		LoginUserWidget->OnOKClicked.RemoveDynamic(this, &ULoginPanelWidget::HandleLoginOK);
		LoginUserWidget->OnCancelClicked.RemoveDynamic(this, &ULoginPanelWidget::HandleLoginCancel);

		LoginUserWidget->OnOKClicked.AddDynamic(this, &ULoginPanelWidget::HandleLoginOK);
		LoginUserWidget->OnCancelClicked.AddDynamic(this, &ULoginPanelWidget::HandleLoginCancel);
	}

}

void ULoginPanelWidget::HandleLoginOK(const FString& ID, const FString& PW)
{
	if (!LoginUserWidget || !SyncLoginWidget) return;

	LoginUserWidget->SetVisibility(ESlateVisibility::Collapsed);

	SyncText = TEXT("로그인 중");
	SyncLoginWidget->SetVisibility(ESlateVisibility::Visible);
	SyncLoginWidget->SetSyncPhase(ESyncPhase::PostLogin, FText::FromString(SyncText));

	StartPostLogin(ID, PW);
}
void ULoginPanelWidget::StartPostLogin(const FString& ID, const FString& PW)
{
	/// Todo : 현재는 서버가 없기 때문에 타이머로 대체

	/// Test Start
	UARGameInstance* GI = GetGameInstance<UARGameInstance>();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GI, ID, PW]() {
		bool bSuccess = false;
		FString ErrorMsg = TEXT("아이디 또는 비밀번호가 틀렸습니다.");
		if (GI) {
			if (GI->CheckLogin(ID, PW)) {
				bSuccess = true;
				ErrorMsg = TEXT("");
			}
		}
		else ErrorMsg = TEXT("시스템 오류: GameInstance를 찾을 수 없습니다.");
		OnPostLoginFinished(bSuccess, ErrorMsg);

		}, 2.0f, false);
	/// Test End
}
void ULoginPanelWidget::OnPostLoginFinished(bool bSuccess, const FString& ErrorMessage)
{
	SyncLoginWidget->SetVisibility(ESlateVisibility::Collapsed);

	if (bSuccess) {
		FPlayerAccountService::LoadPlayerData(this);
		LoginUserWidget->SetVisibility(ESlateVisibility::Collapsed);
		HandleIDLoginState(true);
	}
	else {
		ErrCommonDialog->SetVisibility(ESlateVisibility::Visible);
		ErrCommonDialog->OnResult.RemoveDynamic(this, &ULoginPanelWidget::OnErrCommonDialog);
		ErrCommonDialog->OnResult.AddDynamic(this, &ULoginPanelWidget::OnErrCommonDialog);
	}
}
void ULoginPanelWidget::OnErrCommonDialog(EDialogResult res)
{
	if (res == EDialogResult::OK) {
		ErrCommonDialog->SetVisibility(ESlateVisibility::Hidden);
		LoginUserWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
void ULoginPanelWidget::HandleLoginCancel()
{
	if(LoginUserWidget) LoginUserWidget->SetVisibility(ESlateVisibility::Hidden);
	if(LoginOverlay) LoginOverlay->SetVisibility(ESlateVisibility::Visible);
	ClickExitBtn();
}
// ========================================================
// Google 로그인(W.I.P)
// ========================================================
void ULoginPanelWidget::OnGoogleLoginClicked()
{
}