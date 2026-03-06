#include "UI/Lobby/Contents/Character/CharacterHUDWidget.h"
#include "UI/Lobby/Contents/Character/RoundedSlotWidget.h"
#include "UI/Lobby/Contents/Character/CharacterInfo.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "UI/Lobby/Contents/Character/InventorySlot.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Lobby/LobbyHUD.h"
#include "GameplayTags/ArcanumTags.h"

#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/BattleCharacter/BattleStats/DataTable/DTBattleStats.h"

void UCharacterHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GI = Cast<UARGameInstance>(GetGameInstance());
    if (GI)
    {
        DataSubsystem = GI->GetSubsystem<UGameDataSubsystem>();
    }

    EquipmentSlots.Add(Weapon1Slot);
    EquipmentSlots.Add(Weapon2Slot);
    EquipmentSlots.Add(LegendaryWeaponSlot);
    EquipmentSlots.Add(HelmetSlot);
    EquipmentSlots.Add(ChestSlot);
    EquipmentSlots.Add(GloveSlot);
    EquipmentSlots.Add(BootsSlot);

    for (auto& EquipSlot : EquipmentSlots)
    {
        EquipSlot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSquareSlotClicked);
    }

    if (CharacterInfo)
    {
        CharacterInfo->OnEnhanceBtnClicked.AddDynamic(this, &UCharacterHUDWidget::CharacterEnhancement);
        CharacterInfo->OnSetPlayerBtnClicked.AddDynamic(this, &UCharacterHUDWidget::SetPlayerCharacter);
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

    /// Test : 유닛창 테스트용
      for (int32 Index = 0; Index < 12; ++Index)
    {
        URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
        if (!NewSlot)
            continue;

        UWrapBoxSlot* WrapSlot = UnitGridPanel->AddChildToWrapBox(NewSlot);
        if (WrapSlot)
        {
            WrapSlot->SetHorizontalAlignment(HAlign_Fill);
            WrapSlot->SetVerticalAlignment(VAlign_Fill);
        }
    }
}

// ========================================================
// 캐릭터창 초기화
// ========================================================

void UCharacterHUDWidget::InitCharacterHUD()
{
    // 캐릭터 목록창 생성하기
    int32 SelectedIndex = INDEX_NONE;

    CharacterGridPanel->ClearChildren();
    CreatedCharacterSlots.Empty();

    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
        
        TSoftObjectPtr<UTexture2D> CharacterIconSoftPtr = ParentLobby->CachedPlayerData.OwnedCharacters[i].CharacterInfo.BattleCharacterInitData.CharacterIcon;
        UTexture2D* CharacterIcon = CharacterIconSoftPtr.LoadSynchronous();

        GetCurrentGrade = ParentLobby->CachedPlayerData.OwnedCharacters[i].CharacterInfo.CurrentGrade; // 0 이면 보유X , 0 초과는 보유 및 강화
        
        bool hasOwned = false;
        FGameplayTag CharacterTag = ParentLobby->CachedPlayerData.OwnedCharacters[i].CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName CharacterName = GetLeafNameFromTag(CharacterTag);
        
        if (GetCurrentGrade > 0)
            hasOwned = true;
  
        NewSlot->SetIconImage(CharacterIcon, hasOwned, CharacterName);
        NewSlot->OnCharacterSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnCharacterSlotSelected);

        if (!NewSlot)
            continue;

        UWrapBoxSlot* WrapSlot = CharacterGridPanel->AddChildToWrapBox(NewSlot);

        if (WrapSlot)
        {
            WrapSlot->SetHorizontalAlignment(HAlign_Fill);
            WrapSlot->SetVerticalAlignment(VAlign_Fill);
        }

        if (ParentLobby->CachedPlayerData.OwnedCharacters[i].bSelection) 
        {
            SelectedIndex = i;
        }

        CreatedCharacterSlots.Add(NewSlot);
    }

    if (SelectedIndex != INDEX_NONE && CreatedCharacterSlots.IsValidIndex(SelectedIndex))
    {
        auto& Data = ParentLobby->CachedPlayerData.OwnedCharacters[SelectedIndex];

        FGameplayTag CharacterTag = Data.CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName CharacterName = GetLeafNameFromTag(CharacterTag);

        bool hasOwned = Data.CharacterInfo.CurrentGrade > 0;

        OnCharacterSlotSelected(CreatedCharacterSlots[SelectedIndex], CharacterName, hasOwned);
    }
}

