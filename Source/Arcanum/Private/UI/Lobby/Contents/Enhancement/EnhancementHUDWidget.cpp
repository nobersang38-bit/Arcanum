#include "UI/Lobby/Contents/Enhancement/EnhancementHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/SubLayout/InventoryItemSlotWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "UI/Lobby/Contents/ItemDetail/ItemDetailHelper.h"
#include "UI/Lobby/Contents/ItemDetail/ItemTitlePanelWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemStatPanelWidget.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Core/ARGameInstance.h"
#include "DataInfo/InventoryData/Data/FInventoryViewSlot.h"
#include "DataInfo/EnhancementData/DataTable/DTEnhanceRuleRow.h"
#include "Components/TextBlock.h"

void UEnhancementHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EnhanceButton)
	{
		EnhanceButton->OnClicked.RemoveDynamic(this, &UEnhancementHUDWidget::HandleEnhanceButtonClicked);
		EnhanceButton->OnClicked.AddDynamic(this, &UEnhancementHUDWidget::HandleEnhanceButtonClicked);
	}

	if (RerollButton)
	{
		RerollButton->OnClicked.RemoveDynamic(this, &UEnhancementHUDWidget::HandleRerollButtonClicked);
		RerollButton->OnClicked.AddDynamic(this, &UEnhancementHUDWidget::HandleRerollButtonClicked);
	}

	if (DisassembleButton)
	{
		DisassembleButton->OnClicked.RemoveDynamic(this, &UEnhancementHUDWidget::HandleDisassembleButtonClicked);
		DisassembleButton->OnClicked.AddDynamic(this, &UEnhancementHUDWidget::HandleDisassembleButtonClicked);
	}

	if (DetailChanceButton)
	{
		DetailChanceButton->OnClicked.RemoveDynamic(this, &UEnhancementHUDWidget::HandleDetailChanceButtonClicked);
		DetailChanceButton->OnClicked.AddDynamic(this, &UEnhancementHUDWidget::HandleDetailChanceButtonClicked);
	}

	if (DetailChancePanel)
	{
		DetailChancePanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	RefreshDetailChanceText();

	ClearEnhancementInfo();
}

void UEnhancementHUDWidget::RefreshSelectedItemSlot()
{
	if (!SelectedItemGuid.IsValid())
	{
		if (SelectedItemSlotWidget)
		{
			SelectedItemSlotWidget->ClearSlot(0);
			SelectedItemSlotWidget->SetSelected(false);
		}

		ClearEnhancementInfo();
		return;
	}

	FInventoryViewSlot selectedSlot;
	selectedSlot.Type = EInventoryViewSlotType::Equipment;
	selectedSlot.ItemGuid = SelectedItemGuid;
	selectedSlot.ItemTag = FGameplayTag();
	selectedSlot.UpgradeLevel = 0;

	const FPlayerData playerData = FPlayerAccountService::GetPlayerDataCopy(this);
	for (const FEquipmentInfo& equip : playerData.Inventory)
	{
		if (equip.ItemGuid == SelectedItemGuid)
		{
			selectedSlot.ItemTag = equip.ItemTag;
			selectedSlot.UpgradeLevel = equip.CurrUpgradeLevel;

			if (const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, equip.ItemTag))
			{
				selectedSlot.Icon = catalogRow->Icon;
			}
			break;
		}
	}

	if (SelectedItemSlotWidget)
	{
		SelectedItemSlotWidget->SetSlotData(selectedSlot, 0);
		SelectedItemSlotWidget->SetSelected(true);
	}

	RefreshEnhancementInfo();
}

void UEnhancementHUDWidget::SetParentLobby(ULobbyHUD* InLobby)
{
	ParentLobby = InLobby;

	EnhancementInventoryWidget->SetParentLobby(InLobby);
	EnhancementInventoryWidget->SetCurrentFilter(EInventoryCategoryFilter::Equipment);

	EnhancementInventoryWidget->OnInventorySlotSelected.RemoveDynamic(this, &UEnhancementHUDWidget::HandleEnhancementItemSelected);
	EnhancementInventoryWidget->OnInventorySlotSelected.AddDynamic(this, &UEnhancementHUDWidget::HandleEnhancementItemSelected);

	RefreshEquipmentInventory();
}

void UEnhancementHUDWidget::RefreshEquipmentInventory()
{
	if (EnhancementInventoryWidget)
	{
		EnhancementInventoryWidget->SetCurrentFilter(EInventoryCategoryFilter::Equipment);
		EnhancementInventoryWidget->RefreshEquipmentInventory();
	}
}

void UEnhancementHUDWidget::HandleEnhancementItemSelected(const FInventoryViewSlot& InSlot)
{
	if (InSlot.Type == EInventoryViewSlotType::Equipment)
	{
		SelectedItemGuid = InSlot.ItemGuid;
		RefreshSelectedItemSlot();
	}
	else
	{
		SelectedItemGuid.Invalidate();

		if (SelectedItemSlotWidget)
		{
			SelectedItemSlotWidget->ClearSlot(0);
			SelectedItemSlotWidget->SetSelected(false);
		}

		ClearEnhancementInfo();
	}
}

