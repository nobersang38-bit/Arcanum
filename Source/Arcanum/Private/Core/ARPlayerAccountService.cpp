#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "Core/SubSystem/GameDataSubsystem.h"

#include "DataInfo/ItemData/Potion/DTPotionInfoRow.h"
#include "DataInfo/InventoryData/DataTable/DTInventoryRuleItem.h"

#include "Kismet/GameplayStatics.h"

#pragma region 가챠 관련
#include "DataInfo/PlayerData/PlayerGacha/FPlayerGacha.h"
#pragma endregion


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
// 레벨 변경 시 호출 함수
// ========================================================
void FPlayerAccountService::SetHUDIndex(const UObject* WorldContextObject, const int HudIndex)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("Invalid WorldContext or GameInstance!"));
		return;
	}

	GI->HUDIndex = static_cast<EHUDIndex>(GI->HUDIndex);
}
void FPlayerAccountService::SetHUDIndex(const UObject* WorldContextObject, const EHUDIndex HudIndex)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("Invalid WorldContext or GameInstance!"));
		return;
	}

	GI->HUDIndex = HudIndex;
}
int32 FPlayerAccountService::GetHUDIndex(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("Invalid WorldContext or GameInstance!"));
		return 0;
	}

	return static_cast<int32>(GI->HUDIndex);
}
// ========================================================
// PlayerData Updater
// ========================================================
const bool FPlayerAccountService::AddCurrency(const UObject* WorldContextObject, FGameplayTag Tag, int64 Amount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI || !Tag.IsValid() || Amount == 0) return false;

	int64 CurrentOwned = GI->GetCurrencyAmount(Tag);
	if (Amount < 0 && CurrentOwned < FMath::Abs(Amount)) {
		UE_LOG(LogTemp, Warning, TEXT("UpdateCurrency Failed: Not enough currency."));
		return false;
	}

	GI->AddCurrency(Tag, Amount);
	GI->SavePlayerData();
	return true;
}
const FPlayerCurrency FPlayerAccountService::UpdateCurrency(const UObject* WorldContextObject, const FPlayerData& PlayerData, FGameplayTag Tag, int64 Amount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI || !Tag.IsValid() || Amount == 0) return GetPlayerCurrency(WorldContextObject);
	if (!VerifyCurrency(GI, PlayerData)) return GetPlayerCurrency(WorldContextObject);

	int64 CurrentOwned = GI->GetCurrencyAmount(Tag);
	if (Amount < 0 && CurrentOwned < FMath::Abs(Amount)) {
		UE_LOG(LogTemp, Warning, TEXT("UpdateCurrency Failed: Not enough currency."));
		return GI->GetPlayerDataCopy().PlayerCurrency;
	}

	GI->AddCurrency(Tag, Amount);
	GI->SavePlayerData();
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

	FCurrencyData* CurrencyData = PlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!CurrencyData) return false;

	const int64 Price = static_cast<int64>(Row->BuyPrice);
	if (CurrencyData->CurrAmount < Price) {
		UE_LOG(LogTemp, Warning, TEXT("Not enough gold to buy %s"), *RowName.ToString());
		return false;
	}

	FEquipmentInfo NewEquip;
	NewEquip.ItemTag = Row->ItemTag;
	NewEquip.ItemGuid = FGuid::NewGuid();
	NewEquip.CurrUpgradeLevel = 0;
	NewEquip.Equipment = Row->BaseInfoSteps[0];

	// 인벤 용량 체크: 장비+포션 점유 칸으로 체크
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

	// 포션 점유 칸 (스택 기준)
	int32 usedPotionSlots = 0;
	for (const TPair<FGameplayTag, int32>& pair : PlayerData.StackCounts)
	{
		const int32 count = pair.Value;
		if (count <= 0) continue;

		const int32 tagMaxStack = GetMaxStackByItemTag(ruleRow, pair.Key);
		usedPotionSlots += (count + tagMaxStack - 1) / tagMaxStack;
	}

	// 총합이 용량이면 더 못 들어가게 막음
	if ((usedEquipSlots + usedPotionSlots) >= maxSlots)	return false;

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
	return false;
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

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	// 포션: 품절 처리 없음
	if (key.TableTag.MatchesTagExact(Arcanum::DataTable::Potion))
	{
		return PurchasePotion(InWorldContextObject, key.RowName, 1);
	}

	// 장비: 구매 성공 시 품절 처리
	if (key.TableTag.MatchesTagExact(Arcanum::DataTable::Equipment))
	{
		if (PurchaseEquipment(InWorldContextObject, key.RowName))
		{
			return SetShopSlotSoldOut(InWorldContextObject, InSlotIndex);
		}

		return false;
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
int32 FPlayerAccountService::GetPotionCount(const UObject* WorldContextObject, const FGameplayTag& InPotionTag)
{
	if (UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject)))
	{
		if (InPotionTag.IsValid())
		{
			const FPlayerData& playerData = GI->GetPlayerDataCopy();

			if (const int32* foundCount = playerData.StackCounts.Find(InPotionTag))
			{
				return *foundCount;
			}
		}
	}

	return 0;
}

