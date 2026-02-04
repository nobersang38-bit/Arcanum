#include "UI/Login/ARLoginHUD.h"
#include "UI/Login/SubLayout/KeyInputUserWidget.h"
#include "UI/Login/SubLayout/LoginUserWidget.h"
#include "UI/Login/SubLayout/SyncLoginUserWidget.h"
#include "UI/Login/SubLayout/QuitGameUserWidget.h"
#include "UI/Login/SubLayout/AnnouncetUserWidget.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/Button.h"

#include "Object/Camera/ARCineCamera.h"
#include "Object/Character/LoginCharacter.h"

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
	if (SyncLoginWidget) {
		SyncLoginWidget->OnSyncFinished.RemoveDynamic(this, &UARLoginHUD::OnPreLoginSyncFinished);
		SyncLoginWidget->OnSyncFinished.AddDynamic(this, &UARLoginHUD::OnPreLoginSyncFinished);
		SyncLoginWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (AnnouncetUserWidget) {
		AnnouncetUserWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (GuestLoginButton) {
		GuestLoginButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (LoginUserWidget) {
		LoginUserWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (QuitGameWidget) QuitGameWidget->SetVisibility(ESlateVisibility::Collapsed);

	SetCameraByRole(Arcanum::LoginUI::Open);
	LoginCharacter = Cast<ALoginCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), ALoginCharacter::StaticClass()));
}
void UARLoginHUD::OnPressAnyKey()
{
	if (KeyInputWidget) KeyInputWidget->RemoveFromParent();

	if (SyncLoginWidget) {
		SyncLoginWidget->SetVisibility(ESlateVisibility::Visible);
		SyncText = TEXT("동기화중");
		SyncLoginWidget->SetSyncPhase(ESyncPhase::PreLogin, FText::FromString(SyncText));
	}
}

void UARLoginHUD::OnPreLoginSyncFinished(bool bIsSuccess, const FString& ErrorMessage)
{
	if (SyncLoginWidget) {
		SyncLoginWidget->OnSyncFinished.RemoveDynamic(this, &UARLoginHUD::OnPreLoginSyncFinished);
		SyncLoginWidget->SetVisibility(ESlateVisibility::Collapsed);

		SyncLoginWidget->OnSyncFinished.RemoveDynamic(this, &UARLoginHUD::OnPostLoginSyncFinished);
		SyncLoginWidget->OnSyncFinished.AddDynamic(this, &UARLoginHUD::OnPostLoginSyncFinished);
	}

	///// Test Start
	//bIsSuccess = false;
	///// Test End

	if(bIsSuccess){
		SetCameraByRole(Arcanum::LoginUI::Login, false);
		
		if (LoginCharacter)  LoginCharacter->PlayAppearEffect();
		if (AnnouncetUserWidget) {
			float Delay = LoginCharacter->GetAppearDuration() + 2.f;
			
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
				/// Test Start
				AnnounceString = TEXT("서버 점검 안내:\n" "- 오늘 12시부터 1시간 점검\n" "- 점검 중에는 로그인 불가\n" "- 이용에 참고 바랍니다.");
				/// Test End

				AnnouncetUserWidget->SetAnnouncementText(FText::FromString(AnnounceString));
				AnnouncetUserWidget->OnCloseClicked.AddDynamic(this, &UARLoginHUD::HandleAnnouncementClose);
				AnnouncetUserWidget->SetVisibility(ESlateVisibility::Visible); 
			}, Delay, false);
		}
	}
	else {
		QuitGameWidget->SetQuitMessage(ErrorMessage);

		///// Test Start
		//QuitGameWidget->SetQuitMessage(TEXT("서버 점검 중"));
		///// Test End

		QuitGameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
void UARLoginHUD::OnPostLoginSyncFinished(bool bIsSuccess, const FString& ErrorMessage)
{

}

void UARLoginHUD::HandleAnnouncementClose()
{
	if (AnnouncetUserWidget) AnnouncetUserWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	if (GuestLoginButton) {
		GuestLoginButton->SetVisibility(ESlateVisibility::Visible);
		GuestLoginButton->OnClicked.RemoveDynamic(this, &UARLoginHUD::OnGuestLoginButtonClicked);
		GuestLoginButton->OnClicked.AddDynamic(this, &UARLoginHUD::OnGuestLoginButtonClicked);
	}
}
void UARLoginHUD::OnGuestLoginButtonClicked()
{
	if (LoginUserWidget) {
		LoginUserWidget->SetVisibility(ESlateVisibility::Visible);
		LoginUserWidget->OnCancelClicked.RemoveDynamic(this, &UARLoginHUD::HandleLoginCancel);
		LoginUserWidget->OnCancelClicked.AddDynamic(this, &UARLoginHUD::HandleLoginCancel);
	}
}

void UARLoginHUD::HandleLoginCancel()
{
	if (LoginUserWidget) {
		LoginUserWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UARLoginHUD::SetCameraByRole(FGameplayTag CameraRole, bool bImmediately)
{
	if (!GetWorld()) return;

	AARCineCamera* TargetCamera = nullptr;

	for (TActorIterator<AARCineCamera> It(GetWorld()); It; ++It) {
		if (It->CameraRole == CameraRole) {
			TargetCamera = *It;
			break;
		}
	}

	if (!TargetCamera) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) {

		if(bImmediately) PC->SetViewTarget(TargetCamera);
		else PC->SetViewTargetWithBlend(TargetCamera, CameraMoving, EViewTargetBlendFunction::VTBlend_Cubic, 0.5f);
	}
}