#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/ItemData/Potion/DTPotionInfoRow.h"
#include "DataInfo/InventoryData/DataTable/DTInventoryRuleItem.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "Kismet/GameplayStatics.h"

// ========================================================
// 인터페이스(추후 서버 대비용 예시)
// ========================================================
void FPlayerAccountService::GetIPlayerDataCopy(const UObject* WorldContextObject, TFunction<void(bool)> OptionalCallback)
{
	//TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	//Request->OnProcessRequestComplete().BindLambda([this, OptionalCallback](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bConnectedSuccessfully) {
	//		if (bConnectedSuccessfully && Res.IsValid()) {
	//			FPlayerData NewData;
	//			PlayerDataReceivedDelegate.Broadcast(NewData);
	//			if (OptionalCallback) OptionalCallback(true);
	//		}
	//		else if (OptionalCallback) OptionalCallback(false);
	//	});

	//Request->SetURL(TEXT("서버 URL"));
	//Request->SetVerb(TEXT("GET"));
	//Request->ProcessRequest();
}
// ========================================================
// PlayerData Getter
// ========================================================
const FPlayerData FPlayerAccountService::GetPlayerDataCopy(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("Invalid WorldContext or GameInstance!"));
		return FPlayerData();
	}

	return GI->GetPlayerDataCopy();
}
const FPlayerCurrency FPlayerAccountService::GetPlayerCurrency(const UObject* WorldContextObject)
{
	return GetPlayerDataCopy(WorldContextObject).PlayerCurrency;
}
const FPlayerBattleData FPlayerAccountService::GetPlayerBattleData(const UObject* WorldContextObject)
{
	return GetPlayerDataCopy(WorldContextObject).PlayerBattleData;
}
const TArray<FBattleCharacterData> FPlayerAccountService::GetOwnedCharacters(const UObject* WorldContextObject)
{
	return GetPlayerDataCopy(WorldContextObject).OwnedCharacters;
}
const TArray<FEquipmentInfo> FPlayerAccountService::GetInventory(const UObject* WorldContextObject)
{
	return GetPlayerDataCopy(WorldContextObject).Inventory;
}
const TMap<FGameplayTag, FStageProgressData> FPlayerAccountService::GetStageProgressMap(const UObject* WorldContextObject)
{
	return GetPlayerDataCopy(WorldContextObject).StageProgressMap;
}
const FGachaData FPlayerAccountService::GetGachaState(const UObject* WorldContextObject)
{
	return GetPlayerDataCopy(WorldContextObject).GachaState;
}
const FPlayerQuest FPlayerAccountService::GetQuestState(const UObject* WorldContextObject)
{
	return GetPlayerDataCopy(WorldContextObject).QuestState;
}
// ========================================================
// PlayerData Updater
// ========================================================
const FPlayerCurrency FPlayerAccountService::UpdateCurrency(const UObject* WorldContextObject, const FPlayerData& PlayerData, FGameplayTag Tag, int64 Amount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI || !Tag.IsValid() || Amount == 0) return GetPlayerCurrency(WorldContextObject);
	if (!VerifyCurrency(GI, PlayerData)) return GetPlayerCurrency(WorldContextObject);

	GI->AddCurrency(Tag, Amount);
	GI->SavePlayerData();

	GI->OnCurrencyChanged.Broadcast();

	return GI->GetPlayerDataCopy().PlayerCurrency;
}
bool FPlayerAccountService::VerifyCurrency(UARGameInstance* GI, FPlayerData CachedData)
{
	if (!GI) return false;
	const FPlayerData& RealTimeData = GI->GetPlayerData();

	if (!(RealTimeData == CachedData)) {
		UE_LOG(LogTemp, Fatal, TEXT("Memory Manipulation Detected! Client Data Mismatch."));
		VerifiedFailure(GI);
		return false;
	}

	return true;
}
void FPlayerAccountService::VerifiedFailure(UARGameInstance* GI)
{
	if (!GI) return;
	GI->DeletePlayerData();
	FGenericPlatformMisc::RequestExit(true);
}
// ========================================================
// 로그인 관련
// ========================================================
bool FPlayerAccountService::LoadPlayerData(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("LoadPlayerData: GameInstance not found!"));
		return false;
	}

	return GI->LoadPlayerData();
}
bool FPlayerAccountService::SavePlayerData(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("SavePlayerData: GameInstance not found!"));
		return false;
	}

	return GI->SavePlayerData();
}
// ========================================================
// Battle Widget 관련
// ========================================================
void FPlayerAccountService::StopShopOnBattleStart(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return;

	GI->PausedShopRemainingSeconds = FPlayerAccountService::GetShopRemainingSeconds(WorldContextObject);
	GI->bShopPaused = true;

	if (UGameTimeSubsystem* gameTimeSubsystem = GI->GetSubsystem<UGameTimeSubsystem>())
	{
		gameTimeSubsystem->StopShop();
	}
}
// ========================================================
// Character Widget 관련
// ========================================================