// 포션 구매(스택형) - 구매 성공 시 수량 증가(최대 20스택)
bool FPlayerAccountService::PurchasePotion(const UObject* InWorldContextObject, FName InPotionRowName, int32 InBuyCount)
{
	UARGameInstance* Gi = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(InWorldContextObject));
	if (!Gi) return false;

	if (InPotionRowName.IsNone() || InBuyCount <= 0) return false;

	UGameDataSubsystem* dataSubsystem = Gi->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FDTPotionInfoRow* row = dataSubsystem->GetRow<FDTPotionInfoRow>(Arcanum::DataTable::Potion, InPotionRowName);
	if (!row || !row->PotionTag.IsValid()) return false;

	FPlayerData& playerData = Gi->GetPlayerData();
	FCurrencyData* currencyData = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!currencyData) return false;

	const int64 totalPrice = static_cast<int64>(row->BuyPrice) * static_cast<int64>(InBuyCount);
	if (currencyData->CurrAmount < totalPrice) return false;

	const int32 oldCount = playerData.StackCounts.FindRef(row->PotionTag);
	const int64 oldGold = currencyData->CurrAmount;

	// 인벤 포션 용량 체크
	const FDTInventoryRuleItem* ruleRow = GetInventoryRuleRow(InWorldContextObject);
	const int32 maxSlots = ruleRow ? FMath::Max(0, ruleRow->Capacity) : 0;

	// 장비 점유 칸(유효 Guid 개수)
	int32 usedEquipSlots = 0;
	for (int32 i = 0; i < playerData.Inventory.Num(); i++)
	{
		if (playerData.Inventory[i].ItemGuid.IsValid())
		{
			usedEquipSlots++;
		}
	}

	// 포션 점유 칸
	const int32 newPotionCount = oldCount + InBuyCount;

	// 구매 직전 포션 슬롯 점유
	int32 CurrentPotionSlots = 0;
	for (const TPair<FGameplayTag, int32>& pair : playerData.StackCounts)
	{
		const int32 count = pair.Value;
		if (count <= 0) continue;

		const int32 tagMaxStack = GetMaxStackByItemTag(ruleRow, pair.Key);
		CurrentPotionSlots += (count + tagMaxStack - 1) / tagMaxStack;
	}

	// 이 포션의 MaxStack
	const int32 thisMaxStack = GetMaxStackByItemTag(ruleRow, row->PotionTag);

	const int32 oldSlotsForThisPotion = (oldCount > 0) ? ((oldCount + thisMaxStack - 1) / thisMaxStack) : 0;
	const int32 newSlotsForThisPotion = (newPotionCount > 0) ? ((newPotionCount + thisMaxStack - 1) / thisMaxStack) : 0;
	const int32 extraSlotsNeeded = newSlotsForThisPotion - oldSlotsForThisPotion;

	// 총 점유 칸이 용량 초과면 구매 실패
	if ((usedEquipSlots + CurrentPotionSlots + extraSlotsNeeded) > maxSlots) return false;

	int32& currCount = playerData.StackCounts.FindOrAdd(row->PotionTag);
	currCount = newPotionCount;

	currencyData->CurrAmount -= totalPrice;

	Gi->SavePlayerData();

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
	if (InGameInstance)
	{
		const int32 equipmentCount = FMath::Max(0, InEquipmentSlotCount);
		const int32 potionCount = FMath::Max(0, InPotionSlotCount);
		const int32 totalCount = equipmentCount + potionCount;

		if (totalCount > 0)
		{
			if (UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>())
			{
				// 장비 구간 갱신
				if (equipmentCount > 0)
				{
					FShopItemPools itemPools;
					BuildShopItemPools(InGameInstance, itemPools);

					for (int32 i = 0; i < equipmentCount; i++)
					{
						const EShopRarityType rarityType = PickShopRarityType(InGameInstance);
						const FName pickedRow = PickShopItemRow(rarityType, itemPools);

						FShopProductKey key;
						key.TableTag = Arcanum::DataTable::Equipment;
						key.RowName = pickedRow;

						InGameInstance->CurrentShopKeys[i] = key;
					}
				}

				// 물약 구간 고정
				if (potionCount > 0)
				{
					UDataTable* const* potionTablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Potion);

					if (potionTablePtr && *potionTablePtr)
					{
						const TArray<FName> potionRows = (*potionTablePtr)->GetRowNames();

						for (int32 j = 0; j < potionCount; j++)
						{
							const int32 dstIndex = equipmentCount + j;

							if (bInRefreshEquipmentOnly)
							{
								const FShopProductKey& existing = InGameInstance->CurrentShopKeys[dstIndex];
								if (existing.TableTag.IsValid() && !existing.RowName.IsNone()) continue;
							}

							FShopProductKey key;
							key.TableTag = Arcanum::DataTable::Potion;

							if (potionRows.IsValidIndex(j))
							{
								key.RowName = potionRows[j];
							}

							InGameInstance->CurrentShopKeys[dstIndex] = key;
						}
					}
				}
			}
		}
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

