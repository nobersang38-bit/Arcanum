#include "UI/Lobby/LobbyHUD.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/Contents/Currency/CurrencyWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryItemSlotWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"
#include "DataInfo/ItemData/Potion/DTPotionInfoRow.h"
#include "DataInfo/InventoryData/DataTable/DTInventoryRuleItem.h"
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
		ShopHUDWidget->InitPanels(EquipmentShopSlotCount, PotionShopSlotCount);
		ShopHUDWidget->OnBuyRequested.RemoveDynamic(this, &ULobbyHUD::TryPurchaseSelectedItem);
		ShopHUDWidget->OnBuyRequested.AddDynamic(this, &ULobbyHUD::TryPurchaseSelectedItem);

		ShopHUDWidget->OnSellRequested.RemoveDynamic(this, &ULobbyHUD::TrySellSelectedItem);
		ShopHUDWidget->OnSellRequested.AddDynamic(this, &ULobbyHUD::TrySellSelectedItem);
	}

	if (InventoryHUDWidget)
	{
		InventoryHUDWidget->OnInventorySlotSelected.RemoveDynamic(this, &ULobbyHUD::HandleInventorySlotSelected);
		InventoryHUDWidget->OnInventorySlotSelected.AddDynamic(this, &ULobbyHUD::HandleInventorySlotSelected);
	}

	if (InventorySortBtn)
	{
		InventorySortBtn->OnClicked.RemoveDynamic(this, &ULobbyHUD::ClickInventorySortBtn);
		InventorySortBtn->OnClicked.AddDynamic(this, &ULobbyHUD::ClickInventorySortBtn);
	}

	BindGameInstanceEvents();
	BuildEquipmentRowCache();
	BuildPotionRowCache();
	BuildInventoryRuleTableCache();
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

	/// 02/26 수정 : 서비스레이어 거치도록
	CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
	//if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance())) {
	//	CachedPlayerData = gameInstance->GetPlayerDataCopy();
	//	RefreshLobbyCurrencyUI();
	//}
	ClickCharacterMenuBtn();
	RefreshLobbyCurrencyUI();
}

void ULobbyHUD::RefreshAllLobbyUI()
{
	CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);

	// TODO: 로비 갱신
	RefreshLobbyCurrencyUI();
	RefreshInventoryUI();
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
	if (WidgetSwitcher)
	{
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
void ULobbyHUD::HandleInventorySlotSelected(const FInventoryViewSlot& InSlot)
{
	SelectedInventoryItemGuid = InSlot.ItemGuid;
}

void ULobbyHUD::RefreshInventoryUI()
{
	if (InventoryHUDWidget)
	{
		if (UARGameInstance* const gameInstance = Cast<UARGameInstance>(GetGameInstance()))
		{
			const int32 slotCount = FMath::Max(1, GetInventoryCapacityFromRuleTable());

			TArray<FInventoryViewSlot> viewSlots;
			viewSlots.Reserve(slotCount);

			BuildInventoryViewSlots(viewSlots, slotCount);

			InventoryHUDWidget->ApplyInventorySlots(viewSlots);
		}
	}
}

void ULobbyHUD::BuildEquipmentRowCache()
{
	EquipmentRowByTag.Reset();

	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>())
		{
			UDataTable* const* tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment);
			if (!tablePtr || !(*tablePtr)) { return; }

			UDataTable* table = *tablePtr;

			TArray<FDTEquipmentInfoRow*> rows;
			table->GetAllRows(TEXT("BuildEquipmentRowCache"), rows);

			for (FDTEquipmentInfoRow* row : rows)
			{
				if (row && row->ItemTag.IsValid())
				{
					if (!EquipmentRowByTag.Contains(row->ItemTag))
					{
						EquipmentRowByTag.Add(row->ItemTag, row);
					}
				}
			}
		}
	}
}

const FDTEquipmentInfoRow* ULobbyHUD::FindEquipmentRowByTag(const FGameplayTag& InItemTag) const
{
	if (InItemTag.IsValid())
	{
		if (const FDTEquipmentInfoRow* const* found = EquipmentRowByTag.Find(InItemTag))
		{
			return *found;
		}
	}

	return nullptr;
}