// ========================================================
// Enhancement Widget 관련
// ========================================================

// ========================================================
// Shop Widget 관련
// ========================================================
TArray<FName> FPlayerAccountService::GetEquipmentList(const UObject* WorldContextObject)
{
	TArray<FName> Res;
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return Res;

	UGameDataSubsystem* DataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!DataSubsystem) return Res;

	UDataTable** TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment);
	if (!TablePtr || !(*TablePtr)) return Res;

	UDataTable* Table = *TablePtr;
	Res = Table->GetRowNames();

	return Res;


	//TArray<FName> Res;
	//if (!GameInstance) return Res;

	//UGameDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
	//if (!DataSubsystem) return Res;

	//UDataTable** TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment);
	//if (!TablePtr) return Res;

	//UDataTable* Table = *TablePtr;
	//for (const auto& Pair : Table->GetRowMap()) {
	//	Res.Add(Pair.Key);
	//}

	//return Res;
}
bool FPlayerAccountService::PurchaseEquipment(const UObject* WorldContextObject, FName RowName)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	UGameDataSubsystem* DataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!DataSubsystem) return false;

	FPlayerData& PlayerData = GI->GetPlayerData();

	const FDTEquipmentInfoRow* Row = DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, RowName);
	if (!Row || Row->BaseInfoSteps.IsEmpty()) return false;

	if (!Row->ItemTag.IsValid()) return false;

	const FDTItemCatalogRow* CatalogRow =
		DataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, Row->ItemTag.GetTagName());
	if (!CatalogRow) return false;

	FCurrencyData* CurrencyData = PlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!CurrencyData) return false;

	const int64 Price = static_cast<int64>(CatalogRow->BuyPrice);
	if (CurrencyData->CurrAmount < Price) {
		UE_LOG(LogTemp, Warning, TEXT("Not enough gold to buy %s"), *RowName.ToString());
		return false;
	}

	FEquipmentInfo NewEquip;
	NewEquip.ItemTag = Row->ItemTag;
	NewEquip.ItemGuid = FGuid::NewGuid();
	NewEquip.CurrUpgradeLevel = 0;
	NewEquip.Equipment = Row->BaseInfoSteps[0];

	// 인벤 용량 체크: 장비+스택 점유 칸으로 체크
	const FDTInventoryRuleItem* ruleRow = GetInventoryRuleRow(WorldContextObject);
	const int32 maxSlots = ruleRow ? FMath::Max(0, ruleRow->Capacity) : 0;

	// 장비 점유 칸
	int32 usedEquipSlots = 0;
	for (int32 i = 0; i < PlayerData.Inventory.Num(); i++)
	{
		if (PlayerData.Inventory[i].ItemGuid.IsValid())
		{
			usedEquipSlots++;
		}
	}

	// 스택 점유 칸
	int32 usedStackSlots = 0;
	for (const TPair<FGameplayTag, int32>& pair : PlayerData.StackCounts)
	{
		const FGameplayTag itemTag = pair.Key;
		const int32 count = pair.Value;
		if (!itemTag.IsValid() || count <= 0) continue;

		const FDTItemCatalogRow* stackCatalog = DataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, itemTag.GetTagName());
		if (!stackCatalog) continue;

		const int32 maxStack = FMath::Max(1, stackCatalog->MaxStack);
		usedStackSlots += (count + maxStack - 1) / maxStack;
	}

	// 총합이 용량이면 더 못 들어가게 막음
	if ((usedEquipSlots + usedStackSlots) >= maxSlots) return false;

	PlayerData.Inventory.Add(MoveTemp(NewEquip));
	CurrencyData->CurrAmount -= Price;

	GI->SavePlayerData();

	return true;



	//if (!GameInstance) return false;

	//UGameDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
	//if (!DataSubsystem) return false;

	//FPlayerData& PlayerData = GameInstance->GetPlayerData();
	//const FDTEquipmentInfoRow* Row = DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, RowName);

	//if (!Row) return false;
	//if (Row->BaseInfoSteps.IsEmpty()) return false;

	//FCurrencyData* CurrencyData = PlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);

	//if (!CurrencyData) return false;

	//const int64 Price = static_cast<int64>(Row->BuyPrice);
	//if (CurrencyData->CurrAmount < Price) return false;

	//FEquipmentInfo NewEquip;
	//NewEquip.ItemTag = Row->ItemTag;
	//NewEquip.ItemGuid = FGuid::NewGuid();
	//NewEquip.CurrUpgradeLevel = 0;
	//NewEquip.Equipment = Row->BaseInfoSteps[0];
	//PlayerData.Inventory.Add(MoveTemp(NewEquip));
	//CurrencyData->CurrAmount -= Price;

	//GameInstance->SavePlayerData();

	//return true;
}

