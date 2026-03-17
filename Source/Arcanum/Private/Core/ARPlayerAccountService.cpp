#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "Core/SubSystem/GameDataSubsystem.h"

#include "DataInfo/ShopData/Data/FShopRuntimeData.h"
#include "DataInfo/ItemData/DataTable/DTPotionInfoRow.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "DataInfo/EnhancementData/DataTable/DTEnhanceRuleRow.h"

#include "Kismet/GameplayStatics.h"

#pragma region 가챠 관련
#include "DataInfo/PlayerData/PlayerGacha/FPlayerGacha.h"
#pragma endregion

// 스태틱 관련 
FOnSaveCompleted FPlayerAccountService::OnSaveCompleted;

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
void FPlayerAccountService::SetCurrentStageTag(const UObject* WorldContextObject, FGameplayTag CurrentStageTag)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("Invalid WorldContext or GameInstance!"));
		return;
	}

	GI->CurrentStageTag = CurrentStageTag;
}
void FPlayerAccountService::ChangedLevel(const UObject* WorldContextObject, TSoftObjectPtr<UWorld> StageLevel)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (World) {
		UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
		if (!GI) {
			UE_LOG(LogTemp, Error, TEXT("Invalid WorldContext or GameInstance!"));
			return;
		}
		GI->PendingStageLevel = StageLevel;
	}
	const FName MainStageName = TEXT("/Game/Level/CombatStage/MainStage");
	UGameplayStatics::OpenLevel(WorldContextObject, MainStageName);
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
	SavePlayerData(GI);
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
	SavePlayerData(GI);
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
		UE_LOG(LogTemp, Error, TEXT("SavePlayerData(WorldContext): GameInstance not found!"));
		return false;
	}
	return SavePlayerData(GI);
}
bool FPlayerAccountService::SavePlayerData(UARGameInstance* GI)
{
	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("SavePlayerData: GameInstance is Null!"));
		return false;
	}

	bool bFinalResult = GI->SavePlayerData();
	OnSaveCompleted.Broadcast(bFinalResult);

	return bFinalResult;
}
// ========================================================
// Battle Widget 관련
// ========================================================
bool FPlayerAccountService::GetStageData(const UObject* WorldContextObject, TArray<FDTStageDataRow*>& OutRows)
{
	OutRows.Empty();

	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return false;

	UGameDataSubsystem* DataSubsystem = World->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!DataSubsystem) return false;

	UDataTable** TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::StageInfo);
	if (!TablePtr || !(*TablePtr)) return false;

	(*TablePtr)->GetAllRows<FDTStageDataRow>(TEXT("StageContext"), OutRows);

	return OutRows.Num() > 0;
}

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

bool FPlayerAccountService::SetBattlePotionSlot(const UObject* WorldContextObject, int32 InSlotIndex, const FGameplayTag& InPotionTag, int32 InCount)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;
	if (InSlotIndex < 0) return false;
	if (!InPotionTag.IsValid()) return false;
	if (InCount <= 0) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	if (playerData.BattlePotionSlots.Num() < 2)
	{
		playerData.BattlePotionSlots.SetNum(2);
	}	
	if (!playerData.BattlePotionSlots.IsValidIndex(InSlotIndex)) return false;

	const int32 ownedCount = playerData.StackCounts.FindRef(InPotionTag);
	if (ownedCount <= 0) return false;

	const int32 setCount = FMath::Min(ownedCount, InCount);
	if (setCount <= 0) return false;

	for (int32 slotIndex = 0; slotIndex < playerData.BattlePotionSlots.Num(); slotIndex++)
	{
		if (slotIndex != InSlotIndex)
		{
			const FBattlePotionSlotData& slotData = playerData.BattlePotionSlots[slotIndex];

			if (slotData.PotionTag.MatchesTagExact(InPotionTag))
			{
				return false;
			}
		}
	}

	FBattlePotionSlotData& targetSlot = playerData.BattlePotionSlots[InSlotIndex];
	targetSlot.PotionTag = InPotionTag;
	targetSlot.Count = setCount;

	return SavePlayerData(GI);
}

bool FPlayerAccountService::ClearBattlePotionSlot(const UObject* WorldContextObject, int32 InSlotIndex)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;
	if (InSlotIndex < 0) return false;

	FPlayerData& playerData = GI->GetPlayerData();
	if (!playerData.BattlePotionSlots.IsValidIndex(InSlotIndex)) return false;

	playerData.BattlePotionSlots[InSlotIndex].PotionTag = FGameplayTag();
	playerData.BattlePotionSlots[InSlotIndex].Count = 0;

	return SavePlayerData(GI);
}

