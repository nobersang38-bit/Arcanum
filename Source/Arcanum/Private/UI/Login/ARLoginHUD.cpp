#include "UI/Login/ARLoginHUD.h"

#include "UI/Login/SubLayout/KeyInputUserWidget.h"
#include "UI/Login/SubLayout/LoginUserWidget.h"
#include "UI/Login/SubLayout/SyncLoginUserWidget.h"

#include "GameFramework/PlayerController.h"

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
	if (LoginUserWidget) {
		LoginUserWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}
void UARLoginHUD::OnPressAnyKey()
{
	if (KeyInputWidget) KeyInputWidget->RemoveFromParent();

	if (SyncLoginWidget) {
		SyncLoginWidget->SetVisibility(ESlateVisibility::Visible);
		SyncLoginWidget->SetSyncPhase(ESyncPhase::PreLogin);
	}
}

void UARLoginHUD::OnPreLoginSyncFinished()
{
	if (SyncLoginWidget) SyncLoginWidget->RemoveFromParent();	 
	//if (LoginUserWidget) LoginUserWidget->AddToViewport();
}