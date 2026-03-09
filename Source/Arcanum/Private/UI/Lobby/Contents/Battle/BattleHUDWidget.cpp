#include "UI/Lobby/Contents/Battle/BattleHUDWidget.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
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

    // 아이템 슬롯 (Slot Index 모두 0으로 설정)
    EquippedItemSlot1->OnSlotClicked.AddDynamic(this, &UBattleHUDWidget::OnBattleSlotClicked);
    EquippedItemSlot2->OnSlotClicked.AddDynamic(this, &UBattleHUDWidget::OnBattleSlotClicked);

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

    if (ItemListSlot)
    {
        ItemListSlot->OnSetItemBtnClicked.RemoveDynamic(this, &UBattleHUDWidget::SetItemUnit);
        ItemListSlot->OnSetItemBtnClicked.AddDynamic(this, &UBattleHUDWidget::SetItemUnit);
    }
    if (UnitListSlot)
    {
        UnitListSlot->OnSetItemBtnClicked.RemoveDynamic(this, &UBattleHUDWidget::SetItemUnit);
        UnitListSlot->OnSetItemBtnClicked.AddDynamic(this, &UBattleHUDWidget::SetItemUnit);
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

void UBattleHUDWidget::SetItemUnit()
{
    if (ItemListSlot && UnitListSlot)
    {
        ItemListSlot->SetVisibility(ESlateVisibility::Collapsed);
        UnitListSlot->SetVisibility(ESlateVisibility::Collapsed);
    }

}

void UBattleHUDWidget::OnBattleSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex)
{
    OnShowItemUnitList.Broadcast(SlotIndex);

    if (SlotIndex== 0 && ItemListSlot)
    {
        ItemListSlot->SetVisibility(ESlateVisibility::Visible);
        UnitListSlot->SetVisibility(ESlateVisibility::Collapsed);
    }
    else if (SlotIndex == 1 && UnitListSlot)
    {
        ItemListSlot->SetVisibility(ESlateVisibility::Collapsed);
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