// ========================================================
// Character Widget 관련
// ========================================================

bool FPlayerAccountService::EquipItemToCharacter(const UObject* WorldContextObject, const FName& InCharacterName, const FGameplayTag& InEquipSlotTag, const FGuid& InItemGuid)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;
	if (InCharacterName.IsNone() || !InEquipSlotTag.IsValid() || !InItemGuid.IsValid()) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	FEquipmentInfo* foundEquip = nullptr;
	for (FEquipmentInfo& equip : playerData.Inventory)
	{
		if (equip.ItemGuid == InItemGuid)
		{
			foundEquip = &equip;
			break;
		}
	}
	if (!foundEquip) return false;

	const FDTEquipmentInfoRow* equipRow = FindEquipmentInfoRowByTag(WorldContextObject, foundEquip->ItemTag);
	if (!equipRow) return false;

	const FGameplayTag itemSlotTag = equipRow->SlotTag;
	if (!itemSlotTag.IsValid()) return false;

	// 장착 가능 부위 검사
	if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot1) ||
		InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot2))
	{
		if (!foundEquip->ItemTag.MatchesTag(Arcanum::Items::Rarity::Common::Weapon::Root) &&
			!foundEquip->ItemTag.MatchesTag(Arcanum::Items::Rarity::Legendary::Weapon::Root))
		{
			return false;

		}
	}
	else if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Legendary))
	{
		if (!foundEquip->ItemTag.MatchesTag(Arcanum::Items::Rarity::Legendary::Weapon::Root))
		{
			return false;
		}
	}
	else if (InEquipSlotTag.MatchesTag(Arcanum::Items::ItemSlot::Armor::Root))
	{
		if (!itemSlotTag.MatchesTagExact(InEquipSlotTag))
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	FBattleCharacterData* foundCharacter = FindOwnedCharacterByName(playerData, InCharacterName);
	if (!foundCharacter) return false;
	if (!foundCharacter->bSelection) return false;

	TMap<FGameplayTag, FGuid>* slotMap = GetEquipmentSlotMapBySlotTag(*foundCharacter, InEquipSlotTag);
	if (!slotMap) return false;

	// 다른 캐릭터 포함 이미 장착 중인 장비면 실패
	for (const FBattleCharacterData& characterData : playerData.OwnedCharacters)
	{
		const FName otherCharacterName = GetLeafNameFromTag(characterData.CharacterInfo.BattleCharacterInitData.CharacterTag);
		if (otherCharacterName == InCharacterName)	continue;

		for (const TPair<FGameplayTag, FGuid>& pair : characterData.WeaponSlots)
		{
			if (pair.Value == InItemGuid)
			{
				UE_LOG(LogTemp, Warning, TEXT("이미 장착중입니다."));
				return false;
			}
		}

		for (const TPair<FGameplayTag, FGuid>& pair : characterData.LegendaryWeaponSlots)
		{
			if (pair.Value == InItemGuid)
			{
				UE_LOG(LogTemp, Warning, TEXT("이미 장착중입니다."));
				return false;
			}
		}

		for (const TPair<FGameplayTag, FGuid>& pair : characterData.ArmorSlots)
		{
			if (pair.Value == InItemGuid)
			{
				UE_LOG(LogTemp, Warning, TEXT("이미 장착중입니다."));
				return false;
			}
		}
	}

	// 장착 중복 처리
	if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot1) ||
		InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot2))
	{
		FGameplayTag OtherSlotTag;

		if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot1))
		{
			OtherSlotTag = Arcanum::Items::ItemSlot::Weapon::Slot2;
		}
		else
		{
			OtherSlotTag = Arcanum::Items::ItemSlot::Weapon::Slot1;
		}

		FGuid* OtherGuidPtr = slotMap->Find(OtherSlotTag);
		if (OtherGuidPtr && OtherGuidPtr->IsValid())
		{
			if (*OtherGuidPtr == InItemGuid)
			{
				slotMap->Remove(OtherSlotTag);
			}
			else
			{
				const FEquipmentInfo* otherEquip = nullptr;
				for (const FEquipmentInfo& equip : playerData.Inventory)
				{
					if (equip.ItemGuid == *OtherGuidPtr)
					{
						otherEquip = &equip;
						break;
					}
				}
				if (otherEquip && otherEquip->ItemTag == foundEquip->ItemTag)
				{
					return false;
				}
			}
		}
	}

	slotMap->FindOrAdd(InEquipSlotTag) = InItemGuid;
	return SavePlayerData(GI);
}

