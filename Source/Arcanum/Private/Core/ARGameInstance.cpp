#include "Core/ARGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ArcanumSaveGame.h"
#include "Core/SubSystem/GameDataSubsystem.h"

#include "DataInfo/BattleCharacter/BattleStats/DataTable/DTBattleStats.h"
#include "DataInfo/PlayerData/PlayerBattleData/DataTable/DTPlayerBattleStats.h"
#include "DataInfo/BattleCharacter/CharacterInfo/DataTable/DTCharacterBaseInfo.h"

#include "Data/Rows/UnitsDataRow.h"

// ========================================================
// 초기화 관련
// ========================================================
void UARGameInstance::Init()
{
	Super::Init();

	InitializeGameData();
}
void UARGameInstance::InitializeGameData()
{
	/// Todo : 추후 SaveSlot으로 저장이름 변경해줘야함. 지금 변경하면 테스트 불가.
	ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("d"), 0));
	//ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!ArSaveGame) {
		ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::CreateSaveGameObject(UArcanumSaveGame::StaticClass()));
		AddIDPW(TEXT("Admin"), TEXT("12345"));

		InitializeNewPlayerData();

		SavePlayerData();
	}
	else LoadPlayerData();
}
void UARGameInstance::InitializeNewPlayerData()
{
	PlayerData = FPlayerData();

	// 플레이어 재화
	{
		FCurrencyData Gold;
		Gold.CurrAmount = 0;
		Gold.MaxAmount = 999999;
		PlayerData.PlayerCurrency.CurrencyDatas.Add(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, Gold);

		FCurrencyData Shard;
		Shard.CurrAmount = 0;
		Shard.MaxAmount = 999999;
		PlayerData.PlayerCurrency.CurrencyDatas.Add(Arcanum::PlayerData::Currencies::NonRegen::Shard::Value, Shard);

		FCurrencyData Soul;
		Soul.CurrAmount = 0;
		Soul.MaxAmount = 999999;
		PlayerData.PlayerCurrency.CurrencyDatas.Add(Arcanum::PlayerData::Currencies::NonRegen::Soul::Value, Soul);

		FCurrencyData Supply;
		Supply.CurrAmount = 40;
		Shard.MaxAmount = 40;
		PlayerData.PlayerCurrency.CurrencyDatas.Add(Arcanum::PlayerData::Currencies::Regen::Supply::Value, Supply);
	}

	// 플레이어 배틀스탯(고기, 기지체력)
	{
		UGameDataSubsystem* DataSubsystem = GetSubsystem<UGameDataSubsystem>();
		if (!DataSubsystem) return;

		FDTPlayerBattleStatsRow* Row = DataSubsystem->GetRow<FDTPlayerBattleStatsRow>(Arcanum::DataTable::PlayerBattleStats, FName("Player"));
		if (!Row) return;

		PlayerData.PlayerBattleData.PlayerBattleRegenStat = Row->PlayerBattleRegenStat;
		PlayerData.PlayerBattleData.PlayerBattleNonRegenStat = Row->PlayerBattleNonRegenStat;

		for (FRegenStat& Stat : PlayerData.PlayerBattleData.PlayerBattleRegenStat)
		{
			Stat.InitializeTags();
		}
	}

	// 플레이어블 캐릭터 리스트 생성
	{
		UGameDataSubsystem* DataSubsystem = GetSubsystem<UGameDataSubsystem>();
		if (!DataSubsystem) return;

		UDataTable** TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::CharacterInfo);
		if (!TablePtr) return;

		UDataTable* Table = *TablePtr;

		PlayerData.OwnedCharacters.Empty();

		for (const auto& Pair : Table->GetRowMap()) {
			FDTCharacterBaseInfoRow* Row = (FDTCharacterBaseInfoRow*)Pair.Value;
			if (!Row) continue;

			FBattleCharacterData NewCharacter;
			NewCharacter.Character = Row->BattleCharacterInfo.CharacterTag;
			NewCharacter.CharacterInfo.BattleCharacterInitData = Row->BattleCharacterInfo;
			NewCharacter.CharacterInfo.CurrShardCount = 0;
			NewCharacter.CharacterInfo.CurrGrade = Row->BattleCharacterInfo.DefaultGrade;
			NewCharacter.CharacterInfo.CurrStarLevel = 0;
			NewCharacter.CharacterInfo.CurrentLevel = 1;

			FGuid EmptyGuid;
			NewCharacter.WeaponSlots.Add(Arcanum::Items::ItemSlot::Weapon::Slot1, EmptyGuid);
			NewCharacter.WeaponSlots.Add(Arcanum::Items::ItemSlot::Weapon::Slot2, EmptyGuid);
			NewCharacter.LegendaryWeaponSlots.Add(Arcanum::Items::ItemSlot::Weapon::Legendary, EmptyGuid);
			NewCharacter.ArmorSlots.Add(Arcanum::Items::ItemSlot::Armor::Boot, EmptyGuid);
			NewCharacter.ArmorSlots.Add(Arcanum::Items::ItemSlot::Armor::Chest, EmptyGuid);
			NewCharacter.ArmorSlots.Add(Arcanum::Items::ItemSlot::Armor::Glove, EmptyGuid);
			NewCharacter.ArmorSlots.Add(Arcanum::Items::ItemSlot::Armor::Helmet, EmptyGuid);

			PlayerData.OwnedCharacters.Add(NewCharacter);
		}

		if (!PlayerData.OwnedCharacters.IsEmpty()) {
			PlayerData.OwnedCharacters[0].bSelection = true;
			PlayerData.OwnedCharacters[0].CharacterInfo.CurrStarLevel = 1;
		}
	}
	{
		UGameDataSubsystem* DataSubsystem = GetSubsystem<UGameDataSubsystem>();
		if (!DataSubsystem) return;

		UDataTable** TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::AllyUnitInfo);
		if (!TablePtr) return;
		UDataTable* Table = *TablePtr;
		PlayerData.AllyburdenCharacters.Empty();
		for (const auto& Pair : Table->GetRowMap()) {
			FUnitsDataRow* Row = (FUnitsDataRow*)Pair.Value;
			if (!Row) continue;
			const FUnitInfoSetting& UnitInfo = Row->UnitData;
			PlayerData.AllyburdenCharacters.Add(UnitInfo);
		}
	}
}
// ========================================================
// ID/PW용
// ========================================================
bool UARGameInstance::AddIDPW(FString ID, FString PW)
{
	if (ArSaveGame) {
		if (ArSaveGame->UserDatabase.Contains(ID)) return false;

		ArSaveGame->UserDatabase.Add(ID, PW);
		if (ArSaveGame) UGameplayStatics::SaveGameToSlot(ArSaveGame, SaveSlotName, 0);
		return true;
	}
	return false;
}
bool UARGameInstance::CheckLogin(FString ID, FString PW)
{
	if (ArSaveGame && ArSaveGame->UserDatabase.Contains(ID)) return ArSaveGame->UserDatabase[ID] == PW;
	return false;
}
// ========================================================
// 플레이어 데이터 저장/로드
// ========================================================
bool UARGameInstance::SavePlayerData()
{
	if (!ArSaveGame) ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::CreateSaveGameObject(UArcanumSaveGame::StaticClass()));
	if (ArSaveGame) {
		ArSaveGame->PlayerData = PlayerData;
		return UGameplayStatics::SaveGameToSlot(ArSaveGame, SaveSlotName, 0);
	}

	return false;
}
bool UARGameInstance::LoadPlayerData()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) {
		ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
		if (ArSaveGame) {
			PlayerData = ArSaveGame->PlayerData;
			return true;
		}
	}
	else InitializeGameData();

	return false;
}
int64 UARGameInstance::GetCurrencyAmount(FGameplayTag InTag) const
{
	if (!InTag.IsValid()) return INDEX_NONE_LONG;

	const FCurrencyData* FoundData = PlayerData.PlayerCurrency.CurrencyDatas.Find(InTag);

	if (FoundData) return FoundData->CurrAmount;
	else return INDEX_NONE_LONG;
}
void UARGameInstance::AddCurrency(FGameplayTag CurrencyValueTag, int64 Amount)
{
	if (!CurrencyValueTag.IsValid() || Amount == 0) return;

	FCurrencyData& CurrencyData = PlayerData.PlayerCurrency.CurrencyDatas.FindOrAdd(CurrencyValueTag);
	CurrencyData.CurrAmount += Amount;

	if (CurrencyData.MaxAmount > 0) CurrencyData.CurrAmount = FMath::Clamp(CurrencyData.CurrAmount, int64(0), CurrencyData.MaxAmount);
	if (Amount > 0)  CurrencyData.TotalEarned += Amount;
}
bool UARGameInstance::DeletePlayerData()
{
	int32 UserIndex = 0;
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
		return UGameplayStatics::DeleteGameInSlot(SaveSlotName, UserIndex);

	return false;
}

