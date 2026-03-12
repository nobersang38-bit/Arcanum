// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/RoundedSlotWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"

void URoundedSlotWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    //if (BackgroundColor) BackgroundColor->SetBrushColor(RoundColor);
    //if (IconImage && IconImg) IconImage->SetBrushFromTexture(IconImg);
}

FReply URoundedSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    OnCharacterSlotClicked.Broadcast(this, SlotCharacterName, SlotCharacterOwned);
    return FReply::Handled();
}

// ========================================================
// 캐릭터 슬롯 배경색 설정
// ========================================================
void URoundedSlotWidget::SetRoundBackgroundColor(FLinearColor NewColor)
{
    if (BackgroundColor)
    {
        BackgroundColor->SetBrushColor(NewColor);
    }
}

// ========================================================
// 캐릭터 아이콘 설정
// ========================================================
void URoundedSlotWidget::SetIconImage(UTexture2D* CharacterIcon, bool OwnedCharacter, FName CharacterName, FGameplayTag InTag)
{
    if (!IconImage) return;

    FSlateBrush Brush;
    if (CharacterIcon)
    {
        Brush.SetResourceObject(CharacterIcon);

        // 보유 캐릭터 구분
        // 나중에 json 값으로 구분하기
        float TargetAlpha = OwnedCharacter ? 0.0f : 0.8f;
        FLinearColor CurrentColor = SlotDimOverlay->GetColorAndOpacity();
        CurrentColor.A = TargetAlpha;
        SlotDimOverlay->SetColorAndOpacity(CurrentColor);

        SlotCharacterName = CharacterName;
        SlotCharacterOwned = OwnedCharacter;
    }
    else
    {
        Brush.TintColor = FSlateColor(FLinearColor::Black);
    }
    IconImage->SetBrush(Brush);
    /// 260311 변경 : 추가 (태그 사용 할 수 있게 변경)
    Tag = InTag;
}