bool FPlayerAccountService::SellItemByGuid(const UObject* WorldContextObject, const FGuid& InItemGuid)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	if (!InItemGuid.IsValid()) return false;

	UGameDataSubsystem* dataSubsytem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsytem) return false;

	// PlayerData에서 GUID로 장비 찾기
	FPlayerData& playerData = GI->GetPlayerData();

	int32 foundIndex = INDEX_NONE;
	for (int32 i = 0; i < playerData.Inventory.Num(); i++)
	{
		if (playerData.Inventory[i].ItemGuid == InItemGuid)
		{
			foundIndex = i;
			break;
		}
	}
	if (foundIndex == INDEX_NONE) return false;

	const FEquipmentInfo& equipment = playerData.Inventory[foundIndex];

	const FDTItemCatalogRow* catalogRow = dataSubsytem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, equipment.ItemTag.GetTagName());
	if (!catalogRow) return false;

	const int64 sellPrice = static_cast<int64>(catalogRow->SellPrice);
	if (sellPrice < 0) return false;

	// 골드 추가
	FCurrencyData* gold = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!gold) return false;

	gold->CurrAmount += sellPrice;
	if (gold->MaxAmount > 0)
	{
		gold->CurrAmount = FMath::Clamp(gold->CurrAmount, int64(0), gold->MaxAmount);
	}

	// 인벤엔서 제거
	playerData.Inventory.RemoveAtSwap(foundIndex, 1, EAllowShrinking::No);

	// 저장
	GI->SavePlayerData();

	return true;
}

bool FPlayerAccountService::SellStackItemByTag(const UObject* WorldContextObject, const FGameplayTag& InItemTag, int32 InSellCount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	if (!InItemTag.IsValid() || InSellCount <= 0) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	// 보유 수량 확인
	int32* currentCountPtr = playerData.StackCounts.Find(InItemTag);
	if (!currentCountPtr || *currentCountPtr < InSellCount) return false;

	// 판매가
	const FDTItemCatalogRow* catalogRow =
		dataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, InItemTag.GetTagName());
	if (!catalogRow) return false;

	const int64 sellPricePerOne = static_cast<int64>(catalogRow->SellPrice);
	if (sellPricePerOne < 0) return false;

	// 골드 추가
	FCurrencyData* gold = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!gold) return false;

	const int64 totalSellGold = sellPricePerOne * static_cast<int64>(InSellCount);
	gold->CurrAmount += totalSellGold;
	if (gold->MaxAmount > 0)
	{
		gold->CurrAmount = FMath::Clamp(gold->CurrAmount, int64(0), gold->MaxAmount);
	}

	// 스택 차감 (0이면 제거)
	*currentCountPtr -= InSellCount;
	if (*currentCountPtr <= 0)
	{
		playerData.StackCounts.Remove(InItemTag);
	}

	// 저장
	GI->SavePlayerData();

	return true;
}

const FDTEquipmentInfoRow* FPlayerAccountService::GetItemDefinition(UGameDataSubsystem* DataSubsystem, const FGameplayTag& ItemTag)
{
	if (!DataSubsystem) return nullptr;
	return DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, ItemTag.GetTagName());
}

void FPlayerAccountService::InitializeShop(const UObject* WorldContextObject, int32 InEquipmentSlotCount, int32 InPotionSlotCount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return;

	const int32 equipmentCount = FMath::Max(0, InEquipmentSlotCount);
	const int32 potionCount = FMath::Max(0, InPotionSlotCount);
	const int32 totalCount = equipmentCount + potionCount;

	if (totalCount > 0)
	{
		const bool bDataValid =
			(GI->CurrentShopKeys.Num() == totalCount) &&
			(GI->CurrentShopSoldOutStates.Num() == totalCount) &&
			(GI->NextShopRefreshTime.GetTicks() > 0);

		if (!bDataValid || IsShopRefreshExpired(WorldContextObject))
		{
			RefreshShop(WorldContextObject, equipmentCount, potionCount);
		}

		if (UGameTimeSubsystem* gameTimeSubsystem = GI->GetSubsystem<UGameTimeSubsystem>())
		{
			gameTimeSubsystem->StartShop(GI->NextShopRefreshTime);
		}
	}
}

