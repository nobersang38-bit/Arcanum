#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"

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
// Shop Widget 관련
// ========================================================
