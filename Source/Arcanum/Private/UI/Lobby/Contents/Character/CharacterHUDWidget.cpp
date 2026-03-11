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

    EquipmentSlots.Add(RightHandSlot);
    EquipmentSlots.Add(LeftHandSlot);
    EquipmentSlots.Add(TwoHandSlot);
    EquipmentSlots.Add(HelmetSlot);
    EquipmentSlots.Add(ChestSlot);
    EquipmentSlots.Add(GloveSlot);
    EquipmentSlots.Add(BootSlot);

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

FReply UCharacterHUDWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return FReply::Handled();
}

// ========================================================
// 캐릭터 목록창 초기화
// ========================================================

void UCharacterHUDWidget::InitCharacterHUD()
{
   
    CharacterGridPanel->ClearChildren();
    CreatedCharacterSlots.Empty();

    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
        
        auto& TargetCharacter = ParentLobby->CachedPlayerData.OwnedCharacters[i];
        TSoftObjectPtr<UTexture2D> CharacterIconSoftPtr = TargetCharacter.CharacterInfo.BattleCharacterInitData.CharacterIcon;

        UTexture2D* CharacterIcon = CharacterIconSoftPtr.LoadSynchronous();

        GetCurrentGrade = TargetCharacter.CharacterInfo.CurrStarLevel; // 0 이면 보유X , 0 초과는 보유 및 강화 상태
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
        auto Data = ParentLobby->CachedPlayerData.OwnedCharacters[SelectedIndex];

        FGameplayTag CharacterTag = ParentLobby->CachedPlayerData.OwnedCharacters[SelectedIndex].CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName CharacterName = GetLeafNameFromTag(CharacterTag);

        bool hasOwned = ParentLobby->CachedPlayerData.OwnedCharacters[SelectedIndex].CharacterInfo.CurrStarLevel > 0;
  
        OnCharacterSlotSelected(CreatedCharacterSlots[SelectedIndex], CharacterName, hasOwned);
    }
    InitServantCharacter();
}

void UCharacterHUDWidget::InitServantCharacter()
{
    UnitGridPanel->ClearChildren();
    CreatedServantCharacterSlots.Empty();

    /*for (int32 i = 0; i < ParentLobby->CachedPlayerData.AllyburdenCharacters.Num(); i++) {
        URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
        if (!NewSlot) continue;

        const FUnitInfoSetting& UnitData = ParentLobby->CachedPlayerData.AllyburdenCharacters[i];
        TSoftObjectPtr<UTexture2D> CharacterIconSoftPtr = UnitData.Icon;
        UTexture2D* CharacterIcon = CharacterIconSoftPtr.LoadSynchronous();

        FGameplayTag CharacterTag = UnitData.Tag;
        FName CharacterName = GetLeafNameFromTag(CharacterTag);

        bool hasOwned = true;

        NewSlot->SetIconImage(CharacterIcon, hasOwned, CharacterName, CharacterTag);
        NewSlot->OnCharacterSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnCharacterSlotSelected);

        UWrapBoxSlot* WrapSlot = UnitGridPanel->AddChildToWrapBox(NewSlot);
        if (WrapSlot) {
            WrapSlot->SetHorizontalAlignment(HAlign_Fill);
            WrapSlot->SetVerticalAlignment(VAlign_Fill);
        }
        CreatedServantCharacterSlots.Add(NewSlot);
    }*/
}