void FPlayerAccountService::RefreshShop(const UObject* WorldContextObject, int32 InEquipmentSlotCount, int32 InPotionSlotCount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return;

	const int32 equipmentCount = FMath::Max(0, InEquipmentSlotCount);
	const int32 potionCount = FMath::Max(0, InPotionSlotCount);
	const int32 totalCount = equipmentCount + potionCount;

	if (totalCount > 0)
	{
		GI->CurrentShopKeys.SetNum(totalCount);
		GI->CurrentShopSoldOutStates.SetNum(totalCount);

		/* 장비만 재생성, 물약은 유지 */
		GenerateShopItems(GI, equipmentCount, potionCount, true);

		/* 장비 구간만 품절 리셋 */
		ResetShopSoldOutStates(GI, equipmentCount);

		/* 다음 갱신 시각 저장 */
		SetNextShopRefreshTime(GI);

		/* 상점 타이머 시작 */
		if (UGameTimeSubsystem* gameTimeSubsystem = GI->GetSubsystem<UGameTimeSubsystem>())
		{
			gameTimeSubsystem->StartShop(GI->NextShopRefreshTime);
		}
	}
}

bool FPlayerAccountService::GetShopSlotData(const UObject* WorldContextObject, int32 InSlotIndex, FGameplayTag& OutTableTag, FName& OutRowName, bool& OutSoldOut)
{
	OutTableTag = FGameplayTag();
	OutRowName = NAME_None;
	OutSoldOut = false;

	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	if (!GI->CurrentShopKeys.IsValidIndex(InSlotIndex) ||
		!GI->CurrentShopSoldOutStates.IsValidIndex(InSlotIndex))
	{
		return false;
	}

	const FShopProductKey& key = GI->CurrentShopKeys[InSlotIndex];

	OutTableTag = key.TableTag;
	OutRowName = key.RowName;
	OutSoldOut = GI->CurrentShopSoldOutStates[InSlotIndex];

	return true;
}

bool FPlayerAccountService::PurchaseShopSlot(const UObject* InWorldContextObject, int32 InSlotIndex)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(InWorldContextObject));
	if (!GI) return false;

	if (!GI->CurrentShopKeys.IsValidIndex(InSlotIndex) ||
		!GI->CurrentShopSoldOutStates.IsValidIndex(InSlotIndex)) return false;

	if (GI->CurrentShopSoldOutStates[InSlotIndex]) return false;

	const FShopProductKey& key = GI->CurrentShopKeys[InSlotIndex];
	if (!key.TableTag.IsValid() || key.RowName.IsNone()) return false;

	if (!key.TableTag.MatchesTagExact(Arcanum::DataTable::ItemCatalog)) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FDTItemCatalogRow* catalogRow =
		dataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, key.RowName);
	if (!catalogRow) return false;

	if (!catalogRow->ItemTag.IsValid() || !catalogRow->StorePolicyTag.IsValid()) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	FCurrencyData* gold = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!gold) return false;

	const int64 price = static_cast<int64>(catalogRow->BuyPrice);
	if (gold->CurrAmount < price) return false;

	if (catalogRow->StorePolicyTag.MatchesTagExact(Arcanum::Inventory::Store::Stack))
	{
		// 인벤 용량만 InventoryRule에서 가져옴
		const FDTInventoryRuleItem* ruleRow = GetInventoryRuleRow(InWorldContextObject);
		const int32 maxSlots = ruleRow ? FMath::Max(0, ruleRow->Capacity) : 0;

		// 장비 점유 칸(유효 Guid)
		int32 usedEquipSlots = 0;
		for (int32 i = 0; i < playerData.Inventory.Num(); i++)
		{
			if (playerData.Inventory[i].ItemGuid.IsValid())
			{
				usedEquipSlots++;
			}
		}

		// 현재 스택 점유 칸(카탈로그 MaxStack 기준)
		int32 currentStackSlots = 0;
		for (const TPair<FGameplayTag, int32>& pair : playerData.StackCounts)
		{
			const FGameplayTag itemTag = pair.Key;
			const int32 count = pair.Value;
			if (!itemTag.IsValid() || count <= 0) continue;

			const FDTItemCatalogRow* stackCatalog =
				dataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, itemTag.GetTagName());
			if (!stackCatalog) continue;

			const int32 maxStack = FMath::Max(1, stackCatalog->MaxStack);
			currentStackSlots += (count + maxStack - 1) / maxStack;
		}

		// 이번 아이템 추가로 필요한 슬롯
		const int32 maxStackForThis = FMath::Max(1, catalogRow->MaxStack);

		const int32 oldCount = playerData.StackCounts.FindRef(catalogRow->ItemTag);
		const int32 newCount = oldCount + 1;

		const int32 oldSlotsForThis = (oldCount > 0) ? ((oldCount + maxStackForThis - 1) / maxStackForThis) : 0;
		const int32 newSlotsForThis = (newCount > 0) ? ((newCount + maxStackForThis - 1) / maxStackForThis) : 0;
		const int32 extraSlotsNeeded = newSlotsForThis - oldSlotsForThis;

		if ((usedEquipSlots + currentStackSlots + extraSlotsNeeded) > maxSlots) return false;

		playerData.StackCounts.FindOrAdd(catalogRow->ItemTag) = newCount;

		gold->CurrAmount -= price;

		GI->SavePlayerData();
		return true;
	}

	// Guid: 장비
	if (catalogRow->StorePolicyTag.MatchesTagExact(Arcanum::Inventory::Store::Guid))
	{
		if (!AddGuidByCatalog(InWorldContextObject, catalogRow)) return false;

		gold->CurrAmount -= price;

		GI->SavePlayerData();

		return SetShopSlotSoldOut(InWorldContextObject, InSlotIndex);
	}

	return false;
}

