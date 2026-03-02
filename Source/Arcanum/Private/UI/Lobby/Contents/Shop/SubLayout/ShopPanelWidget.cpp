#include "UI/Lobby/Contents/Shop/SubLayout/ShopPanelWidget.h"
#include "UI/Lobby/Contents/Shop/SubLayout/ShopItemSlotWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

void UShopPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedShopSlotIndex = INDEX_NONE;
	StartIndex = 0;
}

void UShopPanelWidget::InitSlots(int32 InSlotCount, int32 InStartIndex)
{
	SlotCount = FMath::Max(0, InSlotCount);
	StartIndex = FMath::Max(0, InStartIndex);

	CreateSlots();
	BindSlotEvents();

	SelectedShopSlotIndex = INDEX_NONE;
	RefreshSelection();
}

void UShopPanelWidget::ApplyData(
	const TArray<FName>& InRowNames,
	const TArray<bool>& InSoldOutStates,
	const TArray<TSoftObjectPtr<UTexture2D>>& InIcons,
	const TArray<FText>& InNames,
	const TArray<FText>& InDescs,
	const TArray<int64>& InPrices)
{
	if (Slots.Num() <= 0) return;

	for (int32 localIndex = 0; localIndex < Slots.Num(); localIndex++)
	{
		UShopItemSlotWidget* slot = Slots[localIndex];
		if (!slot) continue;

		const int32 sourceIndex = StartIndex + localIndex;

		if (!InRowNames.IsValidIndex(sourceIndex) ||
			!InSoldOutStates.IsValidIndex(sourceIndex) ||
			!InIcons.IsValidIndex(sourceIndex) ||
			!InNames.IsValidIndex(sourceIndex) ||
			!InDescs.IsValidIndex(sourceIndex) ||
			!InPrices.IsValidIndex(sourceIndex))
		{
			slot->ClearSlot(); continue;
		}

		const FName rowName = InRowNames[sourceIndex];
		if (rowName.IsNone())
		{
			slot->ClearSlot(); continue;
		}

		slot->SetSlotData(
			sourceIndex,
			rowName,
			InIcons[sourceIndex],
			InNames[sourceIndex],
			InDescs[sourceIndex],
			InPrices[sourceIndex],
			InSoldOutStates[sourceIndex]
		);

		slot->SetSelected(sourceIndex == SelectedShopSlotIndex);
	}

	RefreshSelection();
}

void UShopPanelWidget::ClearSelection()
{
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

void UShopPanelWidget::HandleSlotClicked(int32 InShopSlotIndex)
{
	if (InShopSlotIndex != INDEX_NONE)
	{
		if (SelectedShopSlotIndex != InShopSlotIndex)
		{
			SelectedShopSlotIndex = InShopSlotIndex;

			RefreshSelection();
		}
	}

	OnSlotClicked.Broadcast(InShopSlotIndex);
}

void UShopPanelWidget::RefreshSelection()
{
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (UShopItemSlotWidget* slot = Slots[i])
		{
			const int32 slotIndex = slot->GetSlotIndex();
			slot->SetSelected(slotIndex != INDEX_NONE && slotIndex == SelectedShopSlotIndex);
		}
	}
}