// ========================================================
// 캐릭터 슬롯 클릭
// ========================================================
void UCharacterHUDWidget::OnCharacterSlotSelected(URoundedSlotWidget* ClickedSlot, FName CharacterName, bool SlotCharacterOwned)
{
    FCurrencyData* soulData = ParentLobby->CachedPlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Soul::Value);
    const int64 soulAmount = (soulData) ? soulData->CurrAmount : 0;

    CombinedInfoString = "";
    CurrentSelectedCharacterName = CharacterName;

    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

        FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName ListCharacterName = GetLeafNameFromTag(CharacterTag);

        bool bIsSelected = (ListCharacterName == CharacterName);
        
        if (bIsSelected)
        {
            TargetGradeIndex =  (TargetData.CharacterInfo.CurrStarLevel > 0)? TargetData.CharacterInfo.CurrStarLevel - 1 : 0;
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
            UpdateCharacterInfo(CharacterName, TargetData.bSelection, SlotCharacterOwned, FinalText, ButtonText, soulAmount);
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
    InitEquipment(CharacterName);

    /// 260311 변경 : 추가 (클릭 시 데이터 변경되게 info 관련은 변경해주세요.)
    FGameplayTag TargetTag = FGameplayTag::RequestGameplayTag(FName("Arcanum.Unit.Ally"));
    if (ClickedSlot->Tag.MatchesTag(TargetTag)) {

        if (CharacterSwitcher)
        {
            CharacterSwitcher->SetActiveWidgetIndex(0);
            UCharacterInfo* InfoWidget = Cast<UCharacterInfo>(CharacterSwitcher->GetWidgetAtIndex(0));

            if (InfoWidget)
            {
                InfoWidget->SetCharacterName(CharacterName);
                InfoWidget->SetStarCharcterInfo(CharacterStar);
                InfoWidget->SetEnhanceButtonEnabled(SlotCharacterOwned, RequiredSoul, soulAmount, TargetGradeIndex);
                InfoWidget->SetPlayerButtonEnabled(SlotCharacterOwned, SlotCharacterOwned);
                InfoWidget->SetGradeCharcterInfo(CharacterGrade);
                //InfoWidget->SetCharcterInfo(InFinalText);
            }
        }

    }
}