int32 FPlayerAccountService::GetMaxStackByItemTag(const FDTInventoryRuleItem* InRuleRow, const FGameplayTag& InItemTag)
{
	if (!InRuleRow || !InItemTag.IsValid()) return 1;

	const FInventoryRuleItem* bestRule = nullptr;

	for (const FInventoryRuleItem& rule : InRuleRow->Rules)
	{
		if (rule.RuleTag.IsValid() && InItemTag.MatchesTag(rule.RuleTag))
		{
			if (!bestRule || rule.Priority > bestRule->Priority)
			{
				bestRule = &rule;
			}
		}
	}

	if (bestRule && bestRule->StackRule == EInventoryStackRule::Stackable)
	{
		return FMath::Max(1, bestRule->MaxStack);
	}

	return 1;
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
		FDateTime StartDate, EndDate;
		bool bStartOk = FDateTime::Parse(Row->StartTime, StartDate);
		bool bEndOk = FDateTime::Parse(Row->EndTime, EndDate);

		if (bStartOk && bEndOk) {
			if (Now >= StartDate && Now <= EndDate) OutRows.Add(Row);
		}
		else UE_LOG(LogTemp, Error, TEXT("Date Parsing Failed: %s"), *Row->StartTime);
	}
	OutRows.Sort([](const FDTGachaBannerDataRow& A, const FDTGachaBannerDataRow& B) { return A.DisplayPriority < B.DisplayPriority; });
}
bool FPlayerAccountService::ExecuteGacha(const UObject* WorldContextObject, const FPlayerData& PlayerData, FGameplayTag BannerTag, FCurrencyCost Cost, int32 PullCount)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return false;

	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	int64 SpendAmount = (PullCount == 1) ? (int64)Cost.SinglePullCost : (int64)Cost.MultiPullCost;
	FPlayerCurrency PlayerCurrency = GetPlayerCurrency(WorldContextObject);
	FCurrencyData* TargetData = PlayerCurrency.CurrencyDatas.Find(Cost.ConsumptionCurrencyTag);

	if (!TargetData || TargetData->CurrAmount < SpendAmount) return false;
	const FDTGachaBannerDataRow* BannerData = GetGachaBannerData(WorldContextObject, BannerTag);
	if (!BannerData) return false;

	UpdateCurrency(WorldContextObject, PlayerData, Cost.ConsumptionCurrencyTag, -SpendAmount);
	return GI->GenerateResults(BannerData, PullCount);
}