// ========================================================
// 가챠 관련
// ========================================================
bool UARGameInstance::GenerateResults(const FDTGachaBannerDataRow* BannerData, int32 PullCount)
{
	GachaItemResult.Empty();
	TArray<FGachaItemResult> Results;
	Results.Reserve(PullCount);

	FGachaBannerState& BannerState = PlayerData.GachaState.BannerStates.FindOrAdd(BannerData->BannerTag);
	for (int32 i = 0; i < PullCount; ++i) {
		FGameplayTag Grade = DetermineGrade(BannerData);
		ApplyHardPity(BannerData, BannerState, Grade);

		const FGachaGradePool* Pool = BannerData->GradePools.FindByPredicate([&](const FGachaGradePool& P) { return P.GradeTag == Grade; });
		if (Pool) {
			FGachaItemResult Result = ResolvePickup(BannerData, *Pool, BannerState, Grade);
			Results.Add(Result);

		}
	}

	GachaItemResult = Results;
	return GachaItemResult.Num() > 0;


	//TArray<FGachaItemResult> Results;
	//const FGameplayTag EpicGrade = GetHighestGrade(BannerData);

	//// 테스트용 카운터
	//int32 LastEpicPullIndex = 0;
	//for (int32 i = 1; i <= 100000; ++i) {
	//    FGameplayTag Grade = DetermineGrade(BannerData);
	//    FGachaBannerState& BannerState = PlayerData.GachaState.BannerStates.FindOrAdd(BannerData->BannerTag);

	//    bool bIsPity = ApplyHardPity(BannerData, BannerState, Grade);

	//    const FGachaGradePool* Pool = BannerData->GradePools.FindByPredicate([&](const FGachaGradePool& P) { return P.GradeTag == Grade; });

	//    if (Pool) {
	//        FGachaItemResult Result = ResolvePickup(BannerData, *Pool, BannerState);
	//        Results.Add(Result);

	//        if (Grade == EpicGrade) {
	//            int32 PullsSinceLastEpic = i - LastEpicPullIndex;

	//            // Grade 태그 정보 추가
	//            UE_LOG(LogTemp, Log, TEXT("[%d번째] ★%s 당첨★ | 등급: %s | 이전 에픽 이후 [%d]회 소모 | 방식: %s | 아이템: %s"),
	//                i,
	//                *EpicGrade.ToString(),
	//                *Grade.ToString(), // 실제 결정된 Grade 출력
	//                PullsSinceLastEpic,
	//                bIsPity ? TEXT("천장(Pity)") : TEXT("생뽑(Luck)"),
	//                *Result.ItemTag.ToString());

	//            LastEpicPullIndex = i;
	//        }
	//    }
	//}

	//return Results.Num() > 0;
}
bool UARGameInstance::GenerateResultsTest(const FDTGachaBannerDataRow* BannerData, int32 PullCount)
{
	GachaItemResult.Empty();
	TArray<FGachaItemResult> Results;
	Results.Reserve(PullCount);

	FGachaBannerState& BannerState = PlayerData.GachaState.BannerStates.FindOrAdd(BannerData->BannerTag);
	for (int32 i = 0; i < PullCount; ++i) {
		FGameplayTag Grade = FGameplayTag::RequestGameplayTag(FName("Arcanum.Player.Grade.Epic.2"));
		ApplyHardPity(BannerData, BannerState, Grade);

		const FGachaGradePool* Pool = BannerData->GradePools.FindByPredicate([&](const FGachaGradePool& P) { return P.GradeTag == Grade; });
		if (Pool) {
			FGachaItemResult Result = ResolvePickup(BannerData, *Pool, BannerState, Grade);
			Results.Add(Result);

		}
	}

	GachaItemResult = Results;
	return GachaItemResult.Num() > 0;
}
FGameplayTag UARGameInstance::DetermineGrade(const FDTGachaBannerDataRow* BannerData)
{
	float Rand = FMath::FRand();
	float Acc = 0.f;
	for (const auto& Prob : BannerData->RarityProbabilities) {
		Acc += Prob.Probability;

		if (Rand <= Acc) return Prob.GradeTag;
	}

	return BannerData->RarityProbabilities.Last().GradeTag;
}
bool UARGameInstance::ApplyHardPity(const FDTGachaBannerDataRow* BannerData, FGachaBannerState& BannerState, FGameplayTag& InOutGrade)
{
	const FGameplayTag EpicGrade = GetHighestGrade(BannerData);
	if (InOutGrade == EpicGrade) {
		BannerState.PityCount = 0;
		return false;
	}

	if (BannerState.PityCount >= BannerData->FiveStarPityCount) {
		InOutGrade = EpicGrade;
		BannerState.PityCount = 0;
		return true;
	}
	else {
		BannerState.PityCount++;
		return false;
	}
}
FGameplayTag UARGameInstance::GetHighestGrade(const FDTGachaBannerDataRow* BannerData)
{
	float LowestProb = 999.f;
	FGameplayTag HighestTag;

	for (const auto& Prob : BannerData->RarityProbabilities) {
		if (Prob.Probability < LowestProb) {
			LowestProb = Prob.Probability;
			HighestTag = Prob.GradeTag;
		}
	}
	return HighestTag;
}
FGachaItemResult UARGameInstance::ResolvePickup(const FDTGachaBannerDataRow* BannerData, const FGachaGradePool& Pool, FGachaBannerState& BannerState, FGameplayTag GachaIndex)
{
	FGachaItemResult Result;
	Result.GradeTag = Pool.GradeTag;
	Result.SourceTable = Pool.CommonPoolTable;
	if (Pool.PickupCharacters.Num() == 0) {
		Result.ItemTag = GetRandomFromGrade(Pool, GachaIndex);
		return Result;
	}

	if (BannerState.bPickupGuaranteed) {
		BannerState.bPickupGuaranteed = false;
		Result.ItemTag = Pool.PickupCharacters[0];
		return Result;
	}

	if (FMath::FRand() <= Pool.PickupRatio) {
		int32 Index = FMath::RandRange(0, Pool.PickupCharacters.Num() - 1);
		Result.ItemTag = Pool.PickupCharacters[Index];
	}
	else {
		Result.ItemTag = GetRandomFromGrade(Pool, GachaIndex);
		if (BannerData->bGuaranteePickupOnFail) BannerState.bPickupGuaranteed = true;
	}

	return Result;
}
FGameplayTag UARGameInstance::GetRandomFromGrade(const FGachaGradePool& Pool, FGameplayTag GachaIndex)
{
	UDataTable* PoolTable = Pool.CommonPoolTable.LoadSynchronous();
	if (!PoolTable) return FGameplayTag::EmptyTag;

	TArray<FName> RowNames = PoolTable->GetRowNames();
	TArray<FGameplayTag> ValidList;

	UGameDataSubsystem* DataSubsystem = GetSubsystem<UGameDataSubsystem>();
	if (!DataSubsystem) return FGameplayTag::EmptyTag;

	static const FGameplayTag CharacterRoot = FGameplayTag::RequestGameplayTag("Arcanum.Player.Grade");
	static const FGameplayTag ItemRoot = FGameplayTag::RequestGameplayTag("Arcanum.Items.Rarity");

	if (GachaIndex.MatchesTag(CharacterRoot)) {
		TArray<FName> CandidateRows;

		for (const FName& RowName : RowNames) {
			if (FDTCharacterBaseInfoRow* CharRow = DataSubsystem->GetRow<FDTCharacterBaseInfoRow>(Arcanum::DataTable::CharacterInfo, RowName)) {
				if (CharRow->BattleCharacterInfo.DefaultGrade == Pool.GradeTag) CandidateRows.Add(RowName);
			}
		}

		if (CandidateRows.Num() > 0) {
			int32 RandIndex = FMath::RandRange(0, CandidateRows.Num() - 1);
			FName SelectedRow = CandidateRows[RandIndex];
			if (FDTCharacterBaseInfoRow* CharRow = DataSubsystem->GetRow<FDTCharacterBaseInfoRow>(Arcanum::DataTable::CharacterInfo, SelectedRow)) {
				FGameplayTag CharacterTag = CharRow->BattleCharacterInfo.CharacterTag;
				ValidList.Add(CharacterTag);
				AddCharacterToBattleCharacter(CharRow);
			}
		}
	}
	else if (GachaIndex.MatchesTag(ItemRoot)) {
		TArray<FName> CandidateRows;
		for (const FName& RowName : RowNames) {
			if (FDTEquipmentInfoRow* ItemRow = DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, RowName)) {
				if (ItemRow->ItemTag.MatchesTag(Pool.GradeTag)) CandidateRows.Add(RowName);
			}
		}

		if (CandidateRows.Num() > 0) {
			int32 RandIndex = FMath::RandRange(0, CandidateRows.Num() - 1);
			FName SelectedRow = CandidateRows[RandIndex];

			if (FDTEquipmentInfoRow* ItemRow = DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, SelectedRow)) {
				ValidList.Add(ItemRow->ItemTag);
				AddRandomEquipmentToInventory(ItemRow);
			}
		}
	}

	return ValidList.Num() > 0 ? ValidList[FMath::RandRange(0, ValidList.Num() - 1)] : FGameplayTag::EmptyTag;
}
void UARGameInstance::AddCharacterToBattleCharacter(FDTCharacterBaseInfoRow* CharRow)
{
	if (!CharRow) return;

	const FGameplayTag CharacterTag = CharRow->BattleCharacterInfo.CharacterTag;
	for (FBattleCharacterData& CharData : PlayerData.OwnedCharacters) {
		if (CharData.Character == CharacterTag) {
			if (CharData.CharacterInfo.CurrStarLevel == 0) {
				CharData.CharacterInfo.CurrStarLevel = 1;
				CharData.CharacterInfo.CurrGrade = CharRow->BattleCharacterInfo.DefaultGrade;
			}
			else CharData.CharacterInfo.CurrShardCount += CharData.CharacterInfo.BattleCharacterInitData.DuplicateShardReward;

			return;
		}
	}
}
void UARGameInstance::AddRandomEquipmentToInventory(FDTEquipmentInfoRow* InRow)
{
	if (!InRow || InRow->BaseInfoSteps.Num() == 0) return;

	FEquipmentInfo NewItem;
	NewItem.ItemTag = InRow->ItemTag;
	NewItem.ItemGuid = FGuid::NewGuid();
	NewItem.CurrUpgradeLevel = 0;

	NewItem.Equipment = InRow->BaseInfoSteps[0];
	for (const FStatRangeDefinition& Range : NewItem.Equipment.RandomStatRanges) {
		FDerivedStatModifier FinalStat;
		FinalStat.StatTag = Range.StatTag;

		float MinF = Range.MinValue.Flat;
		float MaxF = Range.MaxValue.Flat;
		FinalStat.Value.Flat = FMath::RandRange(FMath::Min(MinF, MaxF), FMath::Max(MinF, MaxF));

		float MinM = Range.MinValue.Mul;
		float MaxM = Range.MaxValue.Mul;
		FinalStat.Value.Mul = FMath::RandRange(FMath::Min(MinM, MaxM), FMath::Max(MinM, MaxM));
		NewItem.Equipment.OnHitTargetStats.Add(FinalStat);
	}

	if (PlayerData.Inventory.Num() < PlayerData.InventoryCapacity) PlayerData.Inventory.Add(NewItem);
	else {
		if (PlayerData.Mailbox.Num() < PlayerData.MailboxCapacity) {
			FMailItem MailItem;
			MailItem.Equipment = NewItem;
			MailItem.ExpireTime = FDateTime::UtcNow() + FTimespan::FromDays(30);

			PlayerData.Mailbox.Add(MailItem);
		}
	}


	//if (NewItem.Equipment.OnHitTargetStats.Num() > 0)
	//{
	//    const FDerivedStatModifier& LastStat = NewItem.Equipment.OnHitTargetStats.Last();
	//    UE_LOG(LogTemp, Log, TEXT("[Gacha_Success] Item: %s | GUID: %s | Stat: %s | Result: (Flat: %.2f, Mul: %.2f)"),
	//        *NewItem.ItemTag.ToString(),
	//        *NewItem.ItemGuid.ToString(),
	//        *LastStat.StatTag.ToString(),
	//        LastStat.Value.Flat,
	//        LastStat.Value.Mul);
	//}
}