bool FPlayerAccountService::SetShopSlotSoldOut(const UObject* WorldContextObject, int32 InSlotIndex)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	if (GI->CurrentShopSoldOutStates.IsValidIndex(InSlotIndex))
	{
		GI->CurrentShopSoldOutStates[InSlotIndex] = true;

		return true;
	}

	return false;
}

int32 FPlayerAccountService::GetShopRemainingSeconds(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return 0;

	if (GI->NextShopRefreshTime.GetTicks() > 0)
	{
		const FDateTime currentTimeKST = GetCurrentTimeKST();
		const FTimespan difference = GI->NextShopRefreshTime - currentTimeKST;

		return FMath::Max(0, static_cast<int32>(difference.GetTotalSeconds()));
	}

	return 0;
}

// 포션 보유 수량 조회
int32 FPlayerAccountService::GetStackItemCountByTag(const UObject* WorldContextObject, const FGameplayTag& InItemTag)
{
	if (UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		if (InItemTag.IsValid())
		{
			const FPlayerData& playerData = GI->GetPlayerDataCopy();

			if (const int32* foundCount = playerData.StackCounts.Find(InItemTag))
			{
				return *foundCount;
			}
		}
	}

	return 0;
}

// 포션 구매(스택형) - 구매 성공 시 수량 증가(최대 20스택)
bool FPlayerAccountService::PurchaseStackItemByRowName(const UObject* InWorldContextObject, FName InCatalogRowName, int32 InBuyCount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(InWorldContextObject));
	if (!GI) return false;

	if (InCatalogRowName.IsNone() || InBuyCount <= 0) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FDTItemCatalogRow* catalogRow =
		dataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, InCatalogRowName);
	if (!catalogRow) return false;

	if (!catalogRow->ItemTag.IsValid()) return false;
	if (!catalogRow->StorePolicyTag.MatchesTagExact(Arcanum::Inventory::Store::Stack)) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	FCurrencyData* gold = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!gold) return false;

	const int64 totalPrice = static_cast<int64>(catalogRow->BuyPrice) * static_cast<int64>(InBuyCount);
	if (gold->CurrAmount < totalPrice) return false;

	// 용량(Capacity)만 InventoryRule DT에서
	const FDTInventoryRuleItem* ruleRow = GetInventoryRuleRow(InWorldContextObject);
	const int32 maxSlots = ruleRow ? FMath::Max(0, ruleRow->Capacity) : 0;

	// 장비 점유 칸(유효 Guid)
	int32 usedEquipSlots = 0;
	for (int32 i = 0; i < playerData.Inventory.Num(); i++)
	{
		if (playerData.Inventory[i].ItemGuid.IsValid())
		{
			usedEquipSlots++;
		}
	}

	// 현재 스택 점유 칸(카탈로그 MaxStack 기준)
	int32 currentStackSlots = 0;
	for (const TPair<FGameplayTag, int32>& pair : playerData.StackCounts)
	{
		const FGameplayTag itemTag = pair.Key;
		const int32 count = pair.Value;
		if (!itemTag.IsValid() || count <= 0) continue;

		const FDTItemCatalogRow* stackCatalog =
			dataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, itemTag.GetTagName());
		if (!stackCatalog) continue;

		const int32 maxStack = FMath::Max(1, stackCatalog->MaxStack);
		currentStackSlots += (count + maxStack - 1) / maxStack;
	}

	// 이번 아이템 추가로 필요한 슬롯 계산
	const int32 maxStackForThis = FMath::Max(1, catalogRow->MaxStack);

	const int32 oldCount = playerData.StackCounts.FindRef(catalogRow->ItemTag);
	const int32 newCount = oldCount + InBuyCount;

	const int32 oldSlotsForThis = (oldCount > 0) ? ((oldCount + maxStackForThis - 1) / maxStackForThis) : 0;
	const int32 newSlotsForThis = (newCount > 0) ? ((newCount + maxStackForThis - 1) / maxStackForThis) : 0;
	const int32 extraSlotsNeeded = newSlotsForThis - oldSlotsForThis;

	if ((usedEquipSlots + currentStackSlots + extraSlotsNeeded) > maxSlots) return false;

	playerData.StackCounts.FindOrAdd(catalogRow->ItemTag) = newCount;

	gold->CurrAmount -= totalPrice;

	GI->SavePlayerData();

	return true;
}