void UCharacterHUDWidget::UpdateSlotVisuals(const TMap<FGameplayTag, FGuid>& InEquipmentMap)
{
    UDataTable* const* TablePtr = DataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment);
    if (!TablePtr || !(*TablePtr)) return;

    UDataTable* Table = *TablePtr;

    // 데이터 테이블의 모든 행을 미리 가져옴 (루프 밖에서 한 번만 실행)
    TArray<FDTEquipmentInfoRow*> Rows;
    Table->GetAllRows(TEXT(""), Rows);

    for (const auto& Pair : InEquipmentMap)
    {
        FGameplayTag ItemTag = Pair.Key;

        for (FDTEquipmentInfoRow* Row : Rows)
        {
            if (Row && Row->ItemTag == ItemTag)
            {
                // 1. 아이콘 로드
                UTexture2D* Icon = Row->Icon.LoadSynchronous();

                // 2. 태그로부터 슬롯 이름 생성 (Glove -> GloveSlot)
                FString Left, Right;
                FString TagString = ItemTag.ToString();
                if (!TagString.Split(TEXT("."), &Left, &Right, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
                {
                    Right = TagString;
                }
                Right.Append(TEXT("Slot"));

                // 3. 이름으로 위젯을 찾아 이미지 설정
                if (USquareSlotWidget* TargetSlot = Cast<USquareSlotWidget>(GetWidgetFromName(FName(*Right))))
                {
                    TargetSlot->SetItemIconImage(Icon);
                }
                break; // 행을 찾았으므로 다음 아이템으로 넘어감
            }
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
            //CurrStarLevel + 1 저장
            TargetData.CharacterInfo.CurrStarLevel +=1;
            SelectedCharacterName = PlayerName;
            TargetGradeIndex = (TargetData.CharacterInfo.CurrStarLevel > 0) ? TargetData.CharacterInfo.CurrStarLevel - 1 : 0;
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
            UpdateCharacterInfo(SelectedCharacterName, TargetData.bSelection, true, FinalText, ButtonText, soulAmount);
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
void UCharacterHUDWidget::UpdateCharacterInfo(FName CharacterName, bool bSetCharacter, bool SlotCharacterOwned,FText InFinalText, FText InButtonText, int64 soulAmount)
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
            InfoWidget->SetPlayerButtonEnabled(bSetCharacter, SlotCharacterOwned);
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
        
        int32 TargetIndex = 0;

        ActiveWidget = CharacterSwitcher->GetWidgetAtIndex(1);

        if (ActiveWidget)
        {
            CharacterSwitcher->SetActiveWidget(ActiveWidget);
        }
        for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
        {
            FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

            FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
            FName SetPlayerName = GetLeafNameFromTag(CharacterTag);
            UInventorySlot* InventorySlotWidget = Cast<UInventorySlot>(ActiveWidget);

            if (SetPlayerName == CurrentSelectedCharacterName)
            {
                if (TargetData.CharacterInfo.CurrStarLevel > 0)
                {
                    if (InventorySlotWidget)
                    {
                        InventorySlotWidget->SetEquipButtonEnabled(true);
                    }
                }
                else {
                    if (InventorySlotWidget)
                    {

                        InventorySlotWidget->SetEquipButtonEnabled(false);
                    }
                }

            }

        }
    }
    
    InitWeaponInventory(SlotIndex);
}

// ========================================================
// 무기, 장비 인벤토리 출력
// ========================================================
void UCharacterHUDWidget::InitWeaponInventory(int32 SlotIndex)
{
    if (!ParentLobby || !WeaponList) return;
    TArray<FEquipmentInfo> WeaponInventory = ParentLobby->CachedPlayerData.Inventory;
   
   WeaponList->CreateWeaponItems(WeaponInventory, SlotIndex);
    
}

// ========================================================
// 무기, 장비 슬롯 초기화
// ========================================================
void UCharacterHUDWidget::InitEquipment(FName CharacterName)
{
    WeaponSlotItemIcon = nullptr;
    
    for (USquareSlotWidget* WeaponsSlot : EquipmentSlots)
    {
        if (WeaponsSlot) WeaponsSlot->SetItemIconImage(nullptr);
    }

    // CharacterName이 None이면 기본 캐릭터인 Elara 이름 넣기
    if (CharacterName.IsNone())
    {
        CharacterName = FName(TEXT("Elara"));
    }

    FGameplayTag WeaponTag;
    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

        FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName ListCharacterName = GetLeafNameFromTag(CharacterTag);

        bool bIsSelected = (ListCharacterName == CharacterName);

        if (bIsSelected)
        {
            UpdateSlotVisuals(TargetData.Weapons);
            UpdateSlotVisuals(TargetData.ArmorSlots);
        }
    }
}

// ========================================================
// 무기, 장비 장착 버튼클릭
// ========================================================

void UCharacterHUDWidget::SetupEquipment(USquareSlotWidget* ClickedSlot, int32 SlotIndex)
{
    if (!ClickedSlot) return;

    FGameplayTag SelectedTag = ClickedSlot->GetWeaponTag();
    FGuid WeaponGuid = ClickedSlot->GetWeaponGuid();

    if (!SelectedTag.IsValid())
        return;

    // 캐릭터에 장착 무기 데이터 추가
    for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
    {
        FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

        FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
        FName SetPlayerName = GetLeafNameFromTag(CharacterTag);

        if (SetPlayerName != CurrentSelectedCharacterName) continue;
        
        USquareSlotWidget* TargetUI = nullptr;
            
        switch (SlotIndex)
        {
        case 0: 
            TargetUI = RightHandSlot;
            break;
        case 1:
            TargetUI = LeftHandSlot;
            break;
        case 2: 
            TargetUI = TwoHandSlot;
            break;
        case 3:
            TargetUI = HelmetSlot;
            break;
        case 4:
            TargetUI = ChestSlot;
            break;
        case 5:
            TargetUI = GloveSlot;
            break;
        case 6:
            TargetUI = BootSlot;
            break;
        default:
            break;
        }

        if (TargetUI)
        {
            if (SlotIndex <= 2)
            {
                TargetData.Weapons.Add(SelectedTag, WeaponGuid);
            }
            else
            {
                TargetData.ArmorSlots.Add(SelectedTag, WeaponGuid);
            }

            TargetUI->SetItemIconImage(ClickedSlot->IconImg);
        }
        
    }

}