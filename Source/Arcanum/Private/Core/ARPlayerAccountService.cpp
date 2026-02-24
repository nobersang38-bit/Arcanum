#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"

// ========================================================
// PlayerData Getter
// ========================================================
const FPlayerData FPlayerAccountService::GetPlayerDataCopy(const UARGameInstance* GameInstance)
{
    return GameInstance->GetPlayerDataCopy();
}
const FPlayerCurrency& FPlayerAccountService::GetPlayerCurrency(const UARGameInstance* GameInstance) const
{
    return GameInstance->GetPlayerDataCopy().PlayerCurrency;
}
const FPlayerBattleData& FPlayerAccountService::GetPlayerBattleData(const UARGameInstance* GameInstance) const
{
    return GameInstance->GetPlayerDataCopy().PlayerBattleData;
}
const TArray<FBattleCharacterData>& FPlayerAccountService::GetOwnedCharacters(const UARGameInstance* GameInstance) const
{
    return GameInstance->GetPlayerDataCopy().OwnedCharacters;
}
const TArray<FEquipmentInfo>& FPlayerAccountService::GetInventory(const UARGameInstance* GameInstance) const
{
    return GameInstance->GetPlayerDataCopy().Inventory;
}
const TMap<FGameplayTag, FStageProgressData>& FPlayerAccountService::GetStageProgressMap(const UARGameInstance* GameInstance) const
{
    return GameInstance->GetPlayerDataCopy().StageProgressMap;
}
const FGachaData& FPlayerAccountService::GetGachaState(const UARGameInstance* GameInstance) const
{
    return GameInstance->GetPlayerDataCopy().GachaState;
}
const FPlayerQuest& FPlayerAccountService::GetQuestState(const UARGameInstance* GameInstance) const
{
    return GameInstance->GetPlayerDataCopy().QuestState;
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
TArray<FName> FPlayerAccountService::GetEquipmentList(UARGameInstance* GameInstance)
{
    TArray<FName> Res;
    if (!GameInstance) return Res;

    UGameDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
    if (!DataSubsystem) return Res;

    UDataTable** TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment);
    if (!TablePtr) return Res;

    UDataTable* Table = *TablePtr;
    for (const auto& Pair : Table->GetRowMap()) {
        Res.Add(Pair.Key);
    }

    return Res;
}
bool FPlayerAccountService::PurchaseEquipment(UARGameInstance* GameInstance, FName RowName)
{
    if (!GameInstance) return false;

    UGameDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
    if (!DataSubsystem) return false;

    FPlayerData& PlayerData = GameInstance->GetPlayerData();
    const FDTEquipmentInfoRow* Row = DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, RowName);

    if (!Row) return false;
    if (Row->BaseInfoSteps.IsEmpty()) return false;

    FCurrencyData* CurrencyData = PlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);

    if (!CurrencyData) return false;

    const int64 Price = static_cast<int64>(Row->BuyPrice);
    if (CurrencyData->CurrAmount < Price) return false;

    FEquipmentInfo NewEquip;
    NewEquip.ItemTag = Row->ItemTag;
    NewEquip.ItemGuid = FGuid::NewGuid();
    NewEquip.CurrUpgradeLevel = 0;
    NewEquip.Equipment = Row->BaseInfoSteps[0];
    PlayerData.Inventory.Add(MoveTemp(NewEquip));
    CurrencyData->CurrAmount -= Price;

    GameInstance->SavePlayerData();

    return true;
}
const FDTEquipmentInfoRow* FPlayerAccountService::GetItemDefinition(UGameDataSubsystem* DataSubsystem, const FGameplayTag& ItemTag)
{
    if (!DataSubsystem) return nullptr;
    return DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, ItemTag.GetTagName());
}
// ========================================================
// Gacha Widget 관련
// ========================================================



//// 1. 캐릭터 가챠일 때 (캐릭터 마스터 테이블 참조)
//UDataTable* CharTable = SelectedGrade.CommonPoolTable.LoadSynchronous();
//TArray<FBattleCharacterDataRow*> AllChars;
//CharTable->GetAllRows<FBattleCharacterDataRow>(TEXT(""), AllChars);
//
//TArray<FGameplayTag> Candidates;
//for (auto Row : AllChars)
//{
//    // JSON의 DefaultGrade가 현재 뽑으려는 등급(예: Epic)과 일치하는지 확인
//    if (Row->BattleCharacterInfo.DefaultGrade == SelectedGrade.GradeTag)
//    {
//        Candidates.Add(Row->BattleCharacterInfo.CharacterTag);
//    }
//}
//
//// 2. 아이템 가챠일 때 (아이템 마스터 테이블 참조)
//UDataTable* ItemTable = SelectedGrade.CommonPoolTable.LoadSynchronous();
//TArray<FItemDataRow*> AllItems;
//ItemTable->GetAllRows<FItemDataRow>(TEXT(""), AllItems);
//
//for (auto Row : AllItems)
//{
//    // 아이템은 ItemTag 내부에 등급 정보가 있으므로 MatchesTag 활용
//    // 예: Arcanum.Items.Rarity.Common.Weapon.GreatSword가
//    // SelectedGrade.GradeTag(Arcanum.Items.Rarity.Common)에 속하는지 체크
//    if (Row->ItemTag.MatchesTag(SelectedGrade.GradeTag))
//    {
//        Candidates.Add(Row->ItemTag);
//    }
//}