void ULobbyHUD::BuildPotionRowCache()
{
	PotionRowByTag.Reset();

	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>())
		{
			UDataTable* const* tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Potion);
			if (!tablePtr || !(*tablePtr)) return;

			UDataTable* table = *tablePtr;

			TArray<FDTPotionInfoRow*> rows;
			table->GetAllRows(TEXT("BuildPotionRowCache"), rows);

			for (FDTPotionInfoRow* row : rows)
			{
				if (row && row->PotionTag.IsValid())
				{
					if (!PotionRowByTag.Contains(row->PotionTag))
					{
						PotionRowByTag.Add(row->PotionTag, row);
					}
				}
			}
		}
	}
}

void ULobbyHUD::BuildInventoryRuleTableCache()
{
	InventoryRuleRow = nullptr;

	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>())
		{
			if (UDataTable* const* tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::InventoryRule))
			{
				if (*tablePtr)
				{
					InventoryRuleRow = (*tablePtr)->FindRow<FDTInventoryRuleItem>(FName("Default"), TEXT("BuildInventoryRuleTableCache"));
				}
			}
		}
	}

	if (InventoryRuleRow)
	{
		for (const FInventorySlotOrderItem& item : InventoryRuleRow->SlotOrders)
		{
			if (item.SlotTag.IsValid())
			{
				SlotOrderByTag.Add(item.SlotTag, item.Order);
			}
		}
	}

	if (InventoryHUDWidget)
	{
		InventoryHUDWidget->InitInventorySlots(GetInventoryCapacityFromRuleTable());
	}
}

int32 ULobbyHUD::GetInventoryCapacityFromRuleTable() const
{
	if (InventoryRuleRow)
	{
		return FMath::Max(0, InventoryRuleRow->Capacity);
	}

	return 0;
}

bool ULobbyHUD::FindMaxStackByItemTag(const FGameplayTag& InItemTag, int32& OutMaxStack) const
{
	OutMaxStack = 1;
	if (!InventoryRuleRow) return false;
	if (!InItemTag.IsValid()) return false;

	const FInventoryRuleItem* bestRule = nullptr;

	for (const FInventoryRuleItem& rule : InventoryRuleRow->Rules)
	{
		if (rule.RuleTag.IsValid() && InItemTag.MatchesTag(rule.RuleTag))
		{
			if (!bestRule || rule.Priority > bestRule->Priority)
			{
				bestRule = &rule;
			}
		}
	}

	if (!bestRule) return false;

	if (bestRule->StackRule == EInventoryStackRule::Stackable)
	{
		OutMaxStack = FMath::Max(1, bestRule->MaxStack);
		return true;
	}

	OutMaxStack = 1;
	return true;
}

int32 ULobbyHUD::GetSlotOrderFromRuleTable(const FGameplayTag& InSlotTag) const
{
	if (!InSlotTag.IsValid())
	{
		return 999;
	}

	if (const int32* found = SlotOrderByTag.Find(InSlotTag))
	{
		return *found;
	}

	return 999;
}

const FDTPotionInfoRow* ULobbyHUD::FindPotionRowByTag(const FGameplayTag& InPotionTag) const
{
	if (InPotionTag.IsValid())
	{
		if (const FDTPotionInfoRow* const* found = PotionRowByTag.Find(InPotionTag))
		{
			return *found;
		}
	}

	return nullptr;
}

void ULobbyHUD::BuildInventoryViewSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const
{
	OutSlots.Reset();
	const int32 slotCount = FMath::Max(1, InSlotLimit);
	OutSlots.Reserve(slotCount);

	if (bInventorySortedView)
	{
		AppendPotionSlots(OutSlots, slotCount);
		AppendEquipmentSlotsSorted(OutSlots, slotCount);
	}
	else
	{
		AppendEquipmentSlotsRaw(OutSlots, slotCount);
		AppendPotionSlots(OutSlots, slotCount);
	}
}

void ULobbyHUD::ClickInventorySortBtn()
{
	bInventorySortedView = !bInventorySortedView;

	RefreshInventoryUI();

}