// ========================================================
// 캐릭터 슬롯 클릭
// ========================================================
void UCharacterHUDWidget::OnCharacterSlotSelected(URoundedSlotWidget* ClickedSlot, FName CharacterName, bool SlotCharacterOwned)
{
    FCurrencyData* soulData = ParentLobby->CachedPlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Soul::Value);
    const int64 soulAmount = (soulData) ? soulData->CurrAmount : 0;

    CombinedInfoString = "";

    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

        FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName ListCharacterName = GetLeafNameFromTag(CharacterTag);

        bool bIsSelected = (ListCharacterName == CharacterName);
        
        if (bIsSelected)
        {
            TargetGradeIndex =  (TargetData.CharacterInfo.CurrentGrade > 0)? TargetData.CharacterInfo.CurrentGrade - 1 : 0;
            CharacterStar = TargetGradeIndex;
            CharacterGrade = GetGradePriority(TargetData.CharacterInfo.CurrGrade);

            // 최대 강화를 넘지 않기 위해
            if (TargetGradeIndex < 3)
            {
                RequiredSoul = TargetData.CharacterInfo.BattleCharacterInitData.RequiredShardCount[TargetGradeIndex];
                ButtonText = FText::Format(FText::FromString(TEXT("강화 : {0} 소울")), FText::AsNumber(RequiredSoul));
            }
            else
            {
                ButtonText = FText::FromString(TEXT("최대 강화"));
            }

            if (FDTBattleStatsContainerRow* BattleRow = DataSubsystem->GetRow<FDTBattleStatsContainerRow>(Arcanum::DataTable::BattleStats, ListCharacterName)) {
                if (BattleRow->GradeDataSteps.IsValidIndex(TargetGradeIndex)) {
                    const FGradeStatData& CurrentStats = BattleRow->GradeDataSteps[TargetGradeIndex];
                    for (const FRegenStat& RStat : CurrentStats.RegenStats)
                    {
                        FString TagString = RStat.ParentTag.IsValid() ? GetLeafNameFromTag(RStat.ParentTag).ToString() : TEXT("NoTag");
                        FString RowString = FString::Printf(TEXT("%.1f ( %.2f )"),RStat.BaseMax, RStat.BaseTick);

                        if (CombinedInfoString.IsEmpty())
                        {
                            CombinedInfoString = RowString;
                        }
                        else
                        {
                            CombinedInfoString += LINE_TERMINATOR + RowString; // LINE_TERMINATOR \n 역할
                        }
                    }
                    for (const FNonRegenStat& NRStat : CurrentStats.NonRegenStats)
                    {
                        FString TagString = NRStat.TagName.IsValid() ? GetLeafNameFromTag(NRStat.TagName).ToString() : TEXT("NoTag");
                        float TotalValue = NRStat.BaseValue + NRStat.BonusValue + NRStat.ModifierValue;

                        FString RowString = FString::Printf(TEXT("%.2f"), TotalValue);
                        if (CombinedInfoString.IsEmpty())
                        {
                            CombinedInfoString = RowString;
                        }
                        else
                        {
                            CombinedInfoString += LINE_TERMINATOR + RowString; 
                        }
                    }
                }
            }
            FinalText = FText::FromString(CombinedInfoString);
            // 캐릭터 info창 바꾸기
            UpdateCharacterInfo(CharacterName, SlotCharacterOwned, FinalText, ButtonText, soulAmount);
        }

        if (CreatedCharacterSlots.IsValidIndex(i))
        {
            FLinearColor SlotColor;

            // 장착중인 캐릭터 - 노랑
            if (TargetData.bSelection)
            {
                SlotColor = FLinearColor::Yellow;
            }
            // 장착X, 클릭된 캐릭터 - 핑크
            else if (bIsSelected)
            {
                SlotColor = FLinearColor(1.0f, 0.4f, 0.7f, 1.0f);
            }
            // 일반 슬롯 - 흰색
            else
            {
                SlotColor = FLinearColor::White;
            }
            CreatedCharacterSlots[i]->SetRoundBackgroundColor(SlotColor);
        }
    }
}

