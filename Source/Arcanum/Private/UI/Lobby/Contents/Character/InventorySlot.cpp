// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/InventorySlot.h"
#include "Components/Textblock.h"
#include "UI/Common/CommonBtnWidget.h"

void UInventorySlot::NativePreConstruct()
{
	Super::NativePreConstruct();

    FText DefaultName = FText::FromString(TEXT("장비 이름"));
    FText DefaultInfo = FText::FromString(TEXT("공격력"));

    if (EquipNameText)
    {
        EquipNameText->SetText(EquipNameTxt.IsEmpty() ? DefaultName : EquipNameTxt);
    }

    if (EquipInfoText)
    {
        EquipInfoText->SetText(EquipInfoTxt.IsEmpty() ? DefaultInfo : EquipInfoTxt);
    }
}

void UInventorySlot::NativeConstruct()
{
    if (SetupBtn) {
        SetupBtn->OnClicked.RemoveDynamic(this, &UInventorySlot::ClickEquipSetupBtn);
        SetupBtn->OnClicked.AddDynamic(this, &UInventorySlot::ClickEquipSetupBtn);
    }
}

// ========================================================
// 장착 버튼 클릭
// ========================================================
void UInventorySlot::ClickEquipSetupBtn()
{
    OnSetupBtnClicked.Broadcast();
}
