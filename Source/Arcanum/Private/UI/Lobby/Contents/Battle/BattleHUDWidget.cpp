#include "UI/Lobby/Contents/Battle/BattleHUDWidget.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/SubLayout/InventoryItemSlotWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "UI/Lobby/Contents/Battle/ItemSlot.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Lobby/Contents/Battle/StageList.h"
#include "Components/ScrollBox.h"
#include "Components/Spacer.h"

#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"

//#include "Kismet/GameplayStatics.h"

void UBattleHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EquippedPotionSlots.Reset();
	EquippedPotionSlots.Add(EquippedItemSlot1);
	EquippedPotionSlots.Add(EquippedItemSlot2);

	EquippedPotionSlotDatas.Reset();
	EquippedPotionSlotDatas.SetNum(EquippedPotionSlots.Num());

	if (EquippedItemSlot1)
	{
		EquippedItemSlot1->OnInventorySlotClicked.RemoveDynamic(this, &UBattleHUDWidget::HandleEquippedPotionSlot1Clicked);
		EquippedItemSlot1->OnInventorySlotClicked.AddDynamic(this, &UBattleHUDWidget::HandleEquippedPotionSlot1Clicked);
	}

	if (EquippedItemSlot2)
	{
		EquippedItemSlot2->OnInventorySlotClicked.RemoveDynamic(this, &UBattleHUDWidget::HandleEquippedPotionSlot2Clicked);
		EquippedItemSlot2->OnInventorySlotClicked.AddDynamic(this, &UBattleHUDWidget::HandleEquippedPotionSlot2Clicked);
	}

	if (ItemListSlot)
	{
		ItemListSlot->OnSetItemBtnClicked.RemoveDynamic(this, &UBattleHUDWidget::HandleSetItemBtnClicked);
		ItemListSlot->OnSetItemBtnClicked.AddDynamic(this, &UBattleHUDWidget::HandleSetItemBtnClicked);

		ItemListSlot->OnRemoveItemBtnClicked.RemoveDynamic(this, &UBattleHUDWidget::HandleRemoveItemBtnClicked);
		ItemListSlot->OnRemoveItemBtnClicked.AddDynamic(this, &UBattleHUDWidget::HandleRemoveItemBtnClicked);

		if (UInventoryHUDWidget* battleInventoryWidget = ItemListSlot->GetBattleInventoryWidget())
		{
			battleInventoryWidget->SetParentLobby(ParentLobby);

			battleInventoryWidget->OnInventorySlotSelected.RemoveDynamic(this, &UBattleHUDWidget::HandleInventorySlotSelected);
			battleInventoryWidget->OnInventorySlotSelected.AddDynamic(this, &UBattleHUDWidget::HandleInventorySlotSelected);

			battleInventoryWidget->SetCategoryPanelVisible(false);
		}

		ItemListSlot->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 유닛 슬롯  (Slot Index 모두 1로 설정)
	EquippedUnitSlot1->OnSlotClicked.AddDynamic(this, &UBattleHUDWidget::OnBattleSlotClicked);
	EquippedUnitSlot2->OnSlotClicked.AddDynamic(this, &UBattleHUDWidget::OnBattleSlotClicked);
	EquippedUnitSlot3->OnSlotClicked.AddDynamic(this, &UBattleHUDWidget::OnBattleSlotClicked);
	EquippedUnitSlot4->OnSlotClicked.AddDynamic(this, &UBattleHUDWidget::OnBattleSlotClicked);

	// 게임 시작
	if (EnterGameBtn) {
		EnterGameBtn->OnClicked.RemoveDynamic(this, &UBattleHUDWidget::EnterGame);
		EnterGameBtn->OnClicked.AddDynamic(this, &UBattleHUDWidget::EnterGame);
	}

	if (UnitListSlot)
	{
		UnitListSlot->OnSetItemBtnClicked.RemoveDynamic(this, &UBattleHUDWidget::SetUnit);
		UnitListSlot->OnSetItemBtnClicked.AddDynamic(this, &UBattleHUDWidget::SetUnit);
	}

	if (FPlayerAccountService::GetStageData(this, StageDatas)) {
		if (!StageScrollBox || !StageListClass) return;
		StageWidgets.Empty();

		for (FDTStageDataRow* RowPtr : StageDatas) {
			if (!RowPtr) continue;

			UStageList* StageWidget = CreateWidget<UStageList>(this, StageListClass);
			if (StageWidget) {
				StageWidget->StageTag = RowPtr->StageData.StageTag;
				StageWidget->StgName = RowPtr->StageData.StageName;
				StageWidget->StgInfo = RowPtr->StageData.StageDesc;
				StageWidget->StageImg = RowPtr->StageData.StageImg.LoadSynchronous();
				StageWidget->StageImg = RowPtr->StageData.StageImg.Get();
				StageWidget->OnStageClicked.RemoveDynamic(this, &UBattleHUDWidget::OnStageClicked);
				StageWidget->OnStageClicked.AddDynamic(this, &UBattleHUDWidget::OnStageClicked);
				StageScrollBox->AddChild(StageWidget);
				StageWidgets.Add(StageWidget);

				USpacer* Spacer = NewObject<USpacer>(this);
				if (Spacer) {
					Spacer->SetSize(FVector2D(0.f, 10.f));
					StageScrollBox->AddChild(Spacer);
				}
			}
		}
	}

	if (StageWidgets.Num() > 0 && StageWidgets[0]) OnStageClicked(StageWidgets[0]);

	RefreshBattlePotionSlots();
}