// ========================================================
// 캐릭터창 - 강화하기 버튼
// ========================================================

void UCharacterHUDWidget::CharacterEnhancement(FText InCharacterName, int32 InRequiredSoul)
{
    // 이미 보유하고 있는 캐릭터, 소울이 충분한 경우에만 클릭이 되도록 해놓음
    // 소울 소비
     FCurrencyData* soulData = ParentLobby->CachedPlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Soul::Value);
     const int64 soulAmount = (soulData) ? soulData->CurrAmount : 0;
     if(soulAmount>= InRequiredSoul)
     soulData->CurrAmount -= InRequiredSoul;

     FName SelectedCharacterName;
     CombinedInfoString = "";

    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

        FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName PlayerName = GetLeafNameFromTag(CharacterTag);

        bool bIsSelected = InCharacterName.ToString().Equals(PlayerName.ToString());

        if (bIsSelected)
        {
            //CurrentGrade + 1 저장
            TargetData.CharacterInfo.CurrentGrade +=1;
            SelectedCharacterName = PlayerName;
            TargetGradeIndex = (TargetData.CharacterInfo.CurrentGrade > 0) ? TargetData.CharacterInfo.CurrentGrade - 1 : 0;
            CharacterStar = TargetGradeIndex;
            CharacterGrade = GetGradePriority(TargetData.CharacterInfo.CurrGrade);

            // 최대 강화를 넘지 않기 위해
            if (TargetGradeIndex < 3)
            {
                RequiredSoul = TargetData.CharacterInfo.BattleCharacterInitData.RequiredShardCount[TargetGradeIndex];
                ButtonText = FText::Format(FText::FromString(TEXT("강화 : {0} 소울")), FText::AsNumber(RequiredSoul));
            }
            else
            {
                ButtonText = FText::FromString(TEXT("최대 강화"));
            }

            if (FDTBattleStatsContainerRow* BattleRow = DataSubsystem->GetRow<FDTBattleStatsContainerRow>(Arcanum::DataTable::BattleStats, PlayerName)) {
                if (BattleRow->GradeDataSteps.IsValidIndex(TargetGradeIndex)) {
                    const FGradeStatData& CurrentStats = BattleRow->GradeDataSteps[TargetGradeIndex];
                    for (const FRegenStat& RStat : CurrentStats.RegenStats)
                    {
                        FString TagString = RStat.ParentTag.IsValid() ? GetLeafNameFromTag(RStat.ParentTag).ToString() : TEXT("NoTag");
                        FString RowString = FString::Printf(TEXT("%.1f ( %.2f )"), RStat.BaseMax, RStat.BaseTick);

                        if (CombinedInfoString.IsEmpty())
                        {
                            CombinedInfoString = RowString;
                        }
                        else
                        {
                            CombinedInfoString += LINE_TERMINATOR + RowString; // LINE_TERMINATOR \n 역할
                        }
                    }
                    for (const FNonRegenStat& NRStat : CurrentStats.NonRegenStats)
                    {
                        FString TagString = NRStat.TagName.IsValid() ? GetLeafNameFromTag(NRStat.TagName).ToString() : TEXT("NoTag");
                        float TotalValue = NRStat.BaseValue + NRStat.BonusValue + NRStat.ModifierValue;

                        FString RowString = FString::Printf(TEXT("%.2f"), TotalValue);
                        if (CombinedInfoString.IsEmpty())
                        {
                            CombinedInfoString = RowString;
                        }
                        else
                        {
                            CombinedInfoString += LINE_TERMINATOR + RowString;
                        }
                    }
                }
            }
            FinalText = FText::FromString(CombinedInfoString);
            // Info창 다시 불러오기
            UpdateCharacterInfo(SelectedCharacterName, true, FinalText, ButtonText, soulAmount);
        }
    }
  }