bool FPlayerAccountService::IsShopRefreshExpired(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return true;

	if (GI->NextShopRefreshTime.GetTicks() > 0)
	{
		const FDateTime currentTimeKST = GetCurrentTimeKST();

		return (currentTimeKST >= GI->NextShopRefreshTime);
	}

	return true;
}

void FPlayerAccountService::GenerateShopItems(UARGameInstance* InGameInstance, int32 InEquipmentSlotCount, int32 InPotionSlotCount, bool bInRefreshEquipmentOnly)
{
	if (!InGameInstance) return;

	const int32 equipmentCount = FMath::Max(0, InEquipmentSlotCount);
	const int32 potionCount = FMath::Max(0, InPotionSlotCount);
	const int32 totalCount = equipmentCount + potionCount;
	if (totalCount <= 0) return;

	// Guid 풀
	TArray<FGameplayTag> guidItemTags;
	BuildCatalogItemTagsByStorePolicy(InGameInstance, Arcanum::Inventory::Store::Guid, guidItemTags);

	// Stack 풀
	TArray<FGameplayTag> stackItemTags;
	BuildCatalogItemTagsByStorePolicy(InGameInstance, Arcanum::Inventory::Store::Stack, stackItemTags);

	// 장비 구간 갱신
	for (int32 i = 0; i < equipmentCount; i++)
	{
		if (InGameInstance->CurrentShopKeys.IsValidIndex(i))
		{
			FShopProductKey key;
			key.TableTag = Arcanum::DataTable::ItemCatalog;
			key.RowName = PickCatalogRowNameFromTags(guidItemTags);

			InGameInstance->CurrentShopKeys[i] = key;
		}
	}

	// 스택 구간 고정/유지 옵션 처리
	for (int32 j = 0; j < potionCount; j++)
	{
		const int32 dstIndex = equipmentCount + j;
		if (InGameInstance->CurrentShopKeys.IsValidIndex(dstIndex))
		{
			if (bInRefreshEquipmentOnly)
			{
				const FShopProductKey& existing = InGameInstance->CurrentShopKeys[dstIndex];
				if (existing.TableTag.IsValid() && !existing.RowName.IsNone()) continue;
			}
		}

		FShopProductKey key;
		key.TableTag = Arcanum::DataTable::ItemCatalog;
		key.RowName = PickCatalogRowNameFromTags(stackItemTags);

		InGameInstance->CurrentShopKeys[dstIndex] = key;
	}
}

void FPlayerAccountService::BuildShopItemPools(UARGameInstance* InGameInstance, FShopItemPools& OutItemPools)
{
	OutItemPools.CommonRows.Reset();
	OutItemPools.SetRows.Reset();
	OutItemPools.LegendaryRows.Reset();

	if (InGameInstance)
	{
		if (UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>())
		{
			UDataTable* equipmentTable = nullptr;

			if (UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment))
			{
				equipmentTable = *tablePtr;
			}

			if (equipmentTable)
			{
				const FGameplayTag setRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Rarity.SetItem"), false);
				const FGameplayTag legendaryRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Rarity.Legendary"), false);

				for (const TPair<FName, uint8*>& rowPair : equipmentTable->GetRowMap())
				{
					const FDTEquipmentInfoRow* row = reinterpret_cast<const FDTEquipmentInfoRow*>(rowPair.Value);

					if (row && !row->BaseInfoSteps.IsEmpty() && row->ItemTag.IsValid())
					{
						if (legendaryRootTag.IsValid() && row->ItemTag.MatchesTag(legendaryRootTag))
						{
							OutItemPools.LegendaryRows.Add(rowPair.Key);
						}
						else if (setRootTag.IsValid() && row->ItemTag.MatchesTag(setRootTag))
						{
							OutItemPools.SetRows.Add(rowPair.Key);
						}
						else
						{
							OutItemPools.CommonRows.Add(rowPair.Key);
						}
					}
				}
			}
		}
	}
}

