#include "UI/Lobby/Contents/Character/CharacterHUDWidget.h"
#include "UI/Lobby/Contents/Character/RoundedSlotWidget.h"
#include "Components/UniformGridSlot.h"
#include "Components/UniformGridPanel.h"

void UCharacterHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!CharacterGridPanel || !RoundedSlotWidgetClass)
        return;

    int32 NumColumns = 3;

    for (int32 Index = 0; Index < 10; ++Index)
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
}
