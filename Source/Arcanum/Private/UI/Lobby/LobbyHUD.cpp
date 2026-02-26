#include "UI/Lobby/LobbyHUD.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/Contents/Currency/CurrencyWidget.h"
#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"
//#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/BackgroundBlur.h"
#include "Components/WidgetSwitcher.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameTimeSubsystem.h"


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

	if (ShopHUDWidget)
	{
		ShopHUDWidget->InitShopSlots(ShopSlotCount);
		ShopHUDWidget->OnBuyRequested.RemoveDynamic(this, &ULobbyHUD::TryPurchaseSelectedItem);
		ShopHUDWidget->OnBuyRequested.AddDynamic(this, &ULobbyHUD::TryPurchaseSelectedItem);
	}

	BindGameInstanceEvents();
	RefreshAllLobbyUI();

	InitShop();
	BindShopTimer();
	RefreshShopUI();

	// 로비로 돌아올떄(무조건 들오올떄) 스타트 하는거 넣어놈
}

void ULobbyHUD::RefreshAllLobbyUI()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		CachedPlayerData = gameInstance->GetPlayerDataCopy();

		// TODO: 로비 갱신
		RefreshLobbyCurrencyUI();

	}
}

void ULobbyHUD::BindGameInstanceEvents()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		gameInstance->OnCurrencyChanged.RemoveDynamic(this, &ULobbyHUD::HandleCurrencyChanged);
		gameInstance->OnCurrencyChanged.AddDynamic(this, &ULobbyHUD::HandleCurrencyChanged);
	}
}

void ULobbyHUD::HandleCurrencyChanged()
{
	RefreshAllLobbyUI();
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
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
}

void ULobbyHUD::ClickShopMenuBtn()
{
	if (WidgetSwitcher)
	{
		WidgetSwitcher->SetActiveWidgetIndex(3);
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
// 상점
// ========================================================
void ULobbyHUD::InitShop()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		FPlayerAccountService::InitializeShop(gameInstance, ShopSlotCount);
	}
}

void ULobbyHUD::RefreshShopUI()
{
	BuildShopRuntimeCache();

	if (ShopHUDWidget)
	{
		ShopHUDWidget->ApplyShopData(CachedShopRowNames, CachedShopSoldOutStates, CachedShopRowPtrs);

		if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
		{
			const int32 remaining = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);

			HandleShopSecondChanged(remaining);
		}
	}
}

void ULobbyHUD::BindShopTimer()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (UGameTimeSubsystem* gameTimeSubsystem = gameInstance->GetSubsystem<UGameTimeSubsystem>())
		{
			gameTimeSubsystem->OnShopSecondChanged.RemoveDynamic(this, &ULobbyHUD::HandleShopSecondChanged);
			gameTimeSubsystem->OnShopSecondChanged.AddDynamic(this, &ULobbyHUD::HandleShopSecondChanged);
		}

		const int32 remaining = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);
		HandleShopSecondChanged(remaining);
	}
}

void ULobbyHUD::HandleShopSecondChanged(int32 InRemainingSeconds)
{
	if (InRemainingSeconds == 0)
	{
		if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
		{
			FPlayerAccountService::RefreshShop(gameInstance, ShopSlotCount);
			BuildShopRuntimeCache();
			InRemainingSeconds = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);
		}

		if (ShopHUDWidget)
		{
			ShopHUDWidget->ApplyShopData(CachedShopRowNames, CachedShopSoldOutStates, CachedShopRowPtrs);
			ShopHUDWidget->ClearShopSelection();
		}
	}

	if (ShopHUDWidget)
	{
		ShopHUDWidget->SetShopRemainingSeconds(InRemainingSeconds);
	}
}

void ULobbyHUD::TryPurchaseSelectedItem(FName InItemRowName)
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (FPlayerAccountService::PurchaseEquipment(gameInstance, InItemRowName))
		{
			// 구매한 RowName이 들어있는 슬롯을 찾아 품절 처리
			const int32 slotIndex = gameInstance->CurrentShopRowNames.IndexOfByKey(InItemRowName);
			if (slotIndex != INDEX_NONE && gameInstance->CurrentShopSoldOutStates.IsValidIndex(slotIndex))
			{
				gameInstance->CurrentShopSoldOutStates[slotIndex] = true;
			}

			// UI 갱신
			gameInstance->OnCurrencyChanged.Broadcast();
			BuildShopRuntimeCache();

			if (ShopHUDWidget)
			{
				ShopHUDWidget->ApplyShopData(CachedShopRowNames, CachedShopSoldOutStates, CachedShopRowPtrs);
			}
		}
	}
}

void ULobbyHUD::BuildShopRuntimeCache()
{
	CachedShopRowNames.Reset();
	CachedShopSoldOutStates.Reset();
	CachedShopRowPtrs.Reset();

	int32 slotCount = FMath::Max(0, ShopSlotCount);

	CachedShopRowNames.SetNum(slotCount);
	CachedShopSoldOutStates.SetNum(slotCount);
	CachedShopRowPtrs.SetNum(slotCount);

	for (int32 i = 0; i < slotCount; i++)
	{
		CachedShopRowNames[i] = NAME_None;
		CachedShopSoldOutStates[i] = false;
		CachedShopRowPtrs[i] = nullptr;
	}

	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>())
		{
			for (int32 slotIndex = 0; slotIndex < slotCount; slotIndex++)
			{
				if (gameInstance->CurrentShopRowNames.IsValidIndex(slotIndex))
				{
					CachedShopRowNames[slotIndex] = gameInstance->CurrentShopRowNames[slotIndex];
				}

				if (gameInstance->CurrentShopSoldOutStates.IsValidIndex(slotIndex))
				{
					CachedShopSoldOutStates[slotIndex] = gameInstance->CurrentShopSoldOutStates[slotIndex];
				}

				const FName rowName = CachedShopRowNames[slotIndex];
				if (rowName == NAME_None)
				{
					continue;
				}

				const FDTEquipmentInfoRow* rowPtr = dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, rowName);
				CachedShopRowPtrs[slotIndex] = rowPtr;
			}
		}
	}
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