EShopRarityType FPlayerAccountService::PickShopRarityType(UARGameInstance* InGameInstance)
{
	// TODO: 확률 DT 에서 가져올수도 있음
	const int32 legendaryChancePercent = 5;
	const int32 setChancePercent = 10;

	const int32 roll = FMath::RandRange(1, 100);

	if (roll <= legendaryChancePercent)
	{
		return EShopRarityType::Legendary;
	}

	if (roll <= (legendaryChancePercent + setChancePercent))
	{
		return EShopRarityType::Set;
	}

	return EShopRarityType::Common;
}

FName FPlayerAccountService::PickShopItemRow(EShopRarityType InRarityType, FShopItemPools& InOutItemPools)
{
	TArray<FName>* targetRows = nullptr;

	if (InRarityType == EShopRarityType::Legendary)
	{
		targetRows = &InOutItemPools.LegendaryRows;
	}
	else if (InRarityType == EShopRarityType::Set)
	{
		targetRows = &InOutItemPools.SetRows;
	}
	else
	{
		targetRows = &InOutItemPools.CommonRows;
	}

	if (!targetRows || targetRows->Num() <= 0)
	{
		return NAME_None;
	}

	const int32 randomIndex = FMath::RandRange(0, targetRows->Num() - 1);
	const FName pickedRowName = (*targetRows)[randomIndex];

	targetRows->RemoveAt(randomIndex);

	return pickedRowName;
}


void FPlayerAccountService::ResetShopSoldOutStates(UARGameInstance* InGameInstance, int32 InEquipmentSlotCount)
{
	if (!InGameInstance) return;

	const int32 equipmentCount = FMath::Max(0, InEquipmentSlotCount);

	for (int32 i = 0; i < equipmentCount; i++)
	{
		if (InGameInstance->CurrentShopSoldOutStates.IsValidIndex(i))
		{
			InGameInstance->CurrentShopSoldOutStates[i] = false;
		}
	}
}

void FPlayerAccountService::SetNextShopRefreshTime(UARGameInstance* InGameInstance)
{
	if (InGameInstance)
	{
		if (UGameTimeSubsystem* gameTimeSubsystem = InGameInstance->GetSubsystem<UGameTimeSubsystem>())
		{
			const FDateTime currentTimeKST = GetCurrentTimeKST();
			InGameInstance->NextShopRefreshTime = currentTimeKST + FTimespan(0, 0, gameTimeSubsystem->ShopRefreshSeconds);
		}
	}
}

const FDTInventoryRuleItem* FPlayerAccountService::GetInventoryRuleRow(const UObject* WorldContextObject)
{
	UARGameInstance* Gi = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!Gi) return nullptr;

	if (UGameDataSubsystem* dataSubsystem = Gi->GetSubsystem<UGameDataSubsystem>())
	{
		if (UDataTable* const* tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::InventoryRule))
		{
			if (*tablePtr)
			{
				return (*tablePtr)->FindRow<FDTInventoryRuleItem>(FName("Default"), TEXT("InventoryRule"));
			}
		}
	}

	return nullptr;
}

void FPlayerAccountService::BuildCatalogItemTagsByStorePolicy(UARGameInstance* InGameInstance, const FGameplayTag& InStorePolicyTag, TArray<FGameplayTag>& OutItemTags)
{
	OutItemTags.Reset();
	if (!InGameInstance || !InStorePolicyTag.IsValid()) return;

	UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::ItemCatalog);
	if (!tablePtr || !(*tablePtr)) return;

	UDataTable* table = *tablePtr;

	for (const TPair<FName, uint8*>& pair : table->GetRowMap())
	{
		if (const FDTItemCatalogRow* row = reinterpret_cast<const FDTItemCatalogRow*>(pair.Value))
		{
			if (row->ItemTag.IsValid())
			{
				if (row->StorePolicyTag.IsValid())
				{
					if (row->StorePolicyTag.MatchesTagExact(InStorePolicyTag))
					{
						OutItemTags.Add(row->ItemTag);
					}
				}
			}
		}
	}
}

FPlayerAccountService::FAddGuidHandler
FPlayerAccountService::FindGuidAddHandler(const FGameplayTag& InDetailTableTag)
{ 
	if (InDetailTableTag.MatchesTagExact(Arcanum::DataTable::Equipment))
	{
		return &FPlayerAccountService::AddGuidFromEquipment;
	}

	return nullptr;
}

bool FPlayerAccountService::AddGuidByCatalog(const UObject* WorldContextObject, const FDTItemCatalogRow* InCatalogRow)
{
	if (!InCatalogRow) return false;
	if (!InCatalogRow->StorePolicyTag.IsValid()) return false;

	if (!InCatalogRow->StorePolicyTag.MatchesTagExact(Arcanum::Inventory::Store::Guid))	return false;

	if (!InCatalogRow->DetailTableTag.IsValid() || InCatalogRow->DetailRowName.IsNone()) return false;
	
	const FAddGuidHandler handler = FindGuidAddHandler(InCatalogRow->DetailTableTag);
	if (!handler) return false;

	return handler(WorldContextObject, InCatalogRow);
}