//void UARGameInstance::RefreshSupply()
//{
//    FGameplayTag SupplyTag =
//        Arcanum::PlayerData::Currencies::Regen::Supply::Value;
//
//    FCurrencyData* Supply =
//        PlayerData.PlayerCurrency.CurrencyDatas.Find(SupplyTag);
//
//    if (!Supply)
//        return;
//
//    if (Supply->CurrAmount >= Supply->MaxAmount)
//        return;
//
//    FDateTime Now = FDateTime::UtcNow();
//    FTimespan Elapsed = Now - Supply->LastRegenTime;
//
//    const int32 RegenIntervalSec = 300; // 5분
//    int32 RegenAmount = Elapsed.GetTotalSeconds() / RegenIntervalSec;
//
//    if (RegenAmount > 0)
//    {
//        Supply->CurrAmount =
//            FMath::Clamp(
//                Supply->CurrAmount + RegenAmount,
//                int64(0),
//                Supply->MaxAmount);
//
//        Supply->LastRegenTime = Now;
//    }
//}

//bool UARGameInstance::ConsumeSupply(int32 Amount)
//{
//    FGameplayTag SupplyTag =
//        Arcanum::PlayerData::Currencies::Regen::Supply::Value;
//
//    FCurrencyData* Supply =
//        PlayerData.PlayerCurrency.CurrencyDatas.Find(SupplyTag);
//
//    if (!Supply || Supply->CurrAmount < Amount)
//        return false;
//
//    Supply->CurrAmount -= Amount;
//    Supply->LastRegenTime = FDateTime::UtcNow();
//
//    SavePlayerData();
//
//    return true;
//}