void ULobbyHUD::AppendPotionSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const
{
	if (InSlotLimit <= 0) return;
	if (OutSlots.Num() >= InSlotLimit) return;


	TArray<FGameplayTag> potionTags;
	CachedPlayerData.StackCounts.GetKeys(potionTags);

	potionTags.Sort([](const FGameplayTag& InA, const FGameplayTag& InB)
		{
			return InA.GetTagName().LexicalLess(InB.GetTagName());
		});

	for (const FGameplayTag& potionTag : potionTags)
	{
		int32 maxStack = 1;
		FindMaxStackByItemTag(potionTag, maxStack);
		maxStack = FMath::Max(1, maxStack);

		if (OutSlots.Num() >= InSlotLimit) break;

		const int32 totalCount = CachedPlayerData.StackCounts.FindRef(potionTag);
		if (totalCount <= 0) continue;

		const FDTPotionInfoRow* potionRow = FindPotionRowByTag(potionTag);
		if (!potionRow) continue;

		int32 remaining = totalCount;

		while (remaining > 0 && OutSlots.Num() < InSlotLimit)
		{
			FInventoryViewSlot slot;
			slot.Type = EInventoryViewSlotType::StackItem;
			slot.ItemTag = potionTag;

			const int32 stack = FMath::Min(maxStack, remaining);
			slot.StackCount = stack;

			slot.Icon = potionRow->Icon;
			slot.UpgradeLevel = 0;

			OutSlots.Add(MoveTemp(slot));
			remaining -= stack;
		}
	}
}

