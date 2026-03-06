#include "UI/Lobby/Contents/Battle/BattleHUDWidget.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "UI/Lobby/Contents/Battle/ItemSlot.h"
#include "UI/Common/CommonBtnWidget.h"

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
        ItemListSlot->OnSetItemBtnClicked.AddDynamic(this, &UBattleHUDWidget::SetItemUnit);
    }
    if (UnitListSlot)
    {
        UnitListSlot->OnSetItemBtnClicked.AddDynamic(this, &UBattleHUDWidget::SetItemUnit);
    }
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
    // 선택한 스테이지, 장착한 아이템, 장착한 유닛 정보 가져와야함
    UE_LOG(LogTemp, Log, TEXT("게임 시작"));
}
