// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/InventorySlot.h"
#include "Components/Textblock.h"
#include "UI/Common/CommonBtnWidget.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"

#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "DataInfo/BattleCharacter/Equipment/Data/FEquipmentData.h"

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
// 인벤토리에 보유중인 무기, 장비 출력하기 
// ========================================================
void UInventorySlot::CreateWeaponItems(TArray<FEquipmentInfo> WeaponList, const FString& TargetPath)
{
    FGameplayTag ListItemTag;

    if (!USquareSlotWidgetClass)
        return;

    for (int32 i = 0; i < WeaponList.Num(); i++)
    {
        USquareSlotWidget* NewSlot = CreateWidget<USquareSlotWidget>(this, USquareSlotWidgetClass);
        WeaponInventoryItemIcon = nullptr;

        ListItemTag = WeaponList[i].ItemTag;

        if (!DataSubsystem)
        {
            if (UARGameInstance* GI = Cast<UARGameInstance>(GetGameInstance()))
            {
                DataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
            }
        }

        if (UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance()))
        {
            if (UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>())
            {
                UDataTable* const* tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment);
                if (!tablePtr || !(*tablePtr)) { return; }

                UDataTable* table = *tablePtr;

                TArray<FDTEquipmentInfoRow*> rows;
                table->GetAllRows(TEXT("BuildEquipmentRowCache"), rows);

                for (FDTEquipmentInfoRow* row : rows)
                {
                    if (row && row->ItemTag.IsValid())
                    {
                        if (row->ItemTag == ListItemTag)
                        {
                            WeaponInventoryItemIcon = row->Icon.LoadSynchronous();
                            EquipNameTxt = row->Desc;
                            // 무기만 출력되는 if문 SlotTag가 Weapon인거
                            if (IsSpecificSlotType(row->SlotTag, TargetPath))
                            {
                                if (NewSlot)
                                {
                                    NewSlot->SetItemIconImage(WeaponInventoryItemIcon);
                                    NewSlot->SetItemName(EquipNameTxt);
                                    NewSlot->SetWeaponTag(row->ItemTag);
                                    NewSlot->SetWeaponGuid(WeaponList[i].ItemGuid);
                                    NewSlot->OnSlotClicked.AddDynamic(this, &UInventorySlot::OnSlotClicked);
                                    InventoryEquipmentSlots.Add(NewSlot);
                                    UWrapBoxSlot* WrapSlot = EquipGridPanel->AddChildToWrapBox(NewSlot);

                                    if (WrapSlot)
                                    {
                                        WrapSlot->SetHorizontalAlignment(HAlign_Fill);
                                        WrapSlot->SetVerticalAlignment(VAlign_Fill);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// ========================================================
// 장착 버튼 클릭
// ========================================================
void UInventorySlot::ClickEquipSetupBtn()
{
    // 장착된 캐릭터랑 클릭된 무기,장비 가져오기 
    //FGameplayTag ClickedItemTag= ClickedSlot->GetWeaponTag();

    OnSetupBtnClicked.Broadcast(CurrentSelectedSlot);
}

// ========================================================
// 인벤토리 아이템 타입 구분 (무기인지 장비인지)
// ========================================================
bool UInventorySlot::IsSpecificSlotType(const FGameplayTag& InTag, const FString& TargetPath)
{
    if (!InTag.IsValid()) return false;

    FString Left, Right;
    if (InTag.GetTagName().ToString().Split(TEXT("."), &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
    {

        return Left.Equals(TargetPath, ESearchCase::IgnoreCase);
    }
    return false;
}

// ========================================================
// 무기, 장비 클릭
// ========================================================
void UInventorySlot::OnSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex)
{
    if (!ClickedSlot) return;
    CurrentSelectedSlot = ClickedSlot;

    for (USquareSlotWidget* EquipSlot : InventoryEquipmentSlots)
    {
        if (EquipSlot)
        {
            // 선택된 슬롯은 핑크, 나머지는 화이트
            FLinearColor TargetColor = (EquipSlot == ClickedSlot) ? FLinearColor(1.0f, 0.4f, 0.7f, 1.0f) : FLinearColor::White;
            EquipSlot->SetSquareBackgroundColor(TargetColor);
        }
        FText ClickedItemName = ClickedSlot->GetItemName();
        EquipNameText->SetText(ClickedItemName);
    }
}