bool FPlayerAccountService::UnequipItemFromCharacter(const UObject* WorldContextObject, const FName& InCharacterName, const FGameplayTag& InEquipSlotTag)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;
	if (InCharacterName.IsNone() || !InEquipSlotTag.IsValid()) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	FBattleCharacterData* foundCharacter = FindOwnedCharacterByName(playerData, InCharacterName);
	if (!foundCharacter) return false;

	TMap<FGameplayTag, FGuid>* slotMap = GetEquipmentSlotMapBySlotTag(*foundCharacter, InEquipSlotTag);
	if (!slotMap) return false;

	if (slotMap->Remove(InEquipSlotTag) > 0)
	{
		return SavePlayerData(GI);
	}

	return false;
}

FBattleCharacterData* FPlayerAccountService::FindOwnedCharacterByName(FPlayerData& InPlayerData, const FName& InCharacterName)
{
	for (FBattleCharacterData& characterData : InPlayerData.OwnedCharacters)
	{
		const FName characterName = GetLeafNameFromTag(characterData.CharacterInfo.BattleCharacterInitData.CharacterTag);
		if (characterName == InCharacterName)
		{
			return &characterData;
		}
	}
	return nullptr;
}

TMap<FGameplayTag, FGuid>* FPlayerAccountService::GetEquipmentSlotMapBySlotTag(FBattleCharacterData& InCharacterData, const FGameplayTag& InEquipSlotTag)
{
	if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot1) ||
		InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot2))
	{
		return &InCharacterData.WeaponSlots;
	}

	if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Legendary))
	{
		return &InCharacterData.LegendaryWeaponSlots;
	}

	if (InEquipSlotTag.MatchesTag(Arcanum::Items::ItemSlot::Armor::Root))
	{
		return &InCharacterData.ArmorSlots;
	}

	return nullptr;
}

// ========================================================
// Enhancement Widget 관련
// ========================================================

bool FPlayerAccountService::EnhanceEquipment(const UObject* WorldContextObject, const FGuid& InItemGuid)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;
	if (!InItemGuid.IsValid()) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	FEquipmentInfo* foundEquip = nullptr;
	for (FEquipmentInfo& equip : playerData.Inventory)
	{
		if (equip.ItemGuid == InItemGuid)
		{
			foundEquip = &equip;
			break;
		}
	}
	if (!foundEquip) return false;

	const FDTItemCatalogRow* catalogRow = FindItemCatalogRowByTag(GI, foundEquip->ItemTag);
	if (!catalogRow) return false;
	if (catalogRow->DetailRowName.IsNone()) return false;

	const FDTEquipmentInfoRow* equipRow = dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
	if (!equipRow) return false;

	const int32 currentLevel = FMath::Max(0, foundEquip->CurrUpgradeLevel);
	const int32 maxLevel = FMath::Max(0, foundEquip->Equipment.MaxUpgradeLevel);
	if (currentLevel >= maxLevel) return false;

	const FName ruleRowName = FName(*FString::Printf(TEXT("Level_%d"), currentLevel));
	const FDTEnhanceRuleRow* ruleRow = dataSubsystem->GetRow<FDTEnhanceRuleRow>(Arcanum::DataTable::EnhanceRule, ruleRowName);
	if (!ruleRow) return false;

	const FGameplayTag goldTag = Arcanum::PlayerData::Currencies::NonRegen::Gold::Value;
	const int64 currentGold = GI->GetCurrencyAmount(goldTag);
	if (currentGold == INDEX_NONE_LONG) return false;
	if (currentGold < ruleRow->EnhanceGoldCost) return false;

	UpdateCurrency(WorldContextObject, GI->GetPlayerDataCopy(), goldTag, -ruleRow->EnhanceGoldCost);

	const int32 roll = FMath::RandRange(1, 100);
	if (roll > ruleRow->EnhanceSuccessRate) return true;

	const int32 nextLevel = currentLevel + 1;
	if (!equipRow->BaseInfoSteps.IsValidIndex(nextLevel)) return false;

	foundEquip->CurrUpgradeLevel = nextLevel;
	foundEquip->Equipment.RandomStatRanges = equipRow->BaseInfoSteps[nextLevel].RandomStatRanges;

	RollEquipmentStats(equipRow->BaseInfoSteps[nextLevel], foundEquip->Equipment.OwnerStats);

	return SavePlayerData(GI);
}