void ULobbyHUD::AppendEquipmentSlotsRaw(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const
{
	if (InSlotLimit <= 0) return;
	if (OutSlots.Num() >= InSlotLimit) return;

	for (int32 i = 0; i < CachedPlayerData.Inventory.Num(); i++)
	{
		if (OutSlots.Num() >= InSlotLimit) break;

		const FEquipmentInfo& equip = CachedPlayerData.Inventory[i];

		if (equip.ItemGuid.IsValid())
		{
			if (const FDTEquipmentInfoRow* equipRow = FindEquipmentRowByTag(equip.ItemTag))
			{
				FInventoryViewSlot slot;
				slot.Type = EInventoryViewSlotType::Equipment;
				slot.ItemGuid = equip.ItemGuid;
				slot.UpgradeLevel = equip.CurrUpgradeLevel;

				slot.Icon = equipRow->Icon;

				OutSlots.Add(MoveTemp(slot));
			}
		}
	}
}

void ULobbyHUD::AppendEquipmentSlotsSorted(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const
{
	if (InSlotLimit <= 0) return;
	if (OutSlots.Num() >= InSlotLimit) return;

	// 정렬에 필요한 정보 (장비 + DT로우 + 슬롯 우선순위)
	struct FEquipSortItem
	{
		const FEquipmentInfo* Equip = nullptr;
		const FDTEquipmentInfoRow* Row = nullptr;
		int32 SlotOrder = 999;
	};

	// 캐시된 인벤토리에서 정렬 가능한 장비만 추출
	TArray<FEquipSortItem> items;
	items.Reserve(CachedPlayerData.Inventory.Num());

	for (int32 i = 0; i < CachedPlayerData.Inventory.Num(); i++)
	{
		const FEquipmentInfo& equip = CachedPlayerData.Inventory[i];

		if (equip.ItemGuid.IsValid())
		{
			if (const FDTEquipmentInfoRow* equipRow = FindEquipmentRowByTag(equip.ItemTag))
			{
				const int32 order = GetSlotOrderFromRuleTable(equipRow->SlotTag);

				// 정의된 슬롯만 정렬 후보에 추가
				if (order != 999)
				{
					FEquipSortItem item;
					item.Equip = &equip;
					item.Row = equipRow;
					item.SlotOrder = order;

					items.Add(item);
				}
			}
		}
	}

	// 정렬 기준: (1)슬롯 우선순위 -> (2)강화 내림차순 -> (3)태그명 사전순
	items.Sort([](const FEquipSortItem& InA, const FEquipSortItem& InB)
		{
			if (InA.SlotOrder != InB.SlotOrder)
			{
				return InA.SlotOrder < InB.SlotOrder;
			}

			const int32 aUp = InA.Equip ? InA.Equip->CurrUpgradeLevel : 0;
			const int32 bUp = InB.Equip ? InB.Equip->CurrUpgradeLevel : 0;

			if (aUp != bUp)
			{
				// 같은 슬롯이면 강화 높은 게 먼저
				return aUp > bUp;
			}

			const FGameplayTag aTag = InA.Equip ? InA.Equip->ItemTag : FGameplayTag();
			const FGameplayTag bTag = InB.Equip ? InB.Equip->ItemTag : FGameplayTag();

			return aTag.GetTagName().LexicalLess(bTag.GetTagName());
		});

	// 정렬된 결과를 UI 슬롯 구조체로 변환해서 OutSlots에 채움
	for (int32 i = 0; i < items.Num(); i++)
	{
		if (OutSlots.Num() >= InSlotLimit) break;

		if (const FEquipmentInfo* equip = items[i].Equip)
		{
			FInventoryViewSlot slot;
			slot.Type = EInventoryViewSlotType::Equipment;
			slot.ItemGuid = equip->ItemGuid;
			slot.UpgradeLevel = equip->CurrUpgradeLevel;

			// 로우 데이터가 유효할 때만 아이콘 할당
			if (const FDTEquipmentInfoRow* row = items[i].Row)
			{
				slot.Icon = row->Icon;
			}

			OutSlots.Add(MoveTemp(slot));
		}
	}
}

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

void ULobbyHUD::TryPurchaseSelectedItem(int32 InSlotIndex)
{
	if (FPlayerAccountService::PurchaseShopSlot(this, InSlotIndex))
	{
		if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
		{
			gameInstance->OnCurrencyChanged.Broadcast();
		}

		RefreshShopUI();
	}
}

void ULobbyHUD::TrySellSelectedItem()
{
	if (SelectedInventoryItemGuid.IsValid())
	{
		if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
		{
			// TODO: 나중에 서비스 만들면 여기 연결
			// 예: FPlayerAccountService::SellEquipmentByGuid(gameInstance, SelectedInventoryItemGuid);

			SelectedInventoryItemGuid.Invalidate();
		}
	}
}

void ULobbyHUD::BuildShopRuntimeCache()
{
	const int32 equipmentSlotCount = FMath::Max(0, EquipmentShopSlotCount);
	const int32 potionSlotCount = FMath::Max(0, PotionShopSlotCount);
	const int32 slotCount = EquipmentShopSlotCount + PotionShopSlotCount;

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
		if (!gameInstance->CurrentShopKeys.IsValidIndex(slotIndex))	continue;

		const FShopProductKey& key = gameInstance->CurrentShopKeys[slotIndex];

		if (gameInstance->CurrentShopSoldOutStates.IsValidIndex(slotIndex))
		{
			CachedShopSoldOutStates[slotIndex] = gameInstance->CurrentShopSoldOutStates[slotIndex];
		}

		if (!key.TableTag.IsValid() || key.RowName.IsNone()) continue;

		CachedShopRowNames[slotIndex] = key.RowName;

		/* 포션 */
		if (key.TableTag.MatchesTagExact(Arcanum::DataTable::Potion))
		{
			const FDTPotionInfoRow* potionRow = dataSubsystem->GetRow<FDTPotionInfoRow>(Arcanum::DataTable::Potion, key.RowName);
			if (!potionRow) continue;

			CachedShopIcons[slotIndex] = potionRow->Icon;

			CachedShopNames[slotIndex] = FText::FromName(key.RowName);

			CachedShopDescs[slotIndex] = potionRow->Desc;
			CachedShopPrices[slotIndex] = potionRow->BuyPrice;

			CachedShopSoldOutStates[slotIndex] = false;

			continue;
		}

		/* 장비 */
		if (key.TableTag.MatchesTagExact(Arcanum::DataTable::Equipment))
		{
			const FDTEquipmentInfoRow* equipRow = dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, key.RowName);
			if (!equipRow) continue;

			CachedShopIcons[slotIndex] = equipRow->Icon;
			CachedShopNames[slotIndex] = FText::FromName(key.RowName);
			CachedShopDescs[slotIndex] = equipRow->Desc;
			CachedShopPrices[slotIndex] = equipRow->BuyPrice;

			continue;
		}
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
