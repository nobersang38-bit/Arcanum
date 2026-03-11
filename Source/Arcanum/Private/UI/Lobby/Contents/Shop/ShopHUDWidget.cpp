#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "UI/Lobby/Contents/Shop/SubLayout/ShopPanelWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "DataInfo/ShopData/DataTable/DTShopCategoryRuleRow.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"


void UShopHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedSlotIndex = INDEX_NONE;
	ShopPanels.Reset();

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

	BindShopPanels();
}

void UShopHUDWidget::RefreshShopUI()
{
	BuildShopRuntimeCache();

	for (UShopPanelWidget* shopPanel : ShopPanels)
	{
		if (shopPanel)
		{
			if (const FShopViewSlotList* foundViewSlotList = CachedShopSlotsByCategory.Find(shopPanel->CategoryTag))
			{
				shopPanel->ApplyData(foundViewSlotList->ShopViewSlots);
			}
			else
			{
				shopPanel->ClearSlots();
			}
		}
	}

	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		const int32 remaining = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);
		HandleShopSecondChanged(remaining);
	}
}

void UShopHUDWidget::BuildShopRuntimeCache()
{
	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	CachedShopSlotsByCategory.Empty();

	for (const TPair<FGameplayTag, FShopProductList>& pair : gameInstance->ShopCategoryProducts)
	{
		const FGameplayTag& categoryTag = pair.Key;
		const FShopProductList& productList = pair.Value;

		FShopViewSlotList viewSlotList;
		viewSlotList.ShopViewSlots.Reset();

		for (const FShopProductEntry& productEntry : productList.ShopProducts)
		{
			FShopViewSlot viewSlot;
			viewSlot.ItemTag = productEntry.ItemTag;
			viewSlot.bSoldOut = productEntry.bSoldOut;

			if (const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(gameInstance, productEntry.ItemTag))
			{
				viewSlot.Icon = catalogRow->Icon;
				viewSlot.Name = catalogRow->DisplayName;
				viewSlot.Desc = catalogRow->Desc;
				viewSlot.Price = catalogRow->BuyPrice;
			}

			viewSlotList.ShopViewSlots.Add(viewSlot);
		}

		CachedShopSlotsByCategory.Add(categoryTag, viewSlotList);
	}
}

void UShopHUDWidget::ClearShopSelection()
{
	SelectedCategoryTag = FGameplayTag();
	SelectedSlotIndex = INDEX_NONE;

	for (UShopPanelWidget* shopPanel : ShopPanels)
	{
		if (shopPanel)
		{
			shopPanel->ClearSelection();
		}
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

void UShopHUDWidget::InitPanels()
{
	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::ShopCategoryRule);
	UDataTable* table = tablePtr ? *tablePtr : nullptr;
	if (!table) return;

	for (UShopPanelWidget* ShopPanel : ShopPanels)
	{
		if (ShopPanel)
		{
			int32 SlotCount = 0;

			for (const TPair<FName, uint8*>& Pair : table->GetRowMap())
			{
				const FDTShopCategoryRuleRow* Row = reinterpret_cast<const FDTShopCategoryRuleRow*>(Pair.Value);

				if (Row && Row->ShopRule.CategoryTag.MatchesTagExact(ShopPanel->CategoryTag))
				{
					SlotCount = FMath::Max(0, Row->ShopRule.SlotCount);
					break;
				}
			}
			ShopPanel->InitSlots(SlotCount);
			ShopPanel->ClearSelection();
		}
	}
	SelectedCategoryTag = FGameplayTag();
	SelectedSlotIndex = INDEX_NONE;
}

void UShopHUDWidget::BindShopPanels()
{
	ShopPanels.Reset();

	if (ShopPanelContainer)
	{
		const int32 childCount = ShopPanelContainer->GetChildrenCount();

		for (int32 i = 0; i < childCount; i++)
		{
			UWidget* childWidget = ShopPanelContainer->GetChildAt(i);
			UShopPanelWidget* shopPanel = Cast<UShopPanelWidget>(childWidget);

			if (shopPanel)
			{
				shopPanel->OnSlotClicked.RemoveDynamic(this, &UShopHUDWidget::HandleShopSlotClicked);
				shopPanel->OnSlotClicked.AddDynamic(this, &UShopHUDWidget::HandleShopSlotClicked);

				ShopPanels.Add(shopPanel);
			}
		}
	}
}

void UShopHUDWidget::HandleBuyClicked()
{
	if (!ParentLobby) return;
	if (!SelectedCategoryTag.IsValid()) return;
	if (SelectedSlotIndex == INDEX_NONE) return;

	const bool bSuccess = FPlayerAccountService::PurchaseShopSlot(this, SelectedCategoryTag, SelectedSlotIndex);
	UE_LOG(LogTemp, Log, TEXT("ShopHUD Buy(%d) : %s"), SelectedSlotIndex, bSuccess ? TEXT("true") : TEXT("false"));

	if (bSuccess)
	{
		ParentLobby->RefreshAllLobbyUI();
		RefreshShopUI();
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
			RefreshShopUI();
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
			RefreshShopUI();
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[Sell] : false"));
		}
	}
}

void UShopHUDWidget::HandleShopSlotClicked(FGameplayTag InCategoryTag, int32 InLocalIndex)
{
	SelectedCategoryTag = InCategoryTag;
	SelectedSlotIndex = InLocalIndex;

	for (UShopPanelWidget* shopPanel : ShopPanels)
	{
		if (shopPanel)
		{
			if (shopPanel->CategoryTag != InCategoryTag)
			{
				shopPanel->ClearSelection();
			}
			else
			{
				shopPanel->RefreshSelection();
			}
		}
	}
}

void UShopHUDWidget::InitShop()
{
	FPlayerAccountService::InitializeShop(this);
}

void UShopHUDWidget::BindShopTimer()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (UGameTimeSubsystem* gameTimeSubsystem = gameInstance->GetSubsystem<UGameTimeSubsystem>())
		{
			gameTimeSubsystem->OnShopSecondChanged.RemoveDynamic(this, &UShopHUDWidget::HandleShopSecondChanged);
			gameTimeSubsystem->OnShopSecondChanged.AddDynamic(this, &UShopHUDWidget::HandleShopSecondChanged);
		}

		const int32 remaining = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);
		HandleShopSecondChanged(remaining);
	}
}

void UShopHUDWidget::HandleShopSecondChanged(int32 InRemainingSeconds)
{
	if (InRemainingSeconds == 0)
	{
		if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
		{
			FPlayerAccountService::RefreshShop(gameInstance);
			InRemainingSeconds = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);
		}
		RefreshShopUI();
	}
	SetShopRemainingSeconds(InRemainingSeconds);
}

void UShopHUDWidget::RestartShopTimer()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		if (gameInstance->bShopPaused && gameInstance->PausedShopRemainingSeconds > 0)
		{
			const FDateTime nowKst = FPlayerAccountService::GetCurrentTimeKST();
			gameInstance->NextShopRefreshTime = nowKst + FTimespan(0, 0, gameInstance->PausedShopRemainingSeconds);

			gameInstance->bShopPaused = false;
			gameInstance->PausedShopRemainingSeconds = 0;

			if (UGameTimeSubsystem* gameTimeSubsystem = gameInstance->GetSubsystem<UGameTimeSubsystem>())
			{
				gameTimeSubsystem->StartShop(gameInstance->NextShopRefreshTime);
			}
		}
	}
}