bool FPlayerAccountService::RerollEquipment(const UObject* WorldContextObject, const FGuid& InItemGuid)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;
	if (!InItemGuid.IsValid()) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	FPlayerData& playerData = GI->GetPlayerData();

	FEquipmentInfo* foundEquip = nullptr;
	for (FEquipmentInfo& equip : playerData.Inventory)
	{
		if (equip.ItemGuid == InItemGuid)
		{
			foundEquip = &equip;
			break;
		}
	}
	if (!foundEquip) return false;

	const FDTItemCatalogRow* catalogRow = FindItemCatalogRowByTag(GI, foundEquip->ItemTag);
	if (!catalogRow) return false;
	if (catalogRow->DetailRowName.IsNone()) return false;

	const FDTEquipmentInfoRow* equipRow =
		dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
	if (!equipRow) return false;

	const int32 currentLevel = FMath::Max(0, foundEquip->CurrUpgradeLevel);
	if (!equipRow->BaseInfoSteps.IsValidIndex(currentLevel)) return false;

	const FName ruleRowName = FName(*FString::Printf(TEXT("Level_%d"), currentLevel));
	const FDTEnhanceRuleRow* ruleRow = dataSubsystem->GetRow<FDTEnhanceRuleRow>(Arcanum::DataTable::EnhanceRule, ruleRowName);
	if (!ruleRow) return false;

	const FGameplayTag soulTag = Arcanum::PlayerData::Currencies::NonRegen::Soul::Value;
	const int64 currentSoul = GI->GetCurrencyAmount(soulTag);
	if (currentSoul == INDEX_NONE_LONG) return false;
	if (currentSoul < ruleRow->RerollSoulCost)
	{
		UE_LOG(LogTemp, Warning, TEXT("소울 부족"));
		return false;
	}

	UpdateCurrency(WorldContextObject, GI->GetPlayerDataCopy(), soulTag, -ruleRow->RerollSoulCost);

	foundEquip->Equipment.RandomStatRanges = equipRow->BaseInfoSteps[currentLevel].RandomStatRanges;
	RollEquipmentStats(equipRow->BaseInfoSteps[currentLevel], foundEquip->Equipment.OwnerStats);

	return SavePlayerData(GI);
}

bool FPlayerAccountService::DisassembleEquipment(const UObject* WorldContextObject, const FGuid& InItemGuid)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;
	if (!InItemGuid.IsValid()) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

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

	const FEquipmentInfo& foundEquip = playerData.Inventory[foundIndex];

	const FDTItemCatalogRow* catalogRow = FindItemCatalogRowByTag(GI, foundEquip.ItemTag);
	if (!catalogRow) return false;
	if (catalogRow->DetailRowName.IsNone()) return false;

	const FDTEquipmentInfoRow* equipRow = dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
	if (!equipRow) return false;

	const int32 currentLevel = FMath::Max(0, foundEquip.CurrUpgradeLevel);

	const FName ruleRowName = FName(*FString::Printf(TEXT("Level_%d"), currentLevel));
	const FDTEnhanceRuleRow* ruleRow = dataSubsystem->GetRow<FDTEnhanceRuleRow>(Arcanum::DataTable::EnhanceRule, ruleRowName);
	if (!ruleRow) return false;

	const FGameplayTag soulTag = Arcanum::PlayerData::Currencies::NonRegen::Soul::Value;

	UpdateCurrency(WorldContextObject, GI->GetPlayerDataCopy(), soulTag, ruleRow->DisassembleSoulReward);
	playerData.Inventory.RemoveAtSwap(foundIndex, 1, EAllowShrinking::No);

	return SavePlayerData(GI);
}

void FPlayerAccountService::RollEquipmentStats(const FItemDefinition& InItemDefinition, TArray<FDerivedStatModifier>& OutOwnerStats)
{
	OutOwnerStats.Empty();

	for (const FStatRangeDefinition& range : InItemDefinition.RandomStatRanges)
	{
		FDerivedStatModifier finalStat;
		finalStat.StatTag = range.StatTag;

		const float minF = FMath::Min(range.MinValue.Flat, range.MaxValue.Flat);
		const float maxF = FMath::Max(range.MinValue.Flat, range.MaxValue.Flat);
		finalStat.Value.Flat = FMath::RandRange(minF, maxF);

		const float minM = FMath::Min(range.MinValue.Mul, range.MaxValue.Mul);
		const float maxM = FMath::Max(range.MinValue.Mul, range.MaxValue.Mul);
		finalStat.Value.Mul = FMath::RandRange(minM, maxM);

		OutOwnerStats.Add(finalStat);
	}
}

