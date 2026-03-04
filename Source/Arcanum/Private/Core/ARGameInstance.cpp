#include "Core/ARGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ArcanumSaveGame.h"
#include "Core/SubSystem/GameDataSubsystem.h"

#include "DataInfo/BattleCharacter/BattleStats/DataTable/DTBattleStats.h"
#include "DataInfo/PlayerData/PlayerBattleData/DataTable/DTPlayerBattleStats.h"
#include "DataInfo/BattleCharacter/CharacterInfo/DataTable/DTCharacterBaseInfo.h"
#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"
#include "DataInfo/ItemData/Potion/DTPotionInfoRow.h"

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
    ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
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

    // 배틀 캐릭터 리스트 생성
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
            NewCharacter.CharacterInfo.CurrentGrade = 0;
            NewCharacter.CharacterInfo.CurrentLevel = 1;

            PlayerData.OwnedCharacters.Add(NewCharacter);
        }
    }

    if (!PlayerData.OwnedCharacters.IsEmpty()) {
        PlayerData.OwnedCharacters[0].bSelection = true;
        PlayerData.OwnedCharacters[0].CharacterInfo.CurrentGrade = 1;
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

void UARGameInstance::AddTestGold(int64 InAmount)
{
    if (InAmount == 0) { return; }
  
    AddCurrency(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, InAmount);

    SavePlayerData();
    OnCurrencyChanged.Broadcast();
}

void UARGameInstance::AddTestShard(int64 InAmount)
{
    if (InAmount == 0) { return; }

    AddCurrency(Arcanum::PlayerData::Currencies::NonRegen::Shard::Value, InAmount);

    SavePlayerData();
    OnCurrencyChanged.Broadcast();
}

void UARGameInstance::AddTestSoul(int64 InAmount)
{
    if (InAmount == 0) { return; }

    AddCurrency(Arcanum::PlayerData::Currencies::NonRegen::Soul::Value, InAmount);

    SavePlayerData();
    OnCurrencyChanged.Broadcast();
}