// ========================================================
// 캐릭터창 - 장착 버튼
// ========================================================
void UCharacterHUDWidget::SetPlayerCharacter(FText CharacterName)
{
    // 클릭한 캐릭터 bSelection true로 변경
    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

        FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName SetPlayerName = GetLeafNameFromTag(CharacterTag);

        if (CharacterName.ToString().Equals(SetPlayerName.ToString()))
        {
            TargetData.bSelection = true;
        }
        else {
            TargetData.bSelection = false;
        }
    }
    InitCharacterHUD();
}

// ========================================================
// 캐릭터 정보 출력창
// ========================================================
void UCharacterHUDWidget::UpdateCharacterInfo(FName CharacterName, bool SlotCharacterOwned,FText InFinalText, FText InButtonText, int64 soulAmount)
{
    if (CharacterSwitcher)
    {
        CharacterSwitcher->SetActiveWidgetIndex(0);
        UCharacterInfo* InfoWidget = Cast<UCharacterInfo>(CharacterSwitcher->GetWidgetAtIndex(0));

        if (InfoWidget)
        {
            InfoWidget->SetCharacterName(CharacterName);
            InfoWidget->SetStarCharcterInfo(CharacterStar);
            InfoWidget->SetEnhanceButtonEnabled(SlotCharacterOwned, RequiredSoul, soulAmount, TargetGradeIndex);
            InfoWidget->SetPlayerButtonEnabled(SlotCharacterOwned);
            InfoWidget->SetGradeCharcterInfo(CharacterGrade);
            InfoWidget->SetCharcterInfo(InFinalText);
            InfoWidget->SetEnhanceBtnText(InButtonText);
        }
    }
}

// ========================================================
// 무기, 장비 슬롯 클릭
// ========================================================

void UCharacterHUDWidget::OnSquareSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex)
{
    if (CharacterSwitcher)
    {
        UWidget* ActiveWidget = nullptr;
        for (auto& EquipSlot : EquipmentSlots)
        {
            EquipSlot->SetSquareBackgroundColor(
                EquipSlot == ClickedSlot ? FLinearColor(1.0f, 0.4f, 0.7f, 1.0f) : FLinearColor::White
            );
        }

        switch (SlotIndex)
        {
        case 0:  // 무기 슬롯
            UE_LOG(LogTemp, Warning, TEXT("무기 슬롯 Clicked"));
            ActiveWidget = CharacterSwitcher->GetWidgetAtIndex(1);
            break;
        case 1:  // 투구 슬롯
            UE_LOG(LogTemp, Warning, TEXT("투구 슬롯 Clicked"));
            ActiveWidget = CharacterSwitcher->GetWidgetAtIndex(2);
            break;
        case 2:  // 갑옷 슬롯
            UE_LOG(LogTemp, Warning, TEXT("갑옷 슬롯 Clicked"));
            break;
        case 3:  // 장갑 슬롯
            UE_LOG(LogTemp, Warning, TEXT("장갑 슬롯 Clicked"));
            break;
        case 4:  // 신발 슬롯
            UE_LOG(LogTemp, Warning, TEXT("신발 슬롯 Clicked"));
            break;
        default:
            ActiveWidget = CharacterSwitcher->GetWidgetAtIndex(0);
            break;
        }

        if (ActiveWidget)
        {
            CharacterSwitcher->SetActiveWidget(ActiveWidget);
        }
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