#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "Core/SubSystem/GameDataSubsystem.h"

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
void FPlayerAccountService::InitializeShop(const UObject* WorldContextObject, int32 InShopSlotCount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return;

	if (InShopSlotCount > 0)
	{
		const bool bDataValid =
			(GI->CurrentShopRowNames.Num() == InShopSlotCount) &&
			(GI->CurrentShopSoldOutStates.Num() == InShopSlotCount) &&
			(GI->NextShopRefreshTime.GetTicks() > 0);

		if (!bDataValid || IsShopRefreshExpired(WorldContextObject))
		{
			RefreshShop(WorldContextObject, InShopSlotCount);
		}

		if (UGameTimeSubsystem* gameTimeSubsystem = GI->GetSubsystem<UGameTimeSubsystem>())
		{
			gameTimeSubsystem->StartShop(GI->NextShopRefreshTime);
		}
	}
}


void FPlayerAccountService::RefreshShop(const UObject* WorldContextObject, int32 InShopSlotCount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return;

	if (InShopSlotCount > 0)
	{
		// 새 상점 아이템 생성
		GenerateShopItems(GI, InShopSlotCount);

		// 품절 상태 초기화
		GI->CurrentShopSoldOutStates.SetNum(GI->CurrentShopRowNames.Num());
		ResetShopSoldOutStates(GI, GI->CurrentShopRowNames.Num());

		// 다음 갱신 시각 저장
		SetNextShopRefreshTime(GI);

		// 상점 타이머 시작
		if (UGameTimeSubsystem* gameTimeSubsystem = GI->GetSubsystem<UGameTimeSubsystem>())
		{
			gameTimeSubsystem->StartShop(GI->NextShopRefreshTime);
		}
	}
}

bool FPlayerAccountService::GetShopSlotData(const UObject* WorldContextObject, int32 InSlotIndex, FName& OutRowName, bool& OutSoldOut)
{
	OutRowName = NAME_None;
	OutSoldOut = false;

	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	if (GI->CurrentShopRowNames.IsValidIndex(InSlotIndex) &&
		GI->CurrentShopSoldOutStates.IsValidIndex(InSlotIndex))
	{
		OutRowName = GI->CurrentShopRowNames[InSlotIndex];
		OutSoldOut = GI->CurrentShopSoldOutStates[InSlotIndex];

		return true;
	}

	return false;
}

bool FPlayerAccountService::PurchaseShopSlot(const UObject* WorldContextObject, int32 InSlotIndex)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	if (GI->CurrentShopRowNames.IsValidIndex(InSlotIndex) && 
		GI->CurrentShopSoldOutStates.IsValidIndex(InSlotIndex))
	{
		if (!GI->CurrentShopSoldOutStates[InSlotIndex])
		{
			const FName rowName = GI->CurrentShopRowNames[InSlotIndex];

			if (!rowName.IsNone() && PurchaseEquipment(WorldContextObject, rowName))
			{
				if (SetShopSlotSoldOut(WorldContextObject, InSlotIndex))
				{
					return true;
				}
			}
		}
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

void FPlayerAccountService::GenerateShopItems(UARGameInstance* InGameInstance, int32 InShopSlotCount)
{
	if (InGameInstance)
	{
		if (InShopSlotCount > 0)
		{
			FShopItemPools itemPools;
			BuildShopItemPools(InGameInstance, itemPools);

			InGameInstance->CurrentShopRowNames.Reset();
			InGameInstance->CurrentShopRowNames.Reserve(InShopSlotCount);

			for (int32 i = 0; i < InShopSlotCount; i++)
			{
				const EShopRarityType rarityType = PickShopRarityType(InGameInstance);
				const FName pickedRow = PickShopItemRow(rarityType, itemPools);

				InGameInstance->CurrentShopRowNames.Add(pickedRow);
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


void FPlayerAccountService::ResetShopSoldOutStates(UARGameInstance* InGameInstance, int32 InShopSlotCount)
{
	if (InGameInstance)
	{
		if (InShopSlotCount > 0)
		{
			InGameInstance->CurrentShopSoldOutStates.SetNum(InShopSlotCount);

			for (int32 slotIndex = 0; slotIndex < InShopSlotCount; slotIndex++)
			{
				InGameInstance->CurrentShopSoldOutStates[slotIndex] = false;
			}
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