void UEnhancementHUDWidget::RefreshEnhancementInfo()
{
	if (!SelectedItemGuid.IsValid())
	{
		ClearEnhancementInfo();
		return;
	}

	FItemDisplayViewData tooltipData;
	if (!FItemDetailHelper::BuildEquipmentDisplayViewData(this, SelectedItemGuid, tooltipData))
	{
		ClearEnhancementInfo();
		return;
	}

	if (ItemTitlePanelWidget)
	{
		ItemTitlePanelWidget->SetItemNameText(tooltipData.ItemNameText);
		ItemTitlePanelWidget->SetUpgradeLevelText(tooltipData.UpgradeLevelText);
	}

	if (CurrentStatPanelWidget)
	{
		CurrentStatPanelWidget->SetStatLines(tooltipData.StatLines);
	}

	FEnhancementNextStatViewData nextStatData;
	if (FItemDetailHelper::BuildEnhancementNextStatViewData(this, SelectedItemGuid, nextStatData))
	{
		if (NextStatPanelWidget)
		{
			NextStatPanelWidget->SetStatLines(nextStatData.NextStatLines);
		}

		if (EnhanceChanceText)
		{
			EnhanceChanceText->SetText(FText::Format(FText::FromString(TEXT("성공확률 : {0}")),	nextStatData.EnhanceChanceText));
		}
	}
	else
	{
		if (NextStatPanelWidget)
		{
			NextStatPanelWidget->ClearStatLines();
		}

		if (EnhanceChanceText)
		{
			EnhanceChanceText->SetText(FText::FromString(TEXT("MAX")));
		}
	}
}

void UEnhancementHUDWidget::ClearEnhancementInfo()
{
	if (ItemTitlePanelWidget)
	{
		ItemTitlePanelWidget->ClearTitleData();
	}

	if (CurrentStatPanelWidget)
	{
		CurrentStatPanelWidget->ClearStatLines();
	}

	if (NextStatPanelWidget)
	{
		NextStatPanelWidget->ClearStatLines();
	}

	if (EnhanceChanceText)
	{
		EnhanceChanceText->SetText(FText::GetEmpty());
	}
}

void UEnhancementHUDWidget::HandleEnhanceButtonClicked()
{
	if (SelectedItemGuid.IsValid())
	{
		if (FPlayerAccountService::EnhanceEquipment(this, SelectedItemGuid))
		{
			if (ParentLobby)
			{
				ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
			}

			RefreshEquipmentInventory();
			RefreshSelectedItemSlot();
		}
	}
}

void UEnhancementHUDWidget::HandleRerollButtonClicked()
{
	if (SelectedItemGuid.IsValid())
	{
		if (FPlayerAccountService::RerollEquipment(this, SelectedItemGuid))
		{
			if (ParentLobby)
			{
				ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
			}

			RefreshEquipmentInventory();
			RefreshSelectedItemSlot();
		}
	}
}

void UEnhancementHUDWidget::HandleDisassembleButtonClicked()
{
	if (SelectedItemGuid.IsValid())
	{
		if (FPlayerAccountService::DisassembleEquipment(this, SelectedItemGuid))
		{
			SelectedItemGuid.Invalidate();

			if (ParentLobby)
			{
				ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
			}

			RefreshEquipmentInventory();

			if (SelectedItemSlotWidget)
			{
				SelectedItemSlotWidget->ClearSlot(0);
				SelectedItemSlotWidget->SetSelected(false);
			}

			ClearEnhancementInfo();
		}
	}
}

void UEnhancementHUDWidget::HandleDetailChanceButtonClicked()
{
	if (!DetailChanceText) return;

	const ESlateVisibility currentVisibility = DetailChancePanel->GetVisibility();
	DetailChancePanel->SetVisibility(
		currentVisibility == ESlateVisibility::Collapsed ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

}

void UEnhancementHUDWidget::RefreshDetailChanceText()
{
	if (!DetailChanceText) return;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance)
	{
		DetailChanceText->SetText(FText::GetEmpty());
		return;
	}

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem)
	{
		DetailChanceText->SetText(FText::GetEmpty());
		return;
	}

	FString chanceText;

	for (int32 level = 0; level < 5; level++)
	{
		const FName rowName = FName(*FString::Printf(TEXT("Level_%d"), level));
		const FDTEnhanceRuleRow* ruleRow = dataSubsystem->GetRow<FDTEnhanceRuleRow>(Arcanum::DataTable::EnhanceRule, rowName);

		if (ruleRow)
		{
			chanceText += FString::Printf(
				TEXT("+%d → +%d : %d%%"),
				level,
				level + 1,
				ruleRow->EnhanceSuccessRate);

			if (level < 4)
			{
				chanceText += TEXT("\n");
			}
		}
	}

	DetailChanceText->SetText(FText::FromString(chanceText));

}