// ========================================================
// Shop Widget 관련
// ========================================================
bool FPlayerAccountService::PurchaseShopSlot(const UObject* WorldContextObject, const FGameplayTag& InCategoryTag, int32 InLocalIndex)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;
	if (!InCategoryTag.IsValid()) return false;
	if (InLocalIndex < 0) return false;

	FShopProductEntry* productEntry = FindShopProductEntry(GI, InCategoryTag, InLocalIndex);
	if (!productEntry) return false;
	if (!productEntry->ItemTag.IsValid()) return false;
	if (productEntry->bSoldOut) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FDTItemCatalogRow* catalogRow = FindItemCatalogRowByTag(GI, productEntry->ItemTag);
	if (!catalogRow) return false;
	if (!catalogRow->IsValidCatalogRow()) return false;
	if (!catalogRow->StorePolicyTag.IsValid()) return false;

	// 재화 확인
	FPlayerData& playerData = GI->GetPlayerData();
	FCurrencyData* gold = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!gold) return false;

	const int64 price = catalogRow->BuyPrice;
	if (gold->CurrAmount < price) return false;


	// 물약 구매 처리
	if (catalogRow->StorePolicyTag.MatchesTagExact(Arcanum::Inventory::Store::Stack))
	{
		if (!CanAddStackItem(playerData, GI, catalogRow->ItemTag, 1, playerData.InventoryCapacity))
			return false;

		playerData.StackCounts.FindOrAdd(catalogRow->ItemTag) += 1;

		UpdateCurrency(WorldContextObject, GI->GetPlayerDataCopy(), Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, -price);

		return true;
	}

	// 장비 구매 처리
	if (catalogRow->StorePolicyTag.MatchesTagExact(Arcanum::Inventory::Store::Guid))
	{
		if (!CanAddGuidItem(playerData, GI, playerData.InventoryCapacity)) return false;
		if (!AddGuidByCatalog(WorldContextObject, catalogRow)) return false;

		UpdateCurrency(WorldContextObject, GI->GetPlayerDataCopy(), Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, -price);

		productEntry->bSoldOut = true;

		return true;
	}

	return false;
}

bool FPlayerAccountService::SellItemByGuid(const UObject* WorldContextObject, const FGuid& InItemGuid)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return false;

	if (!InItemGuid.IsValid()) return false;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

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

	const FDTItemCatalogRow* catalogRow = FindItemCatalogRowByTag(GI, equipment.ItemTag);
	if (!catalogRow) return false;

	const int64 sellPrice = static_cast<int64>(catalogRow->SellPrice);
	if (sellPrice < 0) return false;

	// 골드 추가
	FCurrencyData* gold = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!gold) return false;

	UpdateCurrency(WorldContextObject, GI->GetPlayerDataCopy(), Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, sellPrice);

	// 인벤엔서 제거
	playerData.Inventory.RemoveAtSwap(foundIndex, 1, EAllowShrinking::No);

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
	const FDTItemCatalogRow* catalogRow = FindItemCatalogRowByTag(GI, InItemTag);
	if (!catalogRow) return false;

	const int64 sellPricePerOne = static_cast<int64>(catalogRow->SellPrice);
	if (sellPricePerOne < 0) return false;

	// 골드 추가
	FCurrencyData* gold = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	if (!gold) return false;

	const int64 totalSellGold = sellPricePerOne * static_cast<int64>(InSellCount);
	UpdateCurrency(WorldContextObject, GI->GetPlayerDataCopy(), Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, totalSellGold);

	// 스택 차감 (0이면 제거)
	*currentCountPtr -= InSellCount;
	if (*currentCountPtr <= 0)
	{
		playerData.StackCounts.Remove(InItemTag);
	}

	return true;
}

const FDTEquipmentInfoRow* FPlayerAccountService::GetItemDefinition(UGameDataSubsystem* DataSubsystem, const FGameplayTag& ItemTag)
{
	if (!DataSubsystem) return nullptr;
	return DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, ItemTag.GetTagName());
}

