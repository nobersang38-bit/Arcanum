#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryItemSlotWidget.h"
#include "Components/WrapBox.h"

void UInventoryHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedSlotIndex = INDEX_NONE;

	CachedViewSlots.Reset();
}

void UInventoryHUDWidget::ApplyInventorySlots(const TArray<FInventoryViewSlot>& InSlots)
{
	const int32 slotCount = Slots.Num();

	CachedViewSlots.Reset();
	CachedViewSlots.SetNum(slotCount);

	for (int32 slotIndex = 0; slotIndex < slotCount; slotIndex++)
	{
		if (Slots.IsValidIndex(slotIndex))
		{
			if (UInventoryItemSlotWidget* slot = Slots[slotIndex])
			{
				if (InSlots.IsValidIndex(slotIndex))
				{
					CachedViewSlots[slotIndex] = InSlots[slotIndex];
					slot->SetSlotData(CachedViewSlots[slotIndex], slotIndex);

					const bool bIsSelected = (SelectedSlotIndex == slotIndex);
					slot->SetSelected(bIsSelected);
				}
				// 입력 데이터가 없는 경우 (빈 슬롯 처리)
				else
				{
					CachedViewSlots[slotIndex] = FInventoryViewSlot();
					slot->ClearSlot(slotIndex);
					slot->SetSelected(false);
				}
			}
		}
	}
}

void UInventoryHUDWidget::ClearSelection()
{
	SelectedSlotIndex = INDEX_NONE;

	RefreshSelection();
}

void UInventoryHUDWidget::InitInventorySlots(int32 InSlotCount)
{
	const int32 slotCount = FMath::Max(1, InSlotCount);

	CreateInventorySlots(slotCount);
	BindSlotEvents();

	SelectedSlotIndex = INDEX_NONE;
	RefreshSelection();
}

void UInventoryHUDWidget::CreateInventorySlots(int32 InSlotCount)
{
	if (!SlotContainer || !InventoryItemSlotWidgetClass) return;

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

void UInventoryHUDWidget::HandleSlotClicked(int32 InSlotIndex)
{
	if (CachedViewSlots.IsValidIndex(InSlotIndex))
	{
		SelectedSlotIndex = InSlotIndex;

		OnInventorySlotSelected.Broadcast(CachedViewSlots[InSlotIndex]);

		RefreshSelection();
	}
}

void UInventoryHUDWidget::RefreshSelection()
{
	for (int32 slotIndex = 0; slotIndex < Slots.Num(); slotIndex++)
	{
		if (Slots.IsValidIndex(slotIndex))
		{
			if (UInventoryItemSlotWidget* slot = Slots[slotIndex])
			{
				const bool bIsSelected = (SelectedSlotIndex == slotIndex);

				slot->SetSelected(bIsSelected);
			}
		}
	}
}

