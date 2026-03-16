#include "UI/Lobby/Contents/Battle/BattleHUDWidget.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/SubLayout/InventoryItemSlotWidget.h"
#include "UI/Lobby/Contents/Battle/ItemSlot.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Lobby/Contents/Battle/StageList.h"
#include "Components/ScrollBox.h"
#include "Components/Spacer.h"

#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"

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
	if (EquippedPotionSlots.IsValidIndex(InSlotIndex))
	{
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
}

void UBattleHUDWidget::HandleInventorySlotSelected(const FInventoryViewSlot& InSlot)
{
	SelectedInventorySlot = InSlot;
}

void UBattleHUDWidget::HandleSetItemBtnClicked()
{
	EquipSelectedPotionToSlot();
}

void UBattleHUDWidget::EquipSelectedPotionToSlot()
{
	if (!EquippedPotionSlots.IsValidIndex(SelectedPotionSlotIndex))	return;
	if (!EquippedPotionSlotDatas.IsValidIndex(SelectedPotionSlotIndex)) return;
	if (SelectedInventorySlot.Type != EInventoryViewSlotType::StackItem) return;
	if (!SelectedInventorySlot.ItemTag.IsValid()) return;
	if (SelectedInventorySlot.StackCount < EquippedPotionCount) return;

	for (int32 slotIndex = 0; slotIndex < EquippedPotionSlotDatas.Num(); slotIndex++)
	{
		if (slotIndex != SelectedPotionSlotIndex)
		{
			const FInventoryViewSlot& equippedSlotData = EquippedPotionSlotDatas[slotIndex];

			if (equippedSlotData.Type == EInventoryViewSlotType::StackItem &&
				equippedSlotData.ItemTag.MatchesTagExact(SelectedInventorySlot.ItemTag))
			{
				return;
			}
		}
	}

	FInventoryViewSlot equipSlot = SelectedInventorySlot;
	equipSlot.StackCount = EquippedPotionCount;

	EquippedPotionSlotDatas[SelectedPotionSlotIndex] = equipSlot;
	if (UInventoryItemSlotWidget* targetSlot = EquippedPotionSlots[SelectedPotionSlotIndex])
	{
		targetSlot->SetSlotData(equipSlot, SelectedPotionSlotIndex);
		targetSlot->SetSelected(true);
	}

	HidePotionInventory();
}
void UBattleHUDWidget::ShowPotionInventory()
{
	if (ItemListSlot)
	{
		ItemListSlot->SetVisibility(ESlateVisibility::Visible);

		if (UInventoryHUDWidget* battleInventoryWidget = ItemListSlot->GetBattleInventoryWidget())
		{
			battleInventoryWidget->RefreshStackInventory();
		}
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