void FPlayerAccountService::InitializeShop(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return;

	if ((GI->ShopCategoryProducts.Num() <= 0) ||
		(GI->NextShopRefreshTime.GetTicks() <= 0) ||
		IsShopRefreshExpired(WorldContextObject))
	{
		GenerateShopItems(GI);

		SetNextShopRefreshTime(GI);
	}

	if (UGameTimeSubsystem* gameTimeSubsystem = GI->GetSubsystem<UGameTimeSubsystem>())
	{
		gameTimeSubsystem->StartShop(GI->NextShopRefreshTime);
	}
}

void FPlayerAccountService::RefreshShop(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return;

	RefreshEquipmentShopCategory(GI);

	SetNextShopRefreshTime(GI);

	if (UGameTimeSubsystem* gameTimeSubsystem = GI->GetSubsystem<UGameTimeSubsystem>())
	{
		gameTimeSubsystem->StartShop(GI->NextShopRefreshTime);
	}
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

void FPlayerAccountService::GenerateShopItems(UARGameInstance* InGameInstance)
{
	if (!InGameInstance) return;

	UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::ShopCategoryRule);
	UDataTable* table = (tablePtr) ? *tablePtr : nullptr;
	if (!table) return;

	InGameInstance->ShopCategoryProducts.Empty();

	for (const TPair<FName, uint8*>& pair : table->GetRowMap())
	{
		const FDTShopCategoryRuleRow* row = reinterpret_cast<const FDTShopCategoryRuleRow*>(pair.Value);
		if (row && row->ShopRule.IsValidRule())
		{
			GenerateShopCategory(InGameInstance, row->ShopRule);
		}
	}
}

void FPlayerAccountService::GenerateShopCategory(UARGameInstance* InGameInstance, const FShopCategoryRule& InShopRule)
{
	if (!InGameInstance) return;
	if (!InShopRule.IsValidRule()) return;

	TArray<FGameplayTag> itemTags;
	BuildCatalogItemTagsByRule(InGameInstance, InShopRule, itemTags);

	FShopProductList productList;
	productList.ShopProducts.Reset();

	for (int32 i = 0; i < InShopRule.SlotCount; i++)
	{
		FShopProductEntry productEntry;
		productEntry.ItemTag = PickItemTagFromPool(itemTags);
		productEntry.bSoldOut = false;

		productList.ShopProducts.Add(productEntry);
	}

	InGameInstance->ShopCategoryProducts.Add(InShopRule.CategoryTag, productList);
}

void FPlayerAccountService::RefreshEquipmentShopCategory(UARGameInstance* InGameInstance)
{
	if (!InGameInstance) return;

	UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::ShopCategoryRule);
	UDataTable* table = (tablePtr) ? *tablePtr : nullptr;
	if (!table) return;

	for (const TPair<FName, uint8*>& pair : table->GetRowMap())
	{
		const FDTShopCategoryRuleRow* row = reinterpret_cast<const FDTShopCategoryRuleRow*>(pair.Value);
		if (row &&
			row->ShopRule.IsValidRule() &&
			row->ShopRule.bRefreshOnTimer)
		{
			GenerateShopCategory(InGameInstance, row->ShopRule);
		}
	}
}

void FPlayerAccountService::BuildCatalogItemTagsByRule(UARGameInstance* InGameInstance, const FShopCategoryRule& InShopRule, TArray<FGameplayTag>& OutItemTags)
{
	OutItemTags.Reset();

	if (!InGameInstance) return;
	if (!InShopRule.IsValidRule()) return;

	UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::ItemCatalog);
	UDataTable* table = (tablePtr) ? *tablePtr : nullptr;
	if (!table) return;

	for (const TPair<FName, uint8*>& pair : table->GetRowMap())
	{
		const FDTItemCatalogRow* row = reinterpret_cast<const FDTItemCatalogRow*>(pair.Value);
		if (row &&
			row->IsValidCatalogRow() &&
			row->StorePolicyTag.MatchesTagExact(InShopRule.StorePolicyTag) &&
			row->ItemTypeTag.MatchesTagExact(InShopRule.ItemTypeTag))
		{
			OutItemTags.Add(row->ItemTag);
		}
	}
}

