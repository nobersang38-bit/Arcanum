#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/Contents/Shop/CurrencyWidget.h"
#include "UI/Lobby/Contents/Shop/ShopItemSlotWidget.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Core/ARGameInstance.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UShopHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectedShopSlotIndex = INDEX_NONE;

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
}

void UShopHUDWidget::InitShopSlots(int32 InShopSlotCount)
{
	ShopSlotCount = FMath::Max(1, InShopSlotCount);

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
		if (ShopSlots.IsValidIndex(slotIndex))
		{
			if (UShopItemSlotWidget* shopSlot = ShopSlots[slotIndex])
			{
				shopSlot->OnShopItemSlotClicked.RemoveDynamic(this, &UShopHUDWidget::HandleShopSlotClicked);
				shopSlot->OnShopItemSlotClicked.AddDynamic(this, &UShopHUDWidget::HandleShopSlotClicked);
			}
		}
	}
}

void UShopHUDWidget::HandleShopSlotClicked(int32 InSlotIndex)
{
	if (ShopSlots.IsValidIndex(InSlotIndex))
	{
		if (SelectedShopSlotIndex != InSlotIndex)
		{
			SelectedShopSlotIndex = InSlotIndex;
			RefreshShopSlotSelection();
		}
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

void UShopHUDWidget::ClearShopSelection()
{
	SelectedShopSlotIndex = INDEX_NONE;
	RefreshShopSlotSelection();
}

void UShopHUDWidget::RefreshShopSlotsUI()
{
	if (UARGameInstance* gameInstance = GetGameInstance<UARGameInstance>())
	{
		if (UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>())
		{
			for (int32 i = 0; i < ShopSlotCount; i++)
			{
				if (!ShopSlots.IsValidIndex(i))
				{
					continue;
				}

				UShopItemSlotWidget* shopSlot = ShopSlots[i];
				if (!shopSlot)
				{
					continue;
				}

				if (!gameInstance->CurrentShopRowNames.IsValidIndex(i))
				{
					shopSlot->ClearSlot();
					continue;
				}

				const FName rowName = gameInstance->CurrentShopRowNames[i];
				if (rowName == NAME_None)
				{
					shopSlot->ClearSlot();
					continue;
				}

				const FDTEquipmentInfoRow* row = dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, rowName);
				if (!row)
				{
					shopSlot->ClearSlot();
					continue;
				}

				shopSlot->SetEquipmentData(*row, i, rowName);

				bool bSoldOut = false;
				if (gameInstance->CurrentShopSoldOutStates.IsValidIndex(i))
				{
					bSoldOut = gameInstance->CurrentShopSoldOutStates[i];
				}
				shopSlot->SetSoldOut(bSoldOut);

				shopSlot->SetSelected(i == SelectedShopSlotIndex);
			}
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

void UShopHUDWidget::HandleBuyClicked()
{
	UE_LOG(LogTemp, Log, TEXT("BuyButton Clicked"));

	if (SelectedShopSlotIndex != INDEX_NONE)
	{
		if (ShopSlots.IsValidIndex(SelectedShopSlotIndex))
		{
			if (UShopItemSlotWidget* slot = ShopSlots[SelectedShopSlotIndex])
			{
				if (slot->IsPurchasable())
				{
					const FName rowName = slot->GetRowName();
					if (rowName != NAME_None)
					{
						OnBuyRequested.Broadcast(rowName);
					}
				}
			}
		}
	}
}

void UShopHUDWidget::HandleSellClicked()
{
	OnSellRequested.Broadcast();
}
