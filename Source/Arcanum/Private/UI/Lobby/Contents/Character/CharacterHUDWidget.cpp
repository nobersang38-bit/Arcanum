#include "UI/Lobby/Contents/Character/CharacterHUDWidget.h"
#include "UI/Lobby/Contents/Character/RoundedSlotWidget.h"
#include "UI/Lobby/Contents/Character/CharacterInfo.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "UI/Lobby/Contents/Character/InventorySlot.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Lobby/LobbyHUD.h"

void UCharacterHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 무기 슬롯 (Slot Index 모두 0으로 설정)
    Weapon1Slot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSlotClicked);
    Weapon2Slot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSlotClicked);
    LegendaryWeaponSlot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSlotClicked);

    // 장비 슬롯 (순서대로 1,2,3,4)
    HelmetSlot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSlotClicked);
    ChestSlot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSlotClicked);
    GloveSlot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSlotClicked);
    BootsSlot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSlotClicked);

    if (CharacterInfo)
    {
        CharacterInfo->OnEnhanceBtnClicked.AddDynamic(this, &UCharacterHUDWidget::ShowEnhancementConfirm);
    }
    
    if (WeaponList)
    {
        WeaponList->OnSetupBtnClicked.AddDynamic(this, &UCharacterHUDWidget::SetupEquipment);
    }

    if (EquipmentList)
    {
        EquipmentList->OnSetupBtnClicked.AddDynamic(this, &UCharacterHUDWidget::SetupEquipment);
    }

    if (!CharacterGridPanel || !RoundedSlotWidgetClass)
        return;

    int32 NumColumns = 3;

    // 캐릭터창 테스트용
    //for (int32 Index = 0; Index < 5; ++Index)
    //{
    //    // 1️ 동적 생성
    //    URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
    //    if (!NewSlot)
    //        continue;

    //    // 2️ GridPanel에 추가
    //    UUniformGridSlot* GridSlot = CharacterGridPanel->AddChildToUniformGrid(NewSlot);

    //    if (GridSlot)
    //    {
    //        // 3️ 위치 지정
    //        GridSlot->SetRow(Index / NumColumns);
    //        GridSlot->SetColumn(Index % NumColumns);
    //    }
    //}

    // 유닛창 테스트용
      for (int32 Index = 0; Index < 8; ++Index)
    {
        // 1️ 동적 생성
        URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
        if (!NewSlot)
            continue;

        // 2️ GridPanel에 추가
        UUniformGridSlot* GridSlot = UnitGridPanel->AddChildToUniformGrid(NewSlot);

        if (GridSlot)
        {
            // 3️ 위치 지정
            GridSlot->SetRow(Index / NumColumns);
            GridSlot->SetColumn(Index % NumColumns);
        }
    }
   
}

FReply UCharacterHUDWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return FReply::Handled();
}

void UCharacterHUDWidget::InitCharacterHUD()
{
    int32 NumColumns = 3;

    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
        
        TSoftObjectPtr<UTexture2D> CharacterIconSoftPtr = ParentLobby->CachedPlayerData.OwnedCharacters[i].CharacterInfo.BattleCharacterInitData.CharacterIcon;
        UTexture2D* CharacterIcon = CharacterIconSoftPtr.LoadSynchronous();

        int GetCurrentGrade = ParentLobby->CachedPlayerData.OwnedCharacters[i].CharacterInfo.CurrentGrade; // 0 이면 보유X , 0 초과는 보유 및 강화
        bool hasOwned = false;

        if (GetCurrentGrade > 0)
            hasOwned = true;

        NewSlot->SetIconImage(CharacterIcon, hasOwned);

        if (!NewSlot)
            continue;

        // 2️ GridPanel에 추가
        UUniformGridSlot* GridSlot = CharacterGridPanel->AddChildToUniformGrid(NewSlot);

        if (GridSlot)
        {
            // 3️ 위치 지정
            GridSlot->SetRow(i / NumColumns);
            GridSlot->SetColumn(i % NumColumns);
        }
    }
}

// ========================================================
// 캐릭터창 - 강화하기 버튼
// ========================================================

void UCharacterHUDWidget::ShowEnhancementConfirm()
{
    // 강화확인창 띄우기
    if (EnhancementConfirm)
    {
        EnhancementConfirm->SetVisibility(ESlateVisibility::Visible);

        EnhancementConfirm->OnResult.RemoveDynamic(this, &UCharacterHUDWidget::OnEnhancementCommonDialog);
        EnhancementConfirm->OnResult.AddDynamic(this, &UCharacterHUDWidget::OnEnhancementCommonDialog);
    }
}


void UCharacterHUDWidget::OnEnhancementCommonDialog(EDialogResult res)
{
    if (res == EDialogResult::OK)
    {
        OnEnhanceOKClicked.Broadcast();
        EnhancementConfirm->SetVisibility(ESlateVisibility::Hidden);
    }
    else if (res == EDialogResult::Cancel)
    {
        EnhancementConfirm->SetVisibility(ESlateVisibility::Hidden);
    }
}

// ========================================================
// 무기, 장비 슬롯 클릭
// ========================================================

void UCharacterHUDWidget::OnSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex)
{
    UE_LOG(LogTemp, Warning, TEXT("Clicked Slot Index: %d"), SlotIndex);

    switch (SlotIndex)
    {
    case 0:
        UE_LOG(LogTemp, Warning, TEXT("무기 슬롯 Clicked"));
        if (CharacterSwitcher)
        {
            CharacterSwitcher->SetActiveWidgetIndex(1);
        }
        break;

    case 1:
        UE_LOG(LogTemp, Warning, TEXT("투구 슬롯 Clicked"));
        if (CharacterSwitcher)
        {
            CharacterSwitcher->SetActiveWidgetIndex(2);
        }
        break;

    case 2:
        UE_LOG(LogTemp, Warning, TEXT("갑옷 슬롯Clicked"));
        break;

    case 3:
        UE_LOG(LogTemp, Warning, TEXT("장갑 슬롯 Clicked"));
        break;

    case 4:
        UE_LOG(LogTemp, Warning, TEXT("신발 슬롯 Clicked"));
        break;

    default:
        if (CharacterSwitcher)
        {
            CharacterSwitcher->SetActiveWidgetIndex(0);
        }
        break;
    }
}

// ========================================================
// 무기, 장비 장착 버튼클릭
// ========================================================

void UCharacterHUDWidget::SetupEquipment()
{
    if (CharacterSwitcher)
    {
        CharacterSwitcher->SetActiveWidgetIndex(0);
    }
}

