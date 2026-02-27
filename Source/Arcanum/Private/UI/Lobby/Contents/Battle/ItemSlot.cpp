// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Battle/ItemSlot.h"
#include "Components/TextBlock.h"
#include "UI/Common/CommonBtnWidget.h"

void UItemSlot::NativePreConstruct()
{
    Super::NativePreConstruct();

    FText DefaultName = FText::FromString(TEXT("아이템 이름"));
    FText DefaultInfo = FText::FromString(TEXT("아이템 설명"));

    if (ItemNameText)
    {
        ItemNameText->SetText(ItemNameTxt.IsEmpty() ? DefaultName : ItemNameTxt);
    }

    if (ItemInfoText)
    {
        ItemInfoText->SetText(ItemInfoTxt.IsEmpty() ? DefaultInfo : ItemInfoTxt);
    }
}

void UItemSlot::NativeConstruct()
{
    if (SetItemBtn) {
        SetItemBtn->OnClicked.RemoveDynamic(this, &UItemSlot::ClickSetItemBtn);
        SetItemBtn->OnClicked.AddDynamic(this, &UItemSlot::ClickSetItemBtn);
    }

    
}

void UItemSlot::ClickSetItemBtn()
{
    OnSetItemBtnClicked.Broadcast();
}
