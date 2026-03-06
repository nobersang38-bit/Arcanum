// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/InventorySlot.h"
#include "Components/Textblock.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

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

void UInventorySlot::CreateWeaponItems(TArray<FEquipmentInfo> WeaponList)
{
    for (int32 i = 0; i < WeaponList.Num(); i++)
    {
        USquareSlotWidget* NewSlot = CreateWidget<USquareSlotWidget>(GetWorld(), USquareSlotWidgetClass);
        UWrapBoxSlot* WrapSlot = EquipGridPanel->AddChildToWrapBox(NewSlot);

        if (WrapSlot)
        {
            WrapSlot->SetHorizontalAlignment(HAlign_Fill);
            WrapSlot->SetVerticalAlignment(VAlign_Fill);
        }
    }
}

// ========================================================
// 장착 버튼 클릭
// ========================================================
void UInventorySlot::ClickEquipSetupBtn()
{
    OnSetupBtnClicked.Broadcast();
}


