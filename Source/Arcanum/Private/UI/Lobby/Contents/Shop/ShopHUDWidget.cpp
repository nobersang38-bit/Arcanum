#include "UI/Lobby/Contents/Shop/ShopHUDWidget.h"
#include "UI/Lobby/Contents/Shop/SubLayout/ShopItemSlotWidget.h"
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

void UShopHUDWidget::ApplyShopData(const TArray<FName>& InRowNames, const TArray<bool>& InSoldOutStates, const TArray<const FDTEquipmentInfoRow*>& InRowPtrs)
{
	for (int32 slotIndex = 0; slotIndex < ShopSlotCount; slotIndex++)
	{
		if (!ShopSlots.IsValidIndex(slotIndex))
		{
			continue;
		}

		UShopItemSlotWidget* slot = ShopSlots[slotIndex];
		if (!slot)
		{
			continue;
		}

		// 데이터 부족/비정상이면 빈 슬롯 처리
		if (!InRowNames.IsValidIndex(slotIndex) || !InSoldOutStates.IsValidIndex(slotIndex) || !InRowPtrs.IsValidIndex(slotIndex))
		{
			slot->ClearSlot();
			continue;
		}

		const FName rowName = InRowNames[slotIndex];
		const bool bSoldOut = InSoldOutStates[slotIndex];
		const FDTEquipmentInfoRow* rowPtr = InRowPtrs[slotIndex];

		if (!rowPtr || rowName == NAME_None)
		{
			slot->ClearSlot();
			continue;
		}

		// 슬롯 위젯은 표시만 하게, RowPtr로 세팅
		slot->SetViewData(slotIndex, rowName, rowPtr, bSoldOut);

		// 선택 강조
		slot->SetSelected(slotIndex == SelectedShopSlotIndex);
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
					OnBuyRequested.Broadcast(SelectedShopSlotIndex);
				}
			}
		}
	}
}

void UShopHUDWidget::HandleSellClicked()
{
	OnSellRequested.Broadcast();
}
