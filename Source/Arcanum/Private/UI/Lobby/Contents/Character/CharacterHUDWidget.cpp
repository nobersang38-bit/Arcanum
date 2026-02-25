#include "UI/Lobby/Contents/Character/CharacterHUDWidget.h"
#include "UI/Lobby/Contents/Character/RoundedSlotWidget.h"
#include "UI/Lobby/Contents/Character/CharacterInfo.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"
#include "UI/Common/CommonDialog.h"

void UCharacterHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CharacterInfo)
    {
        CharacterInfo->OnEnhanceBtnClicked.AddDynamic(this, &UCharacterHUDWidget::ShowEnhancementConfirm);
    }
    
    if (!CharacterGridPanel || !RoundedSlotWidgetClass)
        return;

    int32 NumColumns = 3;

    // 캐릭터창 테스트용
    for (int32 Index = 0; Index < 5; ++Index)
    {
        // 1️ 동적 생성
        URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
        if (!NewSlot)
            continue;

        // 2️ GridPanel에 추가
        UUniformGridSlot* GridSlot = CharacterGridPanel->AddChildToUniformGrid(NewSlot);

        if (GridSlot)
        {
            // 3️ 위치 지정
            GridSlot->SetRow(Index / NumColumns);
            GridSlot->SetColumn(Index % NumColumns);
        }
    }

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
        //UE_LOG(LogTemp, Log, TEXT("ok 클릭"));
        OnOkCharacterEnhance.Broadcast();
    }
    else if (res == EDialogResult::Cancel)
    {
        EnhancementConfirm->SetVisibility(ESlateVisibility::Hidden);
    }
}
