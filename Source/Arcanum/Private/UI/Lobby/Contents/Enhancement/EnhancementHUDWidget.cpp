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

	RefreshEquipmentOnlyInventory();
}


void UEnhancementHUDWidget::RefreshEquipmentOnlyInventory()
{
	if (!InventoryHUDWidget) return;
	if (!ParentLobby) return;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	// 로비 캐시에서 장비만 인벤토리로 가져옴
	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();
	const int32 slotLimit = FMath::Max(1, playerData.InventoryCapacity);

	TArray<FInventoryViewSlot> viewSlots;
	viewSlots.SetNum(slotLimit);

	for (int32 i = 0; i < slotLimit; i++)
	{
		// Empty로 초기화
		FInventoryViewSlot slot;
		slot.Type = EInventoryViewSlotType::Empty;
		viewSlots[i] = slot;
	}

	int32 insertIndex = 0;
	for (int32 i = 0; i < playerData.Inventory.Num(); i++)
	{
		if (insertIndex >= slotLimit) break;

		const FEquipmentInfo& equip = playerData.Inventory[i];
		if (!equip.ItemGuid.IsValid()) continue;

		const FDTItemCatalogRow* catalogRow = dataSubsystem->FindItemCatalogRowByTag(equip.ItemTag);
		if (!catalogRow) continue;

		if (!catalogRow->ItemTypeTag.IsValid() ||
			!catalogRow->ItemTypeTag.MatchesTagExact(Arcanum::Items::Type::Equipment))
		{
			continue;
		}

		FInventoryViewSlot slot;
		slot.Type = EInventoryViewSlotType::Equipment;
		slot.ItemGuid = equip.ItemGuid;
		slot.ItemTag = equip.ItemTag;
		slot.StackCount = 0;
		slot.UpgradeLevel = equip.CurrUpgradeLevel;
		slot.Icon = catalogRow->Icon;

		viewSlots[insertIndex] = MoveTemp(slot);
		insertIndex++;
	}

	InventoryHUDWidget->ApplyInventorySlots(viewSlots);
}
