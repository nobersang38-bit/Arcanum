#include "UI/Lobby/Contents/MailBox/MailboxPanelWidget.h"
#include "UI/Lobby/Contents/Inventory/SubLayout/InventoryItemSlotWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "Components/WrapBox.h"
#include "Core/ARPlayerAccountService.h"
#include "UI/Common/CommonBtnWidget.h"

void UMailboxPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedMailIndex = INDEX_NONE;

	if (ReceiveMailBtn)
	{
		ReceiveMailBtn->OnClicked.RemoveDynamic(this, &UMailboxPanelWidget::ClickReceiveMailBtn);
		ReceiveMailBtn->OnClicked.AddDynamic(this, &UMailboxPanelWidget::ClickReceiveMailBtn);
	}

	if (ReceiveAllMailBtn)
	{
		ReceiveAllMailBtn->OnClicked.RemoveDynamic(this, &UMailboxPanelWidget::ClickReceiveAllMailBtn);
		ReceiveAllMailBtn->OnClicked.AddDynamic(this, &UMailboxPanelWidget::ClickReceiveAllMailBtn);
	}

	RefreshSelection();
}

void UMailboxPanelWidget::RefreshMailboxUI()
{
	if (!ParentLobby) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();
	const int32 slotCount = FMath::Max(1, playerData.MailboxCapacity);

	CreateMailSlots(slotCount);
	BindSlotEvents();

	for (int32 slotIndex = 0; slotIndex < MailSlots.Num(); slotIndex++)
	{
		if (!MailSlots.IsValidIndex(slotIndex)) continue;

		UInventoryItemSlotWidget* slotWidget = MailSlots[slotIndex];
		if (!slotWidget) continue;

		if (playerData.Mailbox.IsValidIndex(slotIndex))
		{
			const FMailItem& mailItem = playerData.Mailbox[slotIndex];
			const FEquipmentInfo& equipment = mailItem.Equipment;
			const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, equipment.ItemTag);

			if (equipment.ItemGuid.IsValid() && catalogRow)
			{
				FInventoryViewSlot slotData;
				slotData.Type = EInventoryViewSlotType::Equipment;
				slotData.ItemGuid = equipment.ItemGuid;
				slotData.ItemTag = equipment.ItemTag;
				slotData.StackCount = 0;
				slotData.UpgradeLevel = equipment.CurrUpgradeLevel;
				slotData.Icon = catalogRow->Icon;

				slotWidget->SetSlotData(slotData, slotIndex);
				slotWidget->SetEquippedMarkVisible(false);
			}
			else
			{
				slotWidget->ClearSlot(slotIndex);
			}
		}
		else
		{
			slotWidget->ClearSlot(slotIndex);
		}
	}

	if (SelectedMailIndex != INDEX_NONE)
	{
		if (!playerData.Mailbox.IsValidIndex(SelectedMailIndex))
		{
			ClearSelection();
		}
		else
		{
			RefreshSelection();
		}
	}
	else
	{
		RefreshSelection();
	}
}

void UMailboxPanelWidget::CreateMailSlots(int32 InSlotCount)
{
	if (!MailSlotContainer || !InventoryItemSlotWidgetClass) return;

	MailSlotContainer->ClearChildren();
	MailSlots.Reset();

	for (int32 slotIndex = 0; slotIndex < InSlotCount; slotIndex++)
	{
		if (UInventoryItemSlotWidget* slot = CreateWidget<UInventoryItemSlotWidget>(this, InventoryItemSlotWidgetClass))
		{
			slot->ClearSlot(slotIndex);
			MailSlotContainer->AddChildToWrapBox(slot);
			MailSlots.Add(slot);
		}
	}
}

void UMailboxPanelWidget::BindSlotEvents()
{
	for (int32 slotIndex = 0; slotIndex < MailSlots.Num(); slotIndex++)
	{
		if (MailSlots.IsValidIndex(slotIndex))
		{
			if (UInventoryItemSlotWidget* slot = MailSlots[slotIndex])
			{
				slot->OnInventorySlotClicked.RemoveDynamic(this, &UMailboxPanelWidget::HandleMailSlotClicked);
				slot->OnInventorySlotClicked.AddDynamic(this, &UMailboxPanelWidget::HandleMailSlotClicked);
			}
		}
	}
}

void UMailboxPanelWidget::RefreshSelection()
{
	for (int32 slotIndex = 0; slotIndex < MailSlots.Num(); slotIndex++)
	{
		if (MailSlots.IsValidIndex(slotIndex))
		{
			if (UInventoryItemSlotWidget* slot = MailSlots[slotIndex])
			{
				const bool bIsSelected = (SelectedMailIndex == slotIndex);
				slot->SetSelected(bIsSelected);
			}
		}
	}
}

void UMailboxPanelWidget::ClearSelection()
{
	SelectedMailIndex = INDEX_NONE;
	RefreshSelection();
}

void UMailboxPanelWidget::HandleMailSlotClicked(int32 InSlotIndex)
{
	if (MailSlots.IsValidIndex(InSlotIndex))
	{
		SelectedMailIndex = InSlotIndex;
		RefreshSelection();
	}
}

void UMailboxPanelWidget::ClickReceiveMailBtn()
{
	if (ParentLobby)
	{
		if (SelectedMailIndex != INDEX_NONE)
		{
			if (FPlayerAccountService::ReceiveMailboxItem(this, SelectedMailIndex))
			{
				ParentLobby->RefreshAllLobbyUI();
				RefreshMailboxUI();
			}
		}
	}
}

void UMailboxPanelWidget::ClickReceiveAllMailBtn()
{
	if (ParentLobby)
	{
		if (FPlayerAccountService::ReceiveAllMailboxItems(this))
		{
			ParentLobby->RefreshAllLobbyUI();
			RefreshMailboxUI();
		}
	}
}
