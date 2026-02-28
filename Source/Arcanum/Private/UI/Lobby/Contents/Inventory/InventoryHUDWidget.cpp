#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryItemSlotWidget.h"
#include "DataInfo/BattleCharacter/Equipment/Data/FEquipmentData.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"

void UInventoryHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedGuid.Invalidate();
}

void UInventoryHUDWidget::ApplyInventoryData(const TArray<FEquipmentInfo>& InItems, const TArray<const FDTEquipmentInfoRow*>& InRowPtrs)
{
	const int32 slotCount = Slots.Num();

	CachedItemGuids.Reset();
	CachedItemGuids.SetNum(slotCount);

	for (int32 slotIndex = 0; slotIndex < slotCount; slotIndex++)
	{
		CachedItemGuids[slotIndex].Invalidate();

		if (!Slots.IsValidIndex(slotIndex) || !Slots[slotIndex])
		{
			continue;
		}

		if (!InItems.IsValidIndex(slotIndex) || !InRowPtrs.IsValidIndex(slotIndex))
		{
			Slots[slotIndex]->ClearSlot(slotIndex);
			continue;
		}

		const FEquipmentInfo& item = InItems[slotIndex];
		const FDTEquipmentInfoRow* rowPtr = InRowPtrs[slotIndex];

		if (!rowPtr)
		{
			Slots[slotIndex]->ClearSlot(slotIndex);
			continue;
		}

		CachedItemGuids[slotIndex] = item.ItemGuid;

		Slots[slotIndex]->SetItemData(item, rowPtr, slotIndex);
		Slots[slotIndex]->SetSelected(CachedItemGuids.IsValidIndex(slotIndex) && CachedItemGuids[slotIndex] == SelectedGuid);
	}
}

void UInventoryHUDWidget::ClearSelection()
{
	SelectedGuid.Invalidate();

	RefreshSelection();
}

void UInventoryHUDWidget::InitInventorySlots(int32 InSlotCount)
{
	const int32 slotCount = FMath::Max(1, InSlotCount);

	CreateInventorySlots(slotCount);
	BindSlotEvents();

	SelectedGuid.Invalidate();
	RefreshSelection();
}

void UInventoryHUDWidget::CreateInventorySlots(int32 InSlotCount)
{
	if (!SlotContainer || !InventoryItemSlotWidgetClass) { return; }

	SlotContainer->ClearChildren();
	Slots.Reset();

	for (int32 slotIndex = 0; slotIndex < InSlotCount; slotIndex++)
	{
		if (UInventoryItemSlotWidget* slot = CreateWidget<UInventoryItemSlotWidget>(this, InventoryItemSlotWidgetClass))
		{
			slot->ClearSlot(slotIndex);
			SlotContainer->AddChildToWrapBox(slot);
			Slots.Add(slot);
		}
	}
}

void UInventoryHUDWidget::BindSlotEvents()
{
	for (int32 slotIndex = 0; slotIndex < Slots.Num(); slotIndex++)
	{
		if (Slots.IsValidIndex(slotIndex))
		{
			if (UInventoryItemSlotWidget* slot = Slots[slotIndex])
			{
				slot->OnInventorySlotClicked.RemoveDynamic(this, &UInventoryHUDWidget::HandleSlotClicked);
				slot->OnInventorySlotClicked.AddDynamic(this, &UInventoryHUDWidget::HandleSlotClicked);
			}
		}
	}
}

void UInventoryHUDWidget::HandleSlotClicked(FGuid InItemGuid)
{
	if (SelectedGuid != InItemGuid)
	{
		SelectedGuid = InItemGuid;

		OnInventorySlotSelected.Broadcast(InItemGuid);

		RefreshSelection();
	}
}

void UInventoryHUDWidget::RefreshSelection()
{
	for (int32 slotIndex = 0; slotIndex < Slots.Num(); slotIndex++)
	{
		if (Slots.IsValidIndex(slotIndex) && Slots[slotIndex])
		{
			const bool bIsSelected = (CachedItemGuids.IsValidIndex(slotIndex) && CachedItemGuids[slotIndex] == SelectedGuid);
			Slots[slotIndex]->SetSelected(bIsSelected);
		}
	}
}

