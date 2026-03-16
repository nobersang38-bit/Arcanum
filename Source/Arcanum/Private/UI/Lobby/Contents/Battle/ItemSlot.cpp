#include "UI/Lobby/Contents/Battle/ItemSlot.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "Components/PanelWidget.h"

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (SetItemBtn)
	{
		SetItemBtn->OnClicked.RemoveDynamic(this, &UItemSlot::ClickSetItemBtn);
		SetItemBtn->OnClicked.AddDynamic(this, &UItemSlot::ClickSetItemBtn);
	}
}

void UItemSlot::ShowBattleInventory()
{
	SetVisibility(ESlateVisibility::Visible);

	if (BattleInventoryWidget)
	{
		BattleInventoryWidget->RefreshStackInventory();
	}
}

void UItemSlot::ClickSetItemBtn()
{
	OnSetItemBtnClicked.Broadcast();
}

