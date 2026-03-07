#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "UI/Lobby/Contents/Shop/SubLayout/ShopPanelWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "Components/TextBlock.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"

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

void UShopHUDWidget::RefreshShopUI()
{
	BuildShopRuntimeCache();

	ApplyShopData(
		CachedShopRowNames,
		CachedShopSoldOutStates,
		CachedShopIcons,
		CachedShopNames,
		CachedShopDescs,
		CachedShopPrices
	);

	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		const int32 remaining = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);

		HandleShopSecondChanged(remaining);
	}
}

void UShopHUDWidget::BuildShopRuntimeCache()
{
	if (!ParentLobby) return;

	const int32 slotCount = FMath::Max(0, EquipmentShopSlotCount) + FMath::Max(0, PotionShopSlotCount);

	CachedShopRowNames.SetNum(slotCount);
	CachedShopSoldOutStates.SetNum(slotCount);

	CachedShopIcons.SetNum(slotCount);
	CachedShopNames.SetNum(slotCount);
	CachedShopDescs.SetNum(slotCount);
	CachedShopPrices.SetNum(slotCount);

	for (int32 i = 0; i < slotCount; i++)
	{
		CachedShopRowNames[i] = NAME_None;
		CachedShopSoldOutStates[i] = false;

		CachedShopIcons[i] = nullptr;
		CachedShopNames[i] = FText::GetEmpty();
		CachedShopDescs[i] = FText::GetEmpty();
		CachedShopPrices[i] = 0;
	}

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance) return;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return;

	for (int32 slotIndex = 0; slotIndex < slotCount; slotIndex++)
	{
		if (!gameInstance->CurrentShopKeys.IsValidIndex(slotIndex)) continue;

		const FShopProductKey& key = gameInstance->CurrentShopKeys[slotIndex];

		if (gameInstance->CurrentShopSoldOutStates.IsValidIndex(slotIndex))
		{
			CachedShopSoldOutStates[slotIndex] = gameInstance->CurrentShopSoldOutStates[slotIndex];
		}

		if (!key.TableTag.IsValid() || key.RowName.IsNone()) continue;

		// ItemCatalog 직접 키(=RowName이 ItemTagName)
		if (key.TableTag.MatchesTagExact(Arcanum::DataTable::ItemCatalog))
		{
			const FDTItemCatalogRow* catalogRow = dataSubsystem->GetRow<FDTItemCatalogRow>(Arcanum::DataTable::ItemCatalog, key.RowName);
			if (!catalogRow) continue;

			CachedShopRowNames[slotIndex] = key.RowName;
			CachedShopIcons[slotIndex] = catalogRow->Icon;
			CachedShopNames[slotIndex] = catalogRow->DisplayName;
			CachedShopDescs[slotIndex] = catalogRow->Desc;
			CachedShopPrices[slotIndex] = catalogRow->BuyPrice;

			continue;
		}

		CachedShopRowNames[slotIndex] = key.RowName;
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

void UShopHUDWidget::InitPanels()
{
	if (EquipmentPanel)
	{
		EquipmentPanel->InitSlots(FMath::Max(0, EquipmentShopSlotCount), 0);
		EquipmentPanel->ClearSelection();
	}

	if (PotionPanel)
	{
		const int32 equipCount = FMath::Max(0, EquipmentShopSlotCount);
		PotionPanel->InitSlots(FMath::Max(0, PotionShopSlotCount), equipCount);
		PotionPanel->ClearSelection();
	}

	SelectedSlotIndex = INDEX_NONE;
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

void UShopHUDWidget::HandleBuyClicked()
{
	if (!ParentLobby) return;
	if (SelectedSlotIndex == INDEX_NONE) return;

	const bool bSuccess = FPlayerAccountService::PurchaseShopSlot(this, SelectedSlotIndex);
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

void UShopHUDWidget::InitShop()
{
	if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
	{
		FPlayerAccountService::InitializeShop(gameInstance, EquipmentShopSlotCount, PotionShopSlotCount);
	}
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
			FPlayerAccountService::RefreshShop(gameInstance, EquipmentShopSlotCount, PotionShopSlotCount);
			BuildShopRuntimeCache();
			InRemainingSeconds = FPlayerAccountService::GetShopRemainingSeconds(gameInstance);
		}

		ApplyShopData(
			CachedShopRowNames,
			CachedShopSoldOutStates,
			CachedShopIcons,
			CachedShopNames,
			CachedShopDescs,
			CachedShopPrices
		);
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