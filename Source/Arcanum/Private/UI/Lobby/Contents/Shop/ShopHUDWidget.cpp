#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/Contents/Shop/CurrencyWidget.h"
#include "UI/Lobby/Contents/Shop/ShopItemSlotWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

void UShopHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateShopSlots();
	BindShopSlotEvents();

	SelectedShopSlotIndex = INDEX_NONE;
	RefreshShopSlotSelection();
}

void UShopHUDWidget::CreateShopSlots()
{
	if (!ShopSlotContainer || !ShopItemSlotWidgetClass) { return; }

	ShopSlotContainer->ClearChildren();
	ShopSlots.Reset();

	for (int32 slotIndex = 0; slotIndex < ShopSlotCount; slotIndex++)
	{
		if (UShopItemSlotWidget* shopSlot = CreateWidget<UShopItemSlotWidget>(this, ShopItemSlotWidgetClass))
		{
			shopSlot->ClearSlot();
			ShopSlotContainer->AddChildToWrapBox(shopSlot);
			ShopSlots.Add(shopSlot);
		}
	}
}

void UShopHUDWidget::BindShopSlotEvents()
{
	for (int32 slotIndex = 0; slotIndex < ShopSlotCount; slotIndex++)
	{
		if (UShopItemSlotWidget* shopSlot = ShopSlots[slotIndex])
		{
			shopSlot->OnShopItemSlotClicked.AddDynamic(this, &UShopHUDWidget::HandleShopSlotClicked);
		}
	}
}

void UShopHUDWidget::HandleShopSlotClicked(int32 InSlotIndex)
{
	if (SelectedShopSlotIndex != InSlotIndex)
	{
		if (SelectedShopSlotIndex != INDEX_NONE && ShopSlots.IsValidIndex(SelectedShopSlotIndex))
		{
			ShopSlots[SelectedShopSlotIndex]->SetSelected(false);
		}

		SelectedShopSlotIndex = InSlotIndex;
		ShopSlots[InSlotIndex]->SetSelected(true);

		RefreshShopSlotSelection();
	}
}

void UShopHUDWidget::RefreshShopSlotSelection()
{
	for (int32 slotIndex = 0; slotIndex < ShopSlotCount; slotIndex++)
	{
		if (ShopSlots.IsValidIndex(slotIndex))
		{
			ShopSlots[slotIndex]->SetSelected(slotIndex == SelectedShopSlotIndex);
		}
	}
}