FGameplayTag FPlayerAccountService::PickItemTagFromPool(TArray<FGameplayTag>& InOutItemTags)
{
	if (InOutItemTags.Num() > 0)
	{
		const int32 randomIndex = FMath::RandRange(0, InOutItemTags.Num() - 1);
		const FGameplayTag pickedItemTag = InOutItemTags[randomIndex];

		InOutItemTags.RemoveAtSwap(randomIndex, 1, EAllowShrinking::No);

		return pickedItemTag;
	}

	return FGameplayTag();
}

FShopProductEntry* FPlayerAccountService::FindShopProductEntry(UARGameInstance* InGameInstance, const FGameplayTag& InCategoryTag, int32 InLocalIndex)
{
	if (!InGameInstance || !InCategoryTag.IsValid() || InLocalIndex < 0) return nullptr;

	FShopProductList* productList = InGameInstance->ShopCategoryProducts.Find(InCategoryTag);
	if (productList && productList->ShopProducts.IsValidIndex(InLocalIndex))
	{
		return &productList->ShopProducts[InLocalIndex];
	}

	return nullptr;
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
	newEquip.ItemTag = InCatalogRow->ItemTag;
	newEquip.ItemGuid = FGuid::NewGuid();
	newEquip.CurrUpgradeLevel = 0;
	newEquip.Equipment = equipRow->BaseInfoSteps[0];

	// 랜덤 능력치
	RollEquipmentStats(newEquip.Equipment, newEquip.Equipment.OwnerStats);

	for (const FDerivedStatModifier& stat : newEquip.Equipment.OwnerStats)
	{
		UE_LOG(LogTemp, Log, TEXT("[Buy] Item=%s Stat=%s Flat=%.2f Mul=%.2f"),
			*newEquip.ItemTag.ToString(),
			*stat.StatTag.ToString(),
			stat.Value.Flat,
			stat.Value.Mul);
	}

	playerData.Inventory.Add(MoveTemp(newEquip));

	return true;
}

bool FPlayerAccountService::CanAddGuidItem(const FPlayerData& InPlayerData, UARGameInstance* InGameInstance, int32 InInventoryCapacity)
{
	const int32 maxSlots = FMath::Max(0, InInventoryCapacity);

	int32 usedGuidSlots = 0;
	for (int32 i = 0; i < InPlayerData.Inventory.Num(); i++)
	{
		if (InPlayerData.Inventory[i].ItemGuid.IsValid())
		{
			usedGuidSlots++;
		}
	}

	const int32 usedStackSlots = GetUsedStackSlotCount(InPlayerData, InGameInstance);

	return (usedGuidSlots + usedStackSlots + 1) <= maxSlots;
}

bool FPlayerAccountService::CanAddStackItem(const FPlayerData& InPlayerData, UARGameInstance* InGameInstance, const FGameplayTag& InItemTag, int32 InAddCount, int32 InInventoryCapacity)
{
	if (InItemTag.IsValid() && InAddCount > 0)
	{
		if (const FDTItemCatalogRow* catalogRow = FindItemCatalogRowByTag(InGameInstance, InItemTag))
		{
			const int32 maxSlots = FMath::Max(0, InInventoryCapacity);

			int32 usedGuidSlots = 0;
			for (int32 i = 0; i < InPlayerData.Inventory.Num(); i++)
			{
				if (InPlayerData.Inventory[i].ItemGuid.IsValid())
				{
					usedGuidSlots++;
				}
			}

			const int32 usedStackSlots = GetUsedStackSlotCount(InPlayerData, InGameInstance);
			const int32 oldCount = InPlayerData.StackCounts.FindRef(InItemTag);
			const int32 extraSlotsNeeded = GetExtraStackSlotsNeeded(oldCount, InAddCount, catalogRow->MaxStack);

			return (usedGuidSlots + usedStackSlots + extraSlotsNeeded) <= maxSlots;
		}
	}

	return false;
}

int32 FPlayerAccountService::GetUsedStackSlotCount(const FPlayerData& InPlayerData, UARGameInstance* InGameInstance)
{
	int32 usedStackSlots = 0;

	for (const TPair<FGameplayTag, int32>& pair : InPlayerData.StackCounts)
	{
		const FGameplayTag itemTag = pair.Key;
		const int32 count = pair.Value;

		if (itemTag.IsValid() && count > 0)
		{
			const FDTItemCatalogRow* stackCatalog = FindItemCatalogRowByTag(InGameInstance, itemTag);

			if (stackCatalog)
			{
				const int32 maxStack = FMath::Max(1, stackCatalog->MaxStack);
				usedStackSlots += (count + maxStack - 1) / maxStack;
			}
		}
	}

	return usedStackSlots;
}

