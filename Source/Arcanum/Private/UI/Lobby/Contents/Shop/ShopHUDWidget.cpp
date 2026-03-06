#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "UI/Lobby/Contents/Shop/SubLayout/ShopPanelWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Core/ARPlayerAccountService.h"
#include "Components/TextBlock.h"

void UShopHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedSlotIndex = INDEX_NONE;


	if (BuyButton)
	{
		BuyButton->OnClicked.RemoveDynamic(this, &UShopHUDWidget::HandleBuyClicked);
		BuyButton->OnClicked.AddDynamic(this, &UShopHUDWidget::HandleBuyClicked);
	}

	if (SellButton)
	{
		SellButton->OnClicked.RemoveDynamic(this, &UShopHUDWidget::HandleSellClicked);
		SellButton->OnClicked.AddDynamic(this, &UShopHUDWidget::HandleSellClicked);
	}

	if (EquipmentPanel)
	{
		EquipmentPanel->OnSlotClicked.RemoveDynamic(this, &UShopHUDWidget::HandleEquipmentSlotClicked);
		EquipmentPanel->OnSlotClicked.AddDynamic(this, &UShopHUDWidget::HandleEquipmentSlotClicked);
	}

	if (PotionPanel)
	{
		PotionPanel->OnSlotClicked.RemoveDynamic(this, &UShopHUDWidget::HandlePotionSlotClicked);
		PotionPanel->OnSlotClicked.AddDynamic(this, &UShopHUDWidget::HandlePotionSlotClicked);
	}
}

void UShopHUDWidget::ApplyShopData(
	const TArray<FName>& InRowNames,
	const TArray<bool>& InSoldOutStates,
	const TArray<TSoftObjectPtr<UTexture2D>>& InIcons,
	const TArray<FText>& InNames,
	const TArray<FText>& InDescs,
	const TArray<int64>& InPrices)
{
	if (EquipmentPanel)
	{
		EquipmentPanel->ApplyData(InRowNames, InSoldOutStates, InIcons, InNames, InDescs, InPrices);
	}

	if (PotionPanel)
	{
		PotionPanel->ApplyData(InRowNames, InSoldOutStates, InIcons, InNames, InDescs, InPrices);
	}
}

void UShopHUDWidget::ClearShopSelection()
{
	SelectedSlotIndex = INDEX_NONE;

	if (EquipmentPanel) 
	{ 
		EquipmentPanel->ClearSelection(); 
	}
	if (PotionPanel) 
	{ 
		PotionPanel->ClearSelection();
	}
}

void UShopHUDWidget::SetShopRemainingSeconds(int32 InRemainingSeconds)
{
	if (ShopTimerText && InRemainingSeconds >= 0)
	{
		const int32 minute = InRemainingSeconds / 60;
		const int32 second = InRemainingSeconds % 60;

		const FString timeText = FString::Printf(TEXT("%02d:%02d"), minute, second);
		ShopTimerText->SetText(FText::FromString(timeText));
	}
}

void UShopHUDWidget::InitPanels(int32 InEquipmentSlotCount, int32 InPotionSlotCount)
{
	if (EquipmentPanel)
	{
		EquipmentPanel->InitSlots(FMath::Max(0, InEquipmentSlotCount), 0);
		EquipmentPanel->ClearSelection();
	}

	if (PotionPanel)
	{
		const int32 equipCount = FMath::Max(0, InEquipmentSlotCount);
		PotionPanel->InitSlots(FMath::Max(0, InPotionSlotCount), equipCount);
		PotionPanel->ClearSelection();
	}

	SelectedSlotIndex = INDEX_NONE;
}

void UShopHUDWidget::HandleBuyClicked()
{
	if (!ParentLobby) return;
	if (SelectedSlotIndex == INDEX_NONE) return;

	const bool bSuccess = FPlayerAccountService::PurchaseShopSlot(this, SelectedSlotIndex); 
	UE_LOG(LogTemp, Log, TEXT("ShopHUD Buy(%d) : %s"), SelectedSlotIndex, bSuccess ? TEXT("true") : TEXT("false"));

	if (bSuccess)
	{
		ParentLobby->RefreshAllLobbyUI();
		ParentLobby->RefreshShopUI();
	}
}

void UShopHUDWidget::HandleEquipmentSlotClicked(int32 InSlotIndex)
{
	SelectedSlotIndex = InSlotIndex;

	if (PotionPanel)
	{
		PotionPanel->ClearSelection();
	}
}

void UShopHUDWidget::HandlePotionSlotClicked(int32 InSlotIndex)
{
	SelectedSlotIndex = InSlotIndex;

	if (EquipmentPanel)
	{
		EquipmentPanel->ClearSelection();
	}
}

void UShopHUDWidget::HandleSellClicked()
{
	if (!ParentLobby) return;
	if (!ParentLobby->GetInventoryHUDWidget()) return;

	UInventoryHUDWidget* inventoryHUD = ParentLobby->GetInventoryHUDWidget();

	const FGuid itemGuid = inventoryHUD->GetSelectedInventoryItemGuid();
	if (itemGuid.IsValid())
	{
		if (FPlayerAccountService::SellItemByGuid(this, itemGuid))
		{
			UE_LOG(LogTemp, Log, TEXT("[Sell] : true"));
			ParentLobby->RefreshAllLobbyUI();
			ParentLobby->RefreshShopUI();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[Sell] : false"));
		}
		return;
	}

	const FGameplayTag itemTag = inventoryHUD->GetSelectedStackItemTag();
	const int32 stackCount = inventoryHUD->GetSelectedStackItemCount();

	if (itemTag.IsValid() && stackCount > 0)
	{
		if (FPlayerAccountService::SellStackItemByTag(this, itemTag, stackCount))
		{
			UE_LOG(LogTemp, Log, TEXT("[Sell] : true"));
			ParentLobby->RefreshAllLobbyUI();
			ParentLobby->RefreshShopUI();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[Sell] : false"));
		}
	}
}