void UARGameInstance::InitializeCharacter(FGameplayTag CharacterTag)
{
	if (UserCharacterRegistry.Contains(CharacterTag)) return;

	const FName RowName = GetLeafNameFromTag(CharacterTag);

	FBattleCharacterData NewData;
	NewData.Character = CharacterTag;

	UGameDataSubsystem* DataSubsystem = GetSubsystem<UGameDataSubsystem>();
	if (!DataSubsystem) return;

	//FDTCharacterBattleRegenStatsRow* RegenRowPtr = DataSubsystem->GetRow<FDTCharacterBattleRegenStatsRow>(Arcanum::DataTable::BattleRegenStats, RowName);
	//if (RegenRowPtr) {
	//    for (TFieldIterator<FProperty> It(FDTCharacterBattleRegenStatsRow::StaticStruct()); It; ++It) {
	//        if (FStructProperty* StructProp = CastField<FStructProperty>(*It)) {
	//            if (StructProp->Struct == FRegenStat::StaticStruct()) {
	//                FRegenStat* StatPtr = StructProp->ContainerPtrToValuePtr<FRegenStat>(RegenRowPtr);
	//                if (StatPtr) {
	//                    FRegenStat TempStat = *StatPtr;
	//                    TempStat.InitializeTags();
	//                    NewData.BattleRegenStat.Add(TempStat);
	//                }
	//            }
	//        }
	//    }
	//}
	//FDTCharacterBattleNonRegenStatsRow* NonRegenRowPtr = DataSubsystem->GetRow<FDTCharacterBattleNonRegenStatsRow>(Arcanum::DataTable::BattleNonRegenStats, RowName);
	//if (NonRegenRowPtr) {
	//    for (TFieldIterator<FProperty> It(FDTCharacterBattleNonRegenStatsRow::StaticStruct()); It; ++It) {
	//        if (FStructProperty* StructProp = CastField<FStructProperty>(*It)) {
	//            if (StructProp->Struct == FNonRegenStat::StaticStruct()) {
	//                FNonRegenStat* StatPtr = StructProp->ContainerPtrToValuePtr<FNonRegenStat>(NonRegenRowPtr);
	//                if (StatPtr) {
	//                    NewData.BattleNonRegenStat.Add(*StatPtr);
	//                }
	//            }
	//        }
	//    }
	//}

	UserCharacterRegistry.Add(CharacterTag, NewData);
}

