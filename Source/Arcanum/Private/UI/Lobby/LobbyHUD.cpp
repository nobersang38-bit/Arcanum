#include "UI/Lobby/LobbyHUD.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/Contents/Currency/CurrencyWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryItemSlotWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Enhancement/EnhancementHUDWidget.h"
#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"
#include "DataInfo/ItemData/DataTable/DTPotionInfoRow.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "UI/Lobby/Contents/Gacha/GachaHUDWidget.h"
//#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/BackgroundBlur.h"
#include "Components/WidgetSwitcher.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/SubSystem/GameTimeSubsystem.h"

void ULobbyHUD::NativeConstruct()
{
	Super::NativeConstruct();

	
	UE_LOG(LogTemp, Log, TEXT("EnhancementHUDWidget = %s"), EnhancementHUDWidget ? TEXT("Valid") : TEXT("Null"));
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
		ShopHUDWidget->InitPanels(EquipmentShopSlotCount, PotionShopSlotCount);
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

	InitShop();
	RestartShopTimer();
	BindShopTimer();
	RefreshShopUI();

	if (!CharacterDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CharacterDataTable is nullptr!"));
		return;
	}

	ClickCharacterMenuBtn();
	RefreshLobbyCurrencyUI();

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
void ULobbyHUD::InitShop()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		FPlayerAccountService::InitializeShop(gameInstance, EquipmentShopSlotCount, PotionShopSlotCount);
	}
}

void ULobbyHUD::RefreshShopUI()
{
	BuildShopRuntimeCache();

	if (ShopHUDWidget)
	{
		ShopHUDWidget->ApplyShopData(
			CachedShopRowNames,
			CachedShopSoldOutStates,
			CachedShopIcons,
			CachedShopNames,
			CachedShopDescs,
			CachedShopPrices
		);

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
			FPlayerAccountService::RefreshShop(gameInstance, EquipmentShopSlotCount, PotionShopSlotCount);
			BuildShopRuntimeCache();
			InRemainingSeconds = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);
		}

		if (ShopHUDWidget)
		{
			ShopHUDWidget->ApplyShopData(
				CachedShopRowNames,
				CachedShopSoldOutStates,
				CachedShopIcons,
				CachedShopNames,
				CachedShopDescs,
				CachedShopPrices
			);
		}
	}

	if (ShopHUDWidget)
	{
		ShopHUDWidget->SetShopRemainingSeconds(InRemainingSeconds);
	}
}

void ULobbyHUD::BuildShopRuntimeCache()
{
	const int32 slotCount = FMath::Max(0, EquipmentShopSlotCount) + FMath::Max(0, PotionShopSlotCount);

	CachedShopRowNames.SetNum(slotCount);
	CachedShopSoldOutStates.SetNum(slotCount);

	CachedShopIcons.SetNum(slotCount);
	CachedShopNames.SetNum(slotCount);
	CachedShopDescs.SetNum(slotCount);
	CachedShopPrices.SetNum(slotCount);

	for (int32 i = 0; i < slotCount; i++)
	{
		CachedShopRowNames[i] = NAME_None;
		CachedShopSoldOutStates[i] = false;

		CachedShopIcons[i] = nullptr;
		CachedShopNames[i] = FText::GetEmpty();
		CachedShopDescs[i] = FText::GetEmpty();
		CachedShopPrices[i] = 0;
	}

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	for (int32 slotIndex = 0; slotIndex < slotCount; slotIndex++)
	{
		if (!gameInstance->CurrentShopKeys.IsValidIndex(slotIndex)) continue;

		const FShopProductKey& key = gameInstance->CurrentShopKeys[slotIndex];

		if (gameInstance->CurrentShopSoldOutStates.IsValidIndex(slotIndex))
		{
			CachedShopSoldOutStates[slotIndex] = gameInstance->CurrentShopSoldOutStates[slotIndex];
		}

		if (!key.TableTag.IsValid() || key.RowName.IsNone()) continue;

		// ItemCatalog 직접 키(=RowName이 ItemTagName)
		if (key.TableTag.MatchesTagExact(Arcanum::DataTable::ItemCatalog))
		{
			const FDTItemCatalogRow* catalogRow = dataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, key.RowName);
			if (!catalogRow) continue;

			CachedShopRowNames[slotIndex] = key.RowName;
			CachedShopIcons[slotIndex] = catalogRow->Icon;
			CachedShopNames[slotIndex] = catalogRow->DisplayName;
			CachedShopDescs[slotIndex] = catalogRow->Desc;
			CachedShopPrices[slotIndex] = catalogRow->BuyPrice;

			continue;
		}

		CachedShopRowNames[slotIndex] = key.RowName;
	}
}

void ULobbyHUD::RestartShopTimer()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (gameInstance->bShopPaused && gameInstance->PausedShopRemainingSeconds > 0)
		{
			const FDateTime nowKst = FPlayerAccountService::GetCurrentTimeKST();
			gameInstance->NextShopRefreshTime = nowKst + FTimespan(0, 0, gameInstance->PausedShopRemainingSeconds);

			gameInstance->bShopPaused = false;
			gameInstance->PausedShopRemainingSeconds = 0;

			if (UGameTimeSubsystem* gameTimeSubsystem = gameInstance->GetSubsystem<UGameTimeSubsystem>())
			{
				gameTimeSubsystem->StartShop(gameInstance->NextShopRefreshTime);
			}
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
