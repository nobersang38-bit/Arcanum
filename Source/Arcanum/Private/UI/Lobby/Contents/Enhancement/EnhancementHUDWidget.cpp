#include "UI/Lobby/Contents/Enhancement/EnhancementHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/InventoryData/Data/InventoryViewSlot.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"

void UEnhancementHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UEnhancementHUDWidget::SetParentLobby(ULobbyHUD* InLobby)
{
	ParentLobby = InLobby;

	EnhancementInventoryWidget->SetParentLobby(InLobby);
	EnhancementInventoryWidget->SetCurrentFilter(EInventoryCategoryFilter::Equipment); // 여기 추가

	RefreshEquipmentInventory();
}

void UEnhancementHUDWidget::RefreshEquipmentInventory()
{
	if (!ParentLobby) return;
	if (!EnhancementInventoryWidget) return;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();

	const int32 slotCount = FMath::Max(1, playerData.InventoryCapacity);

	EnhancementInventoryWidget->InitInventorySlots(slotCount);

	TArray<FInventoryViewSlot> viewSlots;
	viewSlots.Reserve(slotCount);

	for (int32 i = 0; i < playerData.Inventory.Num(); i++)
	{
		if (viewSlots.Num() >= slotCount) break;

		const FEquipmentInfo& equip = playerData.Inventory[i];
		if (!equip.ItemGuid.IsValid()) continue;

		FInventoryViewSlot slot;
		slot.Type = EInventoryViewSlotType::Equipment;
		slot.ItemGuid = equip.ItemGuid;
		slot.ItemTag = equip.ItemTag;
		slot.StackCount = 0;
		slot.UpgradeLevel = equip.CurrUpgradeLevel;

		if (const FDTItemCatalogRow* catalogRow = dataSubsystem->FindItemCatalogRowByTag(equip.ItemTag))
		{
			slot.Icon = catalogRow->Icon;
		}

		viewSlots.Add(MoveTemp(slot));
	}

	EnhancementInventoryWidget->ApplyInventorySlots(viewSlots);
}