bool UARGameInstance::AddTestGold()
{
	return FPlayerAccountService::AddCurrency(this, Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, 10000);
}

bool UARGameInstance::AddTestSoul()
{
	return FPlayerAccountService::AddCurrency(this, Arcanum::PlayerData::Currencies::NonRegen::Soul::Value, 10000);
}

bool UARGameInstance::AddTestShard()
{
	return FPlayerAccountService::AddCurrency(this, Arcanum::PlayerData::Currencies::NonRegen::Shard::Value, 10000);
}

bool UARGameInstance::AddTestEquipmentSet()
{
	UGameDataSubsystem* dataSubsystem = GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem)
	{
		return false;
	}

	const TArray<FGameplayTag> itemTags =
	{
		Arcanum::Items::Rarity::Common::Weapon::GreatSword,
		Arcanum::Items::Rarity::Common::Weapon::Staff,
		Arcanum::Items::Rarity::Legendary::Weapon::Scepter,
		Arcanum::Items::Rarity::SetItem::Talasha::Armor::Helmet,
		Arcanum::Items::Rarity::SetItem::Talasha::Armor::Chest,
		Arcanum::Items::Rarity::SetItem::Talasha::Armor::Glove,
		Arcanum::Items::Rarity::SetItem::Talasha::Armor::Boot
	};

	for (const FGameplayTag& itemTag : itemTags)
	{
		const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, itemTag);
		if (!catalogRow)
		{
			continue;
		}

		if (catalogRow->DetailRowName.IsNone())
		{
			continue;
		}

		FDTEquipmentInfoRow* equipRow = dataSubsystem->GetRow<FDTEquipmentInfoRow>(
			Arcanum::DataTable::Equipment,
			catalogRow->DetailRowName);

		if (!equipRow)
		{
			continue;
		}

		if (!equipRow->ItemTag.IsValid())
		{
			continue;
		}

		if (equipRow->BaseInfoSteps.IsEmpty())
		{
			continue;
		}

		FEquipmentInfo newItem;
		newItem.ItemTag = equipRow->ItemTag;
		newItem.ItemGuid = FGuid::NewGuid();
		newItem.CurrUpgradeLevel = 0;
		newItem.Equipment = equipRow->BaseInfoSteps[0];

		if (newItem.ItemTag.MatchesTag(Arcanum::Items::Rarity::Common::Weapon::Root) ||
			newItem.ItemTag.MatchesTag(Arcanum::Items::Rarity::Legendary::Weapon::Root))
		{
			newItem.Equipment.OnHitTargetStats.Empty();

			for (const FStatRangeDefinition& range : newItem.Equipment.RandomStatRanges)
			{
				FDerivedStatModifier finalStat;
				finalStat.StatTag = range.StatTag;

				const float minF = FMath::Min(range.MinValue.Flat, range.MaxValue.Flat);
				const float maxF = FMath::Max(range.MinValue.Flat, range.MaxValue.Flat);
				finalStat.Value.Flat = FMath::RandRange(minF, maxF);

				const float minM = FMath::Min(range.MinValue.Mul, range.MaxValue.Mul);
				const float maxM = FMath::Max(range.MinValue.Mul, range.MaxValue.Mul);
				finalStat.Value.Mul = FMath::RandRange(minM, maxM);

				newItem.Equipment.OnHitTargetStats.Add(finalStat);
			}
		}
		else
		{
			newItem.Equipment.OwnerStats.Empty();

			for (const FStatRangeDefinition& range : newItem.Equipment.RandomStatRanges)
			{
				FDerivedStatModifier finalStat;
				finalStat.StatTag = range.StatTag;

				const float minF = FMath::Min(range.MinValue.Flat, range.MaxValue.Flat);
				const float maxF = FMath::Max(range.MinValue.Flat, range.MaxValue.Flat);
				finalStat.Value.Flat = FMath::RandRange(minF, maxF);

				const float minM = FMath::Min(range.MinValue.Mul, range.MaxValue.Mul);
				const float maxM = FMath::Max(range.MinValue.Mul, range.MaxValue.Mul);
				finalStat.Value.Mul = FMath::RandRange(minM, maxM);

				newItem.Equipment.OwnerStats.Add(finalStat);
			}
		}

		if (PlayerData.Inventory.Num() < PlayerData.InventoryCapacity)
		{
			PlayerData.Inventory.Add(newItem);
		}
	}

	return SavePlayerData();
}
