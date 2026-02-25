// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyHUD.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
//#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/BackgroundBlur.h"
#include "Components/WidgetSwitcher.h"


void ULobbyHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitCommonDialog) ExitCommonDialog->SetVisibility(ESlateVisibility::Collapsed);
	if (BackgroundBlur) BackgroundBlur->SetVisibility(ESlateVisibility::Collapsed);

	if (BattleMenuBtn) {
		BattleMenuBtn->OnClicked.RemoveDynamic(this, &ULobbyHUD::ClickBattleMenuBtn);
		BattleMenuBtn->OnClicked.AddDynamic(this, &ULobbyHUD::ClickBattleMenuBtn);
	}

	if (CharacterMenuBtn) {
		CharacterMenuBtn->OnClicked.RemoveDynamic(this, &ULobbyHUD::ClickCharacterMenuBtn);
		CharacterMenuBtn->OnClicked.AddDynamic(this, &ULobbyHUD::ClickCharacterMenuBtn);
	}

	if (EnhancementMenuBtn) {
		EnhancementMenuBtn->OnClicked.RemoveDynamic(this, &ULobbyHUD::ClickEnhancementMenuBtn);
		EnhancementMenuBtn->OnClicked.AddDynamic(this, &ULobbyHUD::ClickEnhancementMenuBtn);
	}

	if (ShopMenuBtn) {
		ShopMenuBtn->OnClicked.RemoveDynamic(this, &ULobbyHUD::ClickShopMenuBtn);
		ShopMenuBtn->OnClicked.AddDynamic(this, &ULobbyHUD::ClickShopMenuBtn);
	}
	if (ShopHUDWidget)
	{
		ShopHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (GachaMenuBtn) {
		GachaMenuBtn->OnClicked.RemoveDynamic(this, &ULobbyHUD::ClickGachaMenuBtn);
		GachaMenuBtn->OnClicked.AddDynamic(this, &ULobbyHUD::ClickGachaMenuBtn);
	}

	if (SettingBtn) {
		SettingBtn->OnClicked.RemoveDynamic(this, &ULobbyHUD::ClickSettingBtn);
		SettingBtn->OnClicked.AddDynamic(this, &ULobbyHUD::ClickSettingBtn);
	}

	if (QuitBtn) {
		QuitBtn->OnClicked.RemoveDynamic(this, &ULobbyHUD::ClickQuitBtn);
		QuitBtn->OnClicked.AddDynamic(this, &ULobbyHUD::ClickQuitBtn);
	}
}

void ULobbyHUD::ClickBattleMenuBtn()
{
	/// TODO : 전투 위젯 띄우기
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void ULobbyHUD::ClickCharacterMenuBtn()
{
	/// TODO : 캐릭터 위젯 띄우기

	/*if (CharacterWidgetClass)
	{
		if (!CharacterWidget)
		{
			CharacterWidget = CreateWidget<UCharacterHUDWidget>(GetWorld(), CharacterWidgetClass);
			if (CharacterWidget)
			{
				CharacterWidget->AddToViewport();
			}
		}
	}*/
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}

}

void ULobbyHUD::ClickEnhancementMenuBtn()
{
	/// TODO : 강화 위젯 띄우기
}

void ULobbyHUD::ClickShopMenuBtn()
{
	if (ShopHUDWidget)
	{
		ShopHUDWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyHUD::ClickGachaMenuBtn()
{
	/// TODO : 가챠 위젯 띄우기
}

void ULobbyHUD::ClickSettingBtn()
{
	/// TODO : 설정 위젯 띄우기
}




// ========================================================
// 종료
// ========================================================

void ULobbyHUD::ClickQuitBtn()
{
	// 종료확인창 띄우기
	if (ExitCommonDialog)
	{
		if (WidgetSwitcher)
		{
			WidgetSwitcher->SetVisibility(ESlateVisibility::Hidden); // 위젯스위처 숨기기
		}

		ExitCommonDialog->SetVisibility(ESlateVisibility::Visible);
		BackgroundBlur->SetVisibility(ESlateVisibility::Visible);

		ExitCommonDialog->OnResult.RemoveDynamic(this, &ULobbyHUD::OnExitCommonDialog);
		ExitCommonDialog->OnResult.AddDynamic(this, &ULobbyHUD::OnExitCommonDialog);
	
		// 팝업 떠있을때는 다른 버튼 숨기기
		if (MenuHorizontalBox )
		{
			MenuHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
		}
		if (SettingUHorizontalBox)
		{
			SettingUHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
		}
		if (GoodsHorizontalBox)
		{
			GoodsHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
		}
	}



}

void ULobbyHUD::OnExitCommonDialog(EDialogResult res)
{
	if (res == EDialogResult::OK)
	{
		//UE_LOG(LogTemp, Log, TEXT("ok 클릭"));
		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (PC)
		{
			UKismetSystemLibrary::QuitGame(GetWorld(),PC, EQuitPreference::Quit, true);
		}
	}
	else if (res == EDialogResult::Cancel)
	{
		//UE_LOG(LogTemp, Log, TEXT("cancel 클릭"));if (WidgetSwitcher)
		{
			WidgetSwitcher->SetVisibility(ESlateVisibility::Visible);
		}

		ExitCommonDialog->SetVisibility(ESlateVisibility::Hidden);
		BackgroundBlur->SetVisibility(ESlateVisibility::Collapsed);

		if (MenuHorizontalBox)
		{
			MenuHorizontalBox->SetVisibility(ESlateVisibility::Visible);
		}
		if (SettingUHorizontalBox)
		{
			SettingUHorizontalBox->SetVisibility(ESlateVisibility::Visible);
		}
		if (GoodsHorizontalBox)
		{
			GoodsHorizontalBox->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
