#include "UI/Lobby/Contents/Shop/SubLayout/ShopPanelWidget.h"
#include "UI/Lobby/Contents/Shop/SubLayout/ShopItemSlotWidget.h"
#include "DataInfo/ShopData/Data/FShopRuntimeData.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

void UShopPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedShopSlotIndex = INDEX_NONE;
}

void UShopPanelWidget::InitSlots(int32 InSlotCount)
{
	SlotCount = FMath::Max(0, InSlotCount);

	CreateSlots();
	BindSlotEvents();

	SelectedShopSlotIndex = INDEX_NONE;
	RefreshSelection();
}

void UShopPanelWidget::ApplyData(const TArray<FShopViewSlot>& InShopViewSlots)
{
	if (Slots.Num() <= 0) return;

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		UShopItemSlotWidget* slot = Slots[i];

		if (slot)
		{
			if (InShopViewSlots.IsValidIndex(i) && InShopViewSlots[i].ItemTag.IsValid())
			{
				const FShopViewSlot& viewSlot = InShopViewSlots[i];

				slot->SetSlotData(i, viewSlot.ItemTag, viewSlot.Icon, viewSlot.Name, viewSlot.Desc, viewSlot.Price, viewSlot.bSoldOut);
			}
			else
			{
				slot->ClearSlot();
			}
		}
	
		slot->SetSelected(i == SelectedShopSlotIndex);
	}
}

void UShopPanelWidget::ClearSelection()
{
	SelectedShopSlotIndex = INDEX_NONE;

	RefreshSelection();
}

void UShopPanelWidget::ClearSlots()
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (UShopItemSlotWidget* slot = Slots[i])
		{
			slot->ClearSlot();
		}
	}

	SelectedShopSlotIndex = INDEX_NONE;
	RefreshSelection();
}

void UShopPanelWidget::CreateSlots()
{
	if (!SlotContainer || !SlotWidgetClass) return;

	SlotContainer->ClearChildren();
	Slots.Reset();

	for (int32 i = 0; i < SlotCount; i++)
	{
		if (UShopItemSlotWidget* slot = CreateWidget<UShopItemSlotWidget>(this, SlotWidgetClass))
		{
			slot->ClearSlot();
			SlotContainer->AddChildToWrapBox(slot);
			Slots.Add(slot);
		}
	}
}

void UShopPanelWidget::BindSlotEvents()
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (UShopItemSlotWidget* slot = Slots[i])
		{
			slot->OnShopItemSlotClicked.RemoveDynamic(this, &UShopPanelWidget::HandleSlotClicked);
			slot->OnShopItemSlotClicked.AddDynamic(this, &UShopPanelWidget::HandleSlotClicked);
		}
	}
}

void UShopPanelWidget::HandleSlotClicked(int32 InLocalIndex)
{
	if (InLocalIndex != INDEX_NONE)
	{
		if (SelectedShopSlotIndex != InLocalIndex)
		{
			SelectedShopSlotIndex = InLocalIndex;

			RefreshSelection();
		}
	}

	OnSlotClicked.Broadcast(CategoryTag, InLocalIndex);
}

void UShopPanelWidget::RefreshSelection()
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (UShopItemSlotWidget* slot = Slots[i])
		{
			slot->SetSelected(i == SelectedShopSlotIndex);
		}
	}
}