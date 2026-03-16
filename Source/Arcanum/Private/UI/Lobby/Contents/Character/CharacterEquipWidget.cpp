#include "UI/Lobby/Contents/Character/CharacterEquipWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Components/TextBlock.h"
#include "DataInfo/InventoryData/Data/FInventoryViewSlot.h"

void UCharacterEquipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedItemGuid.Invalidate();

	if (InventoryHUDWidget)
	{
		InventoryHUDWidget->OnInventorySlotSelected.RemoveDynamic(this, &UCharacterEquipWidget::HandleInventorySlotSelected);
		InventoryHUDWidget->OnInventorySlotSelected.AddDynamic(this, &UCharacterEquipWidget::HandleInventorySlotSelected);
	}

	if (EquipBtn)
	{
		EquipBtn->OnClicked.RemoveDynamic(this, &UCharacterEquipWidget::ClickEquipBtn);
		EquipBtn->OnClicked.AddDynamic(this, &UCharacterEquipWidget::ClickEquipBtn);
	}

	if (UnequipBtn)
	{
		UnequipBtn->OnClicked.RemoveDynamic(this, &UCharacterEquipWidget::ClickUnequipBtn);
		UnequipBtn->OnClicked.AddDynamic(this, &UCharacterEquipWidget::ClickUnequipBtn);
	}

	RefreshButtonState();
}

void UCharacterEquipWidget::SetEquipSlotTag(const FGameplayTag& InEquipSlotTag)
{
	CurrentEquipSlotTag = InEquipSlotTag;
	ClearSelectedItem();

	if (InventoryHUDWidget)
	{
		InventoryHUDWidget->SetCategoryPanelVisible(false);
		InventoryHUDWidget->ClearSelection();

		if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot1) ||
			InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Slot2))
		{
			InventoryHUDWidget->RefreshEquipmentInventoryBySlot(EInventoryEquipSlotFilter::Weapon);
		}
		else if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::Legendary))
		{
			InventoryHUDWidget->RefreshEquipmentInventoryBySlot(EInventoryEquipSlotFilter::Legendary);
		}
		else if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Armor::Helmet))
		{
			InventoryHUDWidget->RefreshEquipmentInventoryBySlot(EInventoryEquipSlotFilter::Helmet);
		}
		else if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Armor::Chest))
		{
			InventoryHUDWidget->RefreshEquipmentInventoryBySlot(EInventoryEquipSlotFilter::Chest);
		}
		else if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Armor::Glove))
		{
			InventoryHUDWidget->RefreshEquipmentInventoryBySlot(EInventoryEquipSlotFilter::Glove);
		}
		else if (InEquipSlotTag.MatchesTagExact(Arcanum::Items::ItemSlot::Armor::Boot))
		{
			InventoryHUDWidget->RefreshEquipmentInventoryBySlot(EInventoryEquipSlotFilter::Boots);
		}
	}
}

void UCharacterEquipWidget::ShowEquipmentInventory()
{
	ClearSelectedItem();

	if (InventoryHUDWidget)
	{
		InventoryHUDWidget->SetCategoryPanelVisible(false);
		InventoryHUDWidget->ClearSelection();
		InventoryHUDWidget->SetCurrentFilter(EInventoryCategoryFilter::Equipment);
		InventoryHUDWidget->RefreshEquipmentInventoryBySlot(EInventoryEquipSlotFilter::None);
	}
}

void UCharacterEquipWidget::ClearSelectedItem()
{
	SelectedItemGuid.Invalidate();

	if (EquipNameText)
	{
		EquipNameText->SetText(FText::GetEmpty());
	}

	if (EquipInfoText)
	{
		EquipInfoText->SetText(FText::GetEmpty());
	}

	RefreshButtonState();
}

void UCharacterEquipWidget::RefreshButtonState()
{
	if (EquipBtn)
	{
		EquipBtn->SetIsEnabled(CurrentEquipSlotTag.IsValid() && SelectedItemGuid.IsValid());
	}

	if (UnequipBtn)
	{
		UnequipBtn->SetIsEnabled(CurrentEquipSlotTag.IsValid());
	}
}

void UCharacterEquipWidget::HandleInventorySlotSelected(const FInventoryViewSlot& InSlot)
{
	SelectedItemGuid.Invalidate();

	if (InSlot.Type == EInventoryViewSlotType::Equipment && InSlot.ItemGuid.IsValid())
	{
		SelectedItemGuid = InSlot.ItemGuid;
	}

	RefreshButtonState();
}

void UCharacterEquipWidget::ClickEquipBtn()
{
	if (CurrentEquipSlotTag.IsValid())
	{
		if (SelectedItemGuid.IsValid())
		{
			OnCharacterEquipRequested.Broadcast(CurrentEquipSlotTag, SelectedItemGuid);
		}
	}
}

void UCharacterEquipWidget::ClickUnequipBtn()
{
	if (CurrentEquipSlotTag.IsValid())
	{
		OnCharacterUnequipRequested.Broadcast(CurrentEquipSlotTag);
	}
}