int32 FPlayerAccountService::GetExtraStackSlotsNeeded(int32 InOldCount, int32 InAddCount, int32 InMaxStack)
{
	int32 extraSlotsNeeded = 0;

	if (InAddCount > 0)
	{
		const int32 maxStack = FMath::Max(1, InMaxStack);
		const int32 oldCount = FMath::Max(0, InOldCount);
		const int32 newCount = oldCount + InAddCount;

		const int32 oldSlots = (oldCount > 0) ? ((oldCount + maxStack - 1) / maxStack) : 0;
		const int32 newSlots = (newCount > 0) ? ((newCount + maxStack - 1) / maxStack) : 0;

		extraSlotsNeeded = newSlots - oldSlots;
	}

	return extraSlotsNeeded;
}

void FPlayerAccountService::BuildItemCatalogRowCache(UARGameInstance* InGameInstance)
{
	if (!InGameInstance) return;

	InGameInstance->ItemCatalogRowByTag.Empty();

	UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::ItemCatalog);
	UDataTable* table = (tablePtr) ? *tablePtr : nullptr;
	if (!table) return;

	for (const TPair<FName, uint8*>& pair : table->GetRowMap())
	{
		const FDTItemCatalogRow* row = reinterpret_cast<const FDTItemCatalogRow*>(pair.Value);
		if (row && row->IsValidCatalogRow() && row->ItemTag.IsValid())
		{
			InGameInstance->ItemCatalogRowByTag.Add(row->ItemTag, row);
		}
	}
}

const FDTItemCatalogRow* FPlayerAccountService::FindItemCatalogRowByTag(const UObject* WorldContextObject, const FGameplayTag& InItemTag)
{
	if (!WorldContextObject) return nullptr;
	if (!InItemTag.IsValid()) return nullptr;

	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return nullptr;

	if (GI->ItemCatalogRowByTag.Num() <= 0)
	{
		BuildItemCatalogRowCache(GI);
	}

	const FDTItemCatalogRow* const* foundRow = GI->ItemCatalogRowByTag.Find(InItemTag);
	if (foundRow)
	{
		return *foundRow;
	}

	return nullptr;
}

const FDTEquipmentInfoRow* FPlayerAccountService::FindEquipmentInfoRowByTag(const UObject* WorldContextObject, const FGameplayTag& InItemTag)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return nullptr;

	if (!InItemTag.IsValid()) return nullptr;

	UGameDataSubsystem* dataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return nullptr;

	const FDTItemCatalogRow* catalogRow = FindItemCatalogRowByTag(GI, InItemTag);
	if (!catalogRow) return nullptr;

	if (!catalogRow->DetailTableTag.MatchesTagExact(Arcanum::DataTable::Equipment)) return nullptr;
	if (catalogRow->DetailRowName.IsNone()) return nullptr;

	return dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
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
	bool res = false;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return res;

	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GI) return res;

	int64 SpendAmount = (PullCount == 1) ? (int64)Cost.SinglePullCost : (int64)Cost.MultiPullCost;
	FPlayerCurrency PlayerCurrency = GetPlayerCurrency(WorldContextObject);
	FCurrencyData* TargetData = PlayerCurrency.CurrencyDatas.Find(Cost.ConsumptionCurrencyTag);

	if (!TargetData || TargetData->CurrAmount < SpendAmount) return res;
	const FDTGachaBannerDataRow* BannerData = GetGachaBannerData(WorldContextObject, BannerTag);
	if (!BannerData) return res;

	if (GI->GenerateResults(BannerData, PullCount)) {
		UpdateCurrency(WorldContextObject, PlayerData, Cost.ConsumptionCurrencyTag, -SpendAmount);
		res = true;
	}

	return res;
}
// ========================================================
// Transient 관련
// ========================================================
void FPlayerAccountService::SetGachaItemEmpty(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("Invalid WorldContext or GameInstance!"));
		return;
	}

	GI->GachaItemResult.Empty();
}
TArray<FGachaItemResult> FPlayerAccountService::GetGachaItemResult(const UObject* WorldContextObject)
{
	UARGameInstance* GI = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

	if (!GI) {
		UE_LOG(LogTemp, Error, TEXT("Invalid WorldContext or GameInstance!"));
		return {};
	}

	return GI->GachaItemResult;
}