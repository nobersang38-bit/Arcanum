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

	if (CloseBtn)
	{
		CloseBtn->OnClicked.RemoveDynamic(this, &UItemSlot::ClickCloseBtn);
		CloseBtn->OnClicked.AddDynamic(this, &UItemSlot::ClickCloseBtn);
	}

	if (RemoveBtn)
	{
		RemoveBtn->OnClicked.RemoveDynamic(this, &UItemSlot::ClickRemoveBtn);
		RemoveBtn->OnClicked.AddDynamic(this, &UItemSlot::ClickRemoveBtn);
	}
}

void UItemSlot::ShowBattleInventory()
{
	SetVisibility(ESlateVisibility::Visible);

	if (BattleInventoryWidget)
	{
		BattleInventoryWidget->RefreshConsumableInventory();
	}
}

void UItemSlot::ClickSetItemBtn()
{
	OnSetItemBtnClicked.Broadcast();
}

void UItemSlot::ClickRemoveBtn()
{
	OnRemoveItemBtnClicked.Broadcast();
}

void UItemSlot::ClickCloseBtn()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

