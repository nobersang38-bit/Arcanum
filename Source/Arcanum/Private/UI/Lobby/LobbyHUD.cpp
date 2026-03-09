#include "UI/Lobby/LobbyHUD.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/Contents/Currency/CurrencyWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Enhancement/EnhancementHUDWidget.h"
#include "UI/Lobby/Contents/Gacha/GachaHUDWidget.h"
//#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/BackgroundBlur.h"
#include "Components/WidgetSwitcher.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "Core/ARPlayerAccountService.h"

void ULobbyHUD::NativeConstruct()
{
	Super::NativeConstruct();

	/// 02/26 수정 : 서비스레이어 거치도록
	CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);


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

	if (!CharacterDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterDataTable is nullptr!"));
		return;
	}

	if (ShopHUDWidget)
	{
		ShopHUDWidget->SetParentLobby(this);
		ShopHUDWidget->InitPanels();
		ShopHUDWidget->InitShop();
		ShopHUDWidget->RestartShopTimer();
		ShopHUDWidget->BindShopTimer();
		ShopHUDWidget->RefreshShopUI();
	}

	if (InventoryHUDWidget)
	{
		InventoryHUDWidget->SetParentLobby(this);
		InventoryHUDWidget->InitInventorySlots(GetInventoryCapacity());
	}

	if (EnhancementHUDWidget)
	{
		EnhancementHUDWidget->SetParentLobby(this);
	}

	RefreshAllLobbyUI();

	ClickCharacterMenuBtn();

	FPlayerAccountService::OnSaveCompleted.RemoveDynamic(this, &ULobbyHUD::HandleSaveCompleted);
	FPlayerAccountService::OnSaveCompleted.AddDynamic(this, &ULobbyHUD::HandleSaveCompleted);
}
void ULobbyHUD::HandleSaveCompleted(bool bSuccess)
{
	if (bSuccess) {
		RefreshAllLobbyUI();
		UE_LOG(LogTemp, Log, TEXT("HUD: 저장이 성공적으로 완료되었습니다!"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("HUD: 저장 실패!"));
	}
}
void ULobbyHUD::RefreshAllLobbyUI()
{
	CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);

	// TODO: 로비 갱신
	RefreshLobbyCurrencyUI();
	if (InventoryHUDWidget)
	{
		InventoryHUDWidget->RefreshInventoryUI();
	}

	if (EnhancementHUDWidget)
	{
		EnhancementHUDWidget->RefreshEquipmentInventory();
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
	TArray<FName> RowNames = CharacterDataTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		UE_LOG(LogTemp, Log, TEXT("RowName: %s"), *RowName.ToString());
	}
}

void ULobbyHUD::ClickEnhancementMenuBtn()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
}

void ULobbyHUD::ClickShopMenuBtn()
{
	if (WidgetSwitcher && ShopHUDWidget)
	{
		ShopHUDWidget->SetParentLobby(this);
		WidgetSwitcher->SetActiveWidgetIndex(3);
	}
}

void ULobbyHUD::ClickGachaMenuBtn()
{
	if (UGachaHUDWidget* GachaWidget = Cast<UGachaHUDWidget>(WidgetSwitcher->GetWidgetAtIndex(4))) {
		GachaWidget->SetParentLobby(this);
		WidgetSwitcher->SetActiveWidget(GachaWidget);
	}
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
		if (MenuHorizontalBox)
		{
			MenuHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
		}
		if (SettingUHorizontalBox)
		{
			SettingUHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
		}
		if (CurrencyWidget)
		{
			CurrencyWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

// ========================================================
// 재화
// ========================================================
void ULobbyHUD::RefreshLobbyCurrencyUI()
{
	if (CurrencyWidget)
	{
		CurrencyWidget->RefreshCurrencyUI(CachedPlayerData);
	}
}

// ========================================================
// 인벤토리
// ========================================================



// ========================================================
// 상점
// ========================================================










void ULobbyHUD::OnExitCommonDialog(EDialogResult res)
{
	if (res == EDialogResult::OK)
	{
		//UE_LOG(LogTemp, Log, TEXT("ok 클릭"));
		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (PC)
		{
			UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
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
		if (CurrencyWidget)
		{
			CurrencyWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
