#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/Contents/Shop/CurrencyWidget.h"
#include "UI/Lobby/Contents/Shop/ShopItemSlotWidget.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Core/ARGameInstance.h"
#include "Core/ArcanumSaveGame.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/TextBlock.h"


void UShopHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CreateShopSlots();
	BindShopSlotEvents();

	SelectedShopSlotIndex = INDEX_NONE;
	RefreshShopSlotSelection();

	if (UARGameInstance* gameInstance = GetGameInstance<UARGameInstance>())
	{
		// 상점 저장 데이터 초기화
		FPlayerAccountService::InitializeShop(gameInstance, ShopSlotCount);

		// 슬롯 UI 채우기
		RefreshShopSlotsUI();

		// 상점 타이머 바인딩
		if (UGameTimeSubsystem* gameTimeSubsystem = gameInstance->GetSubsystem<UGameTimeSubsystem>())
		{
			gameTimeSubsystem->OnShopSecondChanged.RemoveDynamic(this, &UShopHUDWidget::HandleShopSecondChanged);
			gameTimeSubsystem->OnShopSecondChanged.AddDynamic(this, &UShopHUDWidget::HandleShopSecondChanged);

			// 처음 1회 즉시 갱신
			HandleShopSecondChanged(FPlayerAccountService::GetShopRemainingSeconds(gameInstance));
		}
	}
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
			if (SelectedShopSlotIndex != INDEX_NONE && ShopSlots.IsValidIndex(SelectedShopSlotIndex))
			{
				ShopSlots[SelectedShopSlotIndex]->SetSelected(false);
			}

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

void UShopHUDWidget::RefreshShopSlotsUI()
{
	if (UARGameInstance* gameInstance = GetGameInstance<UARGameInstance>())
	{
		if (UArcanumSaveGame* saveGame = gameInstance->GetArSaveGame())
		{
			if (UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>())
			{
				for (int32 i = 0; i < ShopSlotCount; i++)
				{
					if (ShopSlots.IsValidIndex(i))
					{
						if (UShopItemSlotWidget* shopSlot = ShopSlots[i])
						{
							if (saveGame->CurrentShopRowNames.IsValidIndex(i))
							{
								const FName rowName = saveGame->CurrentShopRowNames[i];

								if (rowName != NAME_None)
								{
									if (const FDTEquipmentInfoRow* row = dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, rowName))
									{
										shopSlot->SetEquipmentData(*row, i, rowName);

										if (saveGame->CurrentShopSoldOutStates.IsValidIndex(i))
										{
											shopSlot->SetSoldOut(saveGame->CurrentShopSoldOutStates[i]);
										}
										else
										{
											shopSlot->SetSoldOut(false);
										}

										shopSlot->SetSelected(i == SelectedShopSlotIndex);
									}
									else
									{
										shopSlot->ClearSlot();
									}
								}
								else
								{
									shopSlot->ClearSlot();
								}
							}
							else
							{
								shopSlot->ClearSlot();
							}
						}
					}
				}
			}
		}
	}
}

void UShopHUDWidget::RefreshShopIfNeeded()
{
	if (UARGameInstance* gameInstance = GetGameInstance<UARGameInstance>())
	{
		if (FPlayerAccountService::IsShopRefreshExpired(gameInstance))
		{
			FPlayerAccountService::RefreshShop(gameInstance, ShopSlotCount);
		}

		RefreshShopSlotsUI();
	}
}

void UShopHUDWidget::HandleShopSecondChanged(int32 InRemainingSeconds)
{
	if (ShopTimerText)
	{
		if (InRemainingSeconds >= 0)
		{
			const int32 minute = InRemainingSeconds / 60;
			const int32 second = InRemainingSeconds % 60;

			const FString timeText = FString::Printf(TEXT("%02d:%02d"), minute, second);
			ShopTimerText->SetText(FText::FromString(timeText));

			if (InRemainingSeconds == 0)
			{
				RefreshShopIfNeeded();
			}
		}
	}
}