void UBattleHUDWidget::SetParentLobby(ULobbyHUD* InLobby)
{
	ParentLobby = InLobby;

	if (ItemListSlot)
	{
		if (UInventoryHUDWidget* battleInventoryWidget = ItemListSlot->GetBattleInventoryWidget())
		{
			battleInventoryWidget->SetParentLobby(InLobby);
		}
	}
}

void UBattleHUDWidget::SetUnit()
{
	if (UnitListSlot)
	{
		UnitListSlot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBattleHUDWidget::OnBattleSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex)
{
	OnShowUnitList.Broadcast(SlotIndex);

	if (SlotIndex == 0)
	{
		UnitListSlot->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (SlotIndex == 1 && UnitListSlot)
	{
		UnitListSlot->SetVisibility(ESlateVisibility::Visible);
	}
}

void UBattleHUDWidget::EnterGame()
{
	FPlayerAccountService::SetHUDIndex(this, EHUDIndex::BattleMenu);
	FPlayerAccountService::SetCurrentStageTag(this, CurrentSelectedStage->StageTag);

	FDTStageDataRow** FoundRowPtr = StageDatas.FindByPredicate([&](const FDTStageDataRow* Row) { return Row && Row->StageData.StageTag == CurrentSelectedStage->StageTag; });
	if (FoundRowPtr && *FoundRowPtr) {
		FDTStageDataRow* TargetRow = *FoundRowPtr;

		//if (LoadingWidgetClass)
		//{
		//	LoadingWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), LoadingWidgetClass);
		//	if (LoadingWidgetInstance)
		//	{
		//		LoadingWidgetInstance->AddToViewport(1000); // ZOrder 높게
		//	}
		//}
		//FName LevelName = TargetRow->StageData.StageLevel->GetFName();

		//UGameplayStatics::LoadStreamLevel(this, LevelName, true, false, FLatentActionInfo());

		FPlayerAccountService::ChangedLevel(this, TargetRow->StageData.StageLevel);

		UE_LOG(LogTemp, Log, TEXT("게임 시작"));
	}
}

void UBattleHUDWidget::OnStageClicked(UStageList* ClickedStage)
{
	if (CurrentSelectedStage) CurrentSelectedStage->SetSelected(false);
	CurrentSelectedStage = ClickedStage;
	if (CurrentSelectedStage) CurrentSelectedStage->SetSelected(true);
}

void UBattleHUDWidget::HandleEquippedPotionSlot1Clicked(int32 InIgnoredIndex)
{
	HandleEquippedPotionSlotClicked(0);
}

void UBattleHUDWidget::HandleEquippedPotionSlot2Clicked(int32 InIgnoredIndex)
{
	HandleEquippedPotionSlotClicked(1);
}

void UBattleHUDWidget::HandleEquippedPotionSlotClicked(int32 InSlotIndex)
{
	if (ItemListSlot &&	SelectedPotionSlotIndex == InSlotIndex && ItemListSlot->GetVisibility() == ESlateVisibility::Visible)
	{
		SelectedPotionSlotIndex = INDEX_NONE;
		RefreshEquippedPotionSlotSelection();
		ItemListSlot->SetVisibility(ESlateVisibility::Collapsed);

		return;
	}

	SelectedPotionSlotIndex = InSlotIndex;
	RefreshEquippedPotionSlotSelection();

	if (ItemListSlot)
	{
		ItemListSlot->SetVisibility(ESlateVisibility::Visible);

		if (UInventoryHUDWidget* battleInventoryWidget = ItemListSlot->GetBattleInventoryWidget())
		{
			ItemListSlot->ShowBattleInventory();
		}
	}
}

void UBattleHUDWidget::HandleInventorySlotSelected(const FInventoryViewSlot& InSlot)
{
	SelectedInventorySlot = InSlot;
}

void UBattleHUDWidget::HandleSetItemBtnClicked()
{
	if (SelectedPotionSlotIndex == INDEX_NONE) return;
	if (SelectedInventorySlot.Type != EInventoryViewSlotType::StackItem) return;
	if (!SelectedInventorySlot.ItemTag.IsValid()) return;

	const int32 setCount = FMath::Min(SelectedInventorySlot.StackCount, EquippedPotionCount);

	const bool bSuccess = FPlayerAccountService::SetBattlePotionSlot(this, SelectedPotionSlotIndex, SelectedInventorySlot.ItemTag, setCount);

	if (bSuccess)
	{
		if (ParentLobby)
		{
			ParentLobby->RefreshAllLobbyUI();
		}

		RefreshBattlePotionSlots();

		SelectedInventorySlot = FInventoryViewSlot();

		if (ItemListSlot)
		{
			if (UInventoryHUDWidget* battleInventoryWidget = ItemListSlot->GetBattleInventoryWidget())
			{
				battleInventoryWidget->ClearSelection();
			}
		}

		HidePotionInventory();
	}
}

void UBattleHUDWidget::HandleRemoveItemBtnClicked()
{
	if (SelectedPotionSlotIndex == INDEX_NONE) return;

	const bool bSuccess = FPlayerAccountService::ClearBattlePotionSlot(this, SelectedPotionSlotIndex);

	if (bSuccess)
	{
		if (ParentLobby)
		{
			ParentLobby->RefreshAllLobbyUI();
		}

		RefreshBattlePotionSlots();

		SelectedInventorySlot = FInventoryViewSlot();

		if (ItemListSlot)
		{
			if (UInventoryHUDWidget* battleInventoryWidget = ItemListSlot->GetBattleInventoryWidget())
			{
				battleInventoryWidget->ClearSelection();
			}
		}

		HidePotionInventory();
	}
}


void UBattleHUDWidget::RefreshBattlePotionSlots()
{
	if (!ParentLobby) return;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();

	if (EquippedPotionSlotDatas.Num() < EquippedPotionSlots.Num())
	{
		EquippedPotionSlotDatas.SetNum(EquippedPotionSlots.Num());
	}

	for (int32 slotIndex = 0; slotIndex < EquippedPotionSlots.Num(); slotIndex++)
	{
		FInventoryViewSlot viewSlot;

		if (playerData.BattlePotionSlots.IsValidIndex(slotIndex))
		{
			const FBattlePotionSlotData& potionSlotData = playerData.BattlePotionSlots[slotIndex];

			if (potionSlotData.PotionTag.IsValid() && potionSlotData.Count > 0)
			{
				viewSlot.Type = EInventoryViewSlotType::StackItem;
				viewSlot.ItemTag = potionSlotData.PotionTag;
				viewSlot.StackCount = potionSlotData.Count;

				if (const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, potionSlotData.PotionTag))
				{
					viewSlot.Icon = catalogRow->Icon;
				}
			}
		}

		EquippedPotionSlotDatas[slotIndex] = viewSlot;

		if (UInventoryItemSlotWidget* slotWidget = EquippedPotionSlots[slotIndex])
		{
			if (viewSlot.Type == EInventoryViewSlotType::Empty)
			{
				slotWidget->ClearSlot(slotIndex);
			}
			else
			{
				slotWidget->SetSlotData(viewSlot, slotIndex);
			}
		}
	}
}

void UBattleHUDWidget::ShowPotionInventory()
{
	if (ItemListSlot)
	{
		ItemListSlot->SetVisibility(ESlateVisibility::Visible);
		ItemListSlot->ShowBattleInventory();
	}
}

void UBattleHUDWidget::HidePotionInventory()
{
	if (ItemListSlot)
	{
		ItemListSlot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBattleHUDWidget::RefreshEquippedPotionSlotSelection()
{
	for (int32 slotIndex = 0; slotIndex < EquippedPotionSlots.Num(); slotIndex++)
	{
		if (UInventoryItemSlotWidget* slot = EquippedPotionSlots[slotIndex])
		{
			slot->SetSelected(slotIndex == SelectedPotionSlotIndex);
		}
	}
}