bool FPlayerAccountService::AddGuidFromEquipment(const UObject* WorldContextObject, const FDTItemCatalogRow* InCatalogRow)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	if (!InCatalogRow) return false;
	if (InCatalogRow->DetailRowName.IsNone()) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FDTEquipmentInfoRow* equipRow =
		dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, InCatalogRow->DetailRowName);
	if (!equipRow || equipRow->BaseInfoSteps.IsEmpty() || !equipRow->ItemTag.IsValid()) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	FEquipmentInfo newEquip;
	newEquip.ItemTag = equipRow->ItemTag;
	newEquip.ItemGuid = FGuid::NewGuid();
	newEquip.CurrUpgradeLevel = 0;
	newEquip.Equipment = equipRow->BaseInfoSteps[0];

	playerData.Inventory.Add(MoveTemp(newEquip));
	return true;
}

FName FPlayerAccountService::PickCatalogRowNameFromTags(TArray<FGameplayTag>& InOutItemTags)
{
	if (InOutItemTags.Num() <= 0) return NAME_None;

	const int32 randomIndex = FMath::RandRange(0, InOutItemTags.Num() - 1);
	const FGameplayTag pickedTag = InOutItemTags[randomIndex];

	InOutItemTags.RemoveAtSwap(randomIndex, 1, EAllowShrinking::No);

	return pickedTag.GetTagName();
}

void FPlayerAccountService::FillShopStackItemSlots(UARGameInstance* InGameInstance, int32 InStartIndex, int32 InCount)
{
	if (!InGameInstance) return;
	if (InCount <= 0) return;

	TArray<FGameplayTag> stackItemTags;
	BuildCatalogItemTagsByStorePolicy(InGameInstance, Arcanum::Inventory::Store::Stack, stackItemTags);

	for (int32 j = 0; j < InCount; j++)
	{
		const int32 dstIndex = InStartIndex + j;

		if (!InGameInstance->CurrentShopKeys.IsValidIndex(dstIndex)) continue;

		FShopProductKey key;
		key.TableTag = Arcanum::DataTable::ItemCatalog;
		key.RowName = PickCatalogRowNameFromTags(stackItemTags);

		InGameInstance->CurrentShopKeys[dstIndex] = key;
	}
}

FDateTime FPlayerAccountService::GetCurrentTimeKST()
{
	return FDateTime::UtcNow() + FTimespan(9, 0, 0);
}

// ========================================================
// Gacha Widget 관련
// ========================================================
const FDTGachaBannerDataRow* FPlayerAccountService::GetGachaBannerData(const UObject* WorldContextObject, FGameplayTag TargetTag)
{
	if (!WorldContextObject) return nullptr;

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return nullptr;

	UGameInstance* GI = World->GetGameInstance();
	if (!GI) return nullptr;

	UGameDataSubsystem* DataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!DataSubsystem) return nullptr;

	UDataTable** TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::GachaTable);
	if (!TablePtr || !(*TablePtr)) return nullptr;

	TArray<FDTGachaBannerDataRow*> AllRows;
	(*TablePtr)->GetAllRows<FDTGachaBannerDataRow>(TEXT("GachaContext"), AllRows);

	for (const auto* Row : AllRows) {
		if (Row->BannerTag.MatchesTagExact(TargetTag)) return Row;
	}

	return nullptr;
}
void FPlayerAccountService::GetActiveGachaBannerRows(const UObject* WorldContextObject, TArray<const FDTGachaBannerDataRow*>& OutRows)
{
	OutRows.Empty();

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	UGameInstance* GI = World->GetGameInstance();
	if (!GI) return;

	UGameDataSubsystem* DataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!DataSubsystem) return;

	UDataTable** TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::GachaTable);
	if (!TablePtr || !(*TablePtr)) return;

	TArray<FDTGachaBannerDataRow*> AllRows;
	(*TablePtr)->GetAllRows<FDTGachaBannerDataRow>(TEXT("GachaContext"), AllRows);

	FDateTime Now = FDateTime::UtcNow();
	for (const auto* Row : AllRows) {
		if (Now >= Row->StartTime && Now <= Row->EndTime) OutRows.Add(Row);
	}

	OutRows.Sort([](const FDTGachaBannerDataRow& A, const FDTGachaBannerDataRow& B) { return A.DisplayPriority < B.DisplayPriority; });
}