#include "UI/Lobby/Contents/Character/CharacterHUDWidget.h"
#include "UI/Lobby/Contents/Character/RoundedSlotWidget.h"
#include "UI/Lobby/Contents/Character/CharacterInfo.h"
#include "UI/Lobby/Contents/Character/SquareSlotWidget.h"
#include "UI/Lobby/Contents/Character/CharacterEquipWidget.h"
#include "UI/Lobby/Contents/Character/SetEffectPanelWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemDetailHelper.h"
#include "UI/Lobby/Contents/ItemDetail/ItemStatPanelWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "GameplayTags/ArcanumTags.h"
#include "Data/Types/BaseUnitData.h"


#include "Core/ARPlayerAccountService.h"
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

	EquipmentSlots.Reset();
	EquipmentSlots.Add(Weapon1Slot);
	EquipmentSlots.Add(Weapon2Slot);
	EquipmentSlots.Add(LegendaryWeaponSlot);
	EquipmentSlots.Add(HelmetSlot);
	EquipmentSlots.Add(ChestSlot);
	EquipmentSlots.Add(GloveSlot);
	EquipmentSlots.Add(BootSlot);

	for (auto& EquipSlot : EquipmentSlots)
	{
		EquipSlot->OnSlotClicked.RemoveDynamic(this, &UCharacterHUDWidget::OnSquareSlotClicked);
		EquipSlot->OnSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnSquareSlotClicked);
	}

	if (CharacterInfo)
	{
		CharacterInfo->OnEnhanceBtnClicked.AddDynamic(this, &UCharacterHUDWidget::CharacterEnhancement);
		CharacterInfo->OnSetPlayerBtnClicked.AddDynamic(this, &UCharacterHUDWidget::SetPlayerCharacter);
	}

	if (!CharacterGridPanel || !RoundedSlotWidgetClass)
		return;

	/// Test : 유닛창 테스트용
	for (int32 Index = 0; Index < 12; ++Index)
	{
		URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
		if (!NewSlot) continue;

		UWrapBoxSlot* WrapSlot = UnitGridPanel->AddChildToWrapBox(NewSlot);
		if (WrapSlot)
		{
			WrapSlot->SetHorizontalAlignment(HAlign_Fill);
			WrapSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

	if (CharacterEquipWidget)
	{
		CharacterEquipWidget->OnCharacterEquipRequested.RemoveDynamic(this, &UCharacterHUDWidget::HandleCharacterEquipRequested);
		CharacterEquipWidget->OnCharacterEquipRequested.AddDynamic(this, &UCharacterHUDWidget::HandleCharacterEquipRequested);

		CharacterEquipWidget->OnCharacterUnequipRequested.RemoveDynamic(this, &UCharacterHUDWidget::HandleCharacterUnequipRequested);
		CharacterEquipWidget->OnCharacterUnequipRequested.AddDynamic(this, &UCharacterHUDWidget::HandleCharacterUnequipRequested);
	}

	if (EquipOpenBtn)
	{
		EquipOpenBtn->OnClicked.RemoveDynamic(this, &UCharacterHUDWidget::HandleEquipOpenBtnClicked);
		EquipOpenBtn->OnClicked.AddDynamic(this, &UCharacterHUDWidget::HandleEquipOpenBtnClicked);
	}
}

FReply UCharacterHUDWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

void UCharacterHUDWidget::SetParentLobby(ULobbyHUD* InLobby)
{
	ParentLobby = InLobby;

	if (CharacterEquipWidget)
	{
		if (UInventoryHUDWidget* inventoryHUDWidget = CharacterEquipWidget->GetInventoryHUDWidget())
		{
			inventoryHUDWidget->SetParentLobby(ParentLobby);
			inventoryHUDWidget->InitInventorySlots(ParentLobby->GetInventoryCapacity());
		}
	}
}

// ========================================================
// 캐릭터 목록창 초기화
// ========================================================

void UCharacterHUDWidget::InitCharacterHUD()
{
	SelectedIndex = INDEX_NONE;
	CharacterGridPanel->ClearChildren();
	CreatedCharacterSlots.Empty();

	for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
	{
		URoundedSlotWidget* NewSlot = CreateWidget<URoundedSlotWidget>(GetWorld(), RoundedSlotWidgetClass);
		if (!NewSlot) continue;

		auto& TargetCharacter = ParentLobby->CachedPlayerData.OwnedCharacters[i];
		TSoftObjectPtr<UTexture2D> CharacterIconSoftPtr = TargetCharacter.CharacterInfo.BattleCharacterInitData.CharacterIcon;

		UTexture2D* CharacterIcon = CharacterIconSoftPtr.LoadSynchronous();

		CurrentGrade = TargetCharacter.CharacterInfo.CurrStarLevel; // 0 이면 보유X , 0 초과는 보유 및 강화 상태
		bool hasOwned = false;
		FGameplayTag CharacterTag = ParentLobby->CachedPlayerData.OwnedCharacters[i].CharacterInfo.BattleCharacterInitData.CharacterTag;
		FName CharacterName = GetLeafNameFromTag(CharacterTag);

		if (CurrentGrade > 0)
			hasOwned = true;

		NewSlot->SetIconImage(CharacterIcon, hasOwned, CharacterName);
		NewSlot->OnCharacterSlotClicked.AddDynamic(this, &UCharacterHUDWidget::OnCharacterSlotSelected);

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

	for (int32 i = 0; i < ParentLobby->CachedPlayerData.AllyburdenCharacters.Num(); i++) {
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
	}
}

// ========================================================
// 캐릭터 슬롯 클릭
// ========================================================
void UCharacterHUDWidget::OnCharacterSlotSelected(URoundedSlotWidget* ClickedSlot, FName CharacterName, bool SlotCharacterOwned)
{
	FCurrencyData* GoldData = ParentLobby->CachedPlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	const int64 GoldAmount = (GoldData) ? GoldData->CurrAmount : 0;

	CombinedInfoString = "";
	CurrentSelectedCharacterName = CharacterName;
	bCurrentSelectedCharacterOwned = SlotCharacterOwned;
	UpdateCharacterPortrait(CharacterName);

	for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
	{
		FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

		FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
		FName ListCharacterName = GetLeafNameFromTag(CharacterTag);

		bool bIsSelected = (ListCharacterName == CharacterName);

		if (bIsSelected)
		{
			TargetGradeIndex = (TargetData.CharacterInfo.CurrStarLevel > 0) ? TargetData.CharacterInfo.CurrStarLevel - 1 : 0;
			CharacterStar = TargetGradeIndex;
			CharacterGrade = GetGradePriority(TargetData.CharacterInfo.CurrGrade);

			// 최대 강화를 넘지 않기 위해
			if (TargetGradeIndex < 3)
			{
				RequiredSoul = TargetData.CharacterInfo.BattleCharacterInitData.RequiredGoldCount[TargetGradeIndex];
				ButtonText = FText::Format(FText::FromString(TEXT("강화 : {0} 골드")), FText::AsNumber(RequiredSoul));
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
			// 캐릭터 info창 바꾸기
			UpdateCharacterInfo(CharacterName, TargetData.bSelection, SlotCharacterOwned, FinalText, ButtonText, GoldAmount);
		}

		if (CreatedCharacterSlots.IsValidIndex(i))
		{
			FLinearColor SlotColor;

			// 장착중인 캐릭터 - 노랑
			if (TargetData.bSelection)
			{
				SlotColor = FLinearColor(1.0f, 0.4706f, 0.1216f, 1.0f);
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
	RefreshArmorStatPanel();
	RefreshSetEffectPanel();

	/// 260311 변경 : 추가 (클릭 시 데이터 변경되게 info 관련은 변경해주세요.)
	FGameplayTag TargetTag = FGameplayTag::RequestGameplayTag(FName("Arcanum.Unit.Ally"));
	if (ClickedSlot->Tag.MatchesTag(TargetTag)) {

		if (CharacterSwitcher)
		{
			CharacterSwitcher->SetActiveWidgetIndex(2);
			UCharacterInfo* InfoWidget = Cast<UCharacterInfo>(CharacterSwitcher->GetWidgetAtIndex(2));

			for (int32 i = 0; i < ParentLobby->CachedPlayerData.AllyburdenCharacters.Num(); i++) {

				const FUnitInfoSetting& UnitData = ParentLobby->CachedPlayerData.AllyburdenCharacters[i];

				FGameplayTag CharacterTag = UnitData.Tag;
				FName ListUnitName = GetLeafNameFromTag(CharacterTag);

				bool bIsSelected = (ListUnitName == CharacterName);
				if (bIsSelected)
				{
					FText DescText = UnitData.Description;
					FText MeatCostText = FText::AsNumber(UnitData.MeatCost);
					FText CoolTimeText = FText::AsNumber(UnitData.CoolTime);

					if (InfoWidget)
					{
						InfoWidget->SetCharacterName(CharacterName);
						//InfoWidget->SetStarCharcterInfo(CharacterStar);
						//InfoWidget->SetEnhanceButtonEnabled(SlotCharacterOwned, RequiredSoul, soulAmount, TargetGradeIndex);
						//InfoWidget->SetPlayerButtonEnabled(false, SlotCharacterOwned);
						//InfoWidget->SetGradeCharcterInfo(CharacterGrade);
						InfoWidget->SetDescriptionText(DescText);
						InfoWidget->SetMeatCostText(MeatCostText);
						InfoWidget->SetCoolTimeText(CoolTimeText);
						//InfoWidget->SetEnhanceBtnText(InButtonText);
					}
				}
			}

		}
	}
}


void UCharacterHUDWidget::UpdateSlotVisuals(const TMap<FGameplayTag, FGuid>& InEquipmentMap)
{
	if (!ParentLobby || !DataSubsystem) return;

	for (const TPair<FGameplayTag, FGuid>& pair : InEquipmentMap)
	{
		const FGameplayTag& equipSlotTag = pair.Key;
		const FGuid& itemGuid = pair.Value;

		if (equipSlotTag.IsValid() && itemGuid.IsValid())
		{
			const FEquipmentInfo* foundEquip = nullptr;
			for (const FEquipmentInfo& equip : ParentLobby->CachedPlayerData.Inventory)
			{
				if (equip.ItemGuid == itemGuid)
				{
					foundEquip = &equip;
					break;
				}
			}
			if (!foundEquip) continue;

			if (const FDTEquipmentInfoRow* equipRow = FPlayerAccountService::FindEquipmentInfoRowByTag(this, foundEquip->ItemTag))
			{
				if (UTexture2D* icon = equipRow->Icon.LoadSynchronous())
				{
					USquareSlotWidget* targetSlot = nullptr;

					using namespace Arcanum::Items::ItemSlot;
					if (equipSlotTag.MatchesTagExact(Weapon::Slot1))          targetSlot = Weapon1Slot;
					else if (equipSlotTag.MatchesTagExact(Weapon::Slot2))     targetSlot = Weapon2Slot;
					else if (equipSlotTag.MatchesTagExact(Weapon::Legendary)) targetSlot = LegendaryWeaponSlot;
					else if (equipSlotTag.MatchesTagExact(Armor::Helmet))     targetSlot = HelmetSlot;
					else if (equipSlotTag.MatchesTagExact(Armor::Chest))      targetSlot = ChestSlot;
					else if (equipSlotTag.MatchesTagExact(Armor::Glove))      targetSlot = GloveSlot;
					else if (equipSlotTag.MatchesTagExact(Armor::Boot))       targetSlot = BootSlot;

					if (targetSlot)
					{
						targetSlot->SetItemIconImage(icon);
						targetSlot->SetWeaponGuid(itemGuid);
						targetSlot->SetUpgradeLevel(foundEquip->CurrUpgradeLevel);
					}
				}
			}
		}
	}
}


// ========================================================
// 캐릭터창 - 강화하기 버튼
// ========================================================

void UCharacterHUDWidget::CharacterEnhancement(FText InCharacterName, int32 InRequiredSoul)
{
	// 이미 보유하고 있는 캐릭터, 골드이 충분한 경우에만 클릭이 되도록 해놓음
	// 골드 소비
	FCurrencyData* GOldData = ParentLobby->CachedPlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	const int64 GOldAmount = (GOldData) ? GOldData->CurrAmount : 0;
	/*if (soulAmount >= InRequiredSoul)
		soulData->CurrAmount -= InRequiredSoul;*/
	/// TODO : UpdateCurrency 사용시 캐릭터 CurrStarLevel가 초기화됨
	FPlayerAccountService::UpdateCurrency(this, ParentLobby->CachedPlayerData, Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, InRequiredSoul * -1);

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
			//TargetData.CharacterInfo.CurrStarLevel += 1;
			if (FPlayerAccountService::UpdateCharacter(this, CharacterTag)) {
				ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);

				for (int32 j = 0; j < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); j++)
				{
					FBattleCharacterData& UpdateTargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

					FGameplayTag UpdateCharacterTag = UpdateTargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;

					FName UpdatePlayerName = GetLeafNameFromTag(UpdateCharacterTag);
					bool bIsUpdateSelected = InCharacterName.ToString().Equals(UpdatePlayerName.ToString());

					if (bIsUpdateSelected)
					{
						SelectedCharacterName = UpdatePlayerName;
						TargetGradeIndex = (UpdateTargetData.CharacterInfo.CurrStarLevel > 0) ? UpdateTargetData.CharacterInfo.CurrStarLevel - 1 : 0;
						CharacterStar = TargetGradeIndex;
						CharacterGrade = GetGradePriority(UpdateTargetData.CharacterInfo.CurrGrade);

						// 최대 강화를 넘지 않기 위해
						if (TargetGradeIndex < 3)
						{
							RequiredSoul = UpdateTargetData.CharacterInfo.BattleCharacterInitData.RequiredGoldCount[TargetGradeIndex];
							ButtonText = FText::Format(FText::FromString(TEXT("강화 : {0} 골드")), FText::AsNumber(RequiredSoul));
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
						//const int64 updatedSoulAmount = soulData ? soulData->CurrAmount : 0;
						UpdateCharacterInfo(SelectedCharacterName, UpdateTargetData.bSelection, true, FinalText, ButtonText, GOldAmount);
						//ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
						break;
					}
				}
			}
		}
	}
}

// ========================================================
// 캐릭터창 - 장착 버튼
// ========================================================
void UCharacterHUDWidget::SetPlayerCharacter(FText CharacterName)
{
	if (FPlayerAccountService::SetSelectedCharacter(this, FName(*CharacterName.ToString())))
	{
		ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
		InitCharacterHUD();
	}

	//// 클릭한 캐릭터 bSelection true로 변경
	//for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
	//{
	//	FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

	//	FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
	//	FName SetPlayerName = GetLeafNameFromTag(CharacterTag);

	//	if (CharacterName.ToString().Equals(SetPlayerName.ToString()))
	//	{
	//		TargetData.bSelection = true;
	//	}
	//	else {
	//		TargetData.bSelection = false;
	//	}
	//}
	//InitCharacterHUD();
}

// ========================================================
// 캐릭터 정보 출력창
// ========================================================
void UCharacterHUDWidget::UpdateCharacterInfo(FName CharacterName, bool bSetCharacter, bool SlotCharacterOwned, FText InFinalText, FText InButtonText, int64 soulAmount)
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

void UCharacterHUDWidget::UpdateCharacterPortrait(FName CharacterName)
{
	if (!ParentLobby || !CharacterPortraitImage) return;
	
	//CharacterPortraitImage->SetBrushFromTexture(nullptr);

	for (const FBattleCharacterData characterData : ParentLobby->CachedPlayerData.OwnedCharacters)
	{
		const FName listCharacterName = GetLeafNameFromTag(characterData.CharacterInfo.BattleCharacterInitData.CharacterTag);
		if (listCharacterName == CharacterName)
		{
			UTexture2D* portraitTexture = characterData.CharacterInfo.BattleCharacterInitData.CharacterColor.LoadSynchronous();
			CharacterPortraitImage->SetBrushFromTexture(portraitTexture);
			break;
		}
	}
}

// ========================================================
// 무기, 장비 슬롯 클릭
// ========================================================

void UCharacterHUDWidget::OnSquareSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex)
{
	for (USquareSlotWidget* EquipSlot : EquipmentSlots)
	{
		if (EquipSlot)
		{
			EquipSlot->SetSquareBackgroundColor(
				EquipSlot == ClickedSlot ? FLinearColor(1.0f, 0.4f, 0.7f, 1.0f) : FLinearColor::White
			);
		}
	}

	if (CharacterSwitcher)
	{
		if (UWidget* ActiveWidget = CharacterSwitcher->GetWidgetAtIndex(1))
		{
			CharacterSwitcher->SetActiveWidget(ActiveWidget);
		}
	}

	FGameplayTag EquipSlotTag;

	switch (SlotIndex)
	{
	case 0:
		EquipSlotTag = Arcanum::Items::ItemSlot::Weapon::Slot1;
		break;

	case 1:
		EquipSlotTag = Arcanum::Items::ItemSlot::Weapon::Slot2;
		break;

	case 2:
		EquipSlotTag = Arcanum::Items::ItemSlot::Weapon::Legendary;
		break;

	case 3:
		EquipSlotTag = Arcanum::Items::ItemSlot::Armor::Helmet;
		break;

	case 4:
		EquipSlotTag = Arcanum::Items::ItemSlot::Armor::Chest;
		break;

	case 5:
		EquipSlotTag = Arcanum::Items::ItemSlot::Armor::Glove;
		break;

	case 6:
		EquipSlotTag = Arcanum::Items::ItemSlot::Armor::Boot;
		break;

	default:
		break;
	}
	if (!EquipSlotTag.IsValid()) return;

	if (CharacterEquipWidget)
	{
		CharacterEquipWidget->SetEquipSlotTag(EquipSlotTag);

		if (UInventoryHUDWidget* inventoryHUDWidget = CharacterEquipWidget->GetInventoryHUDWidget())
		{
			inventoryHUDWidget->ClearSelection();
		}
	}
}

// ========================================================
// 무기, 장비 슬롯 초기화
// ========================================================
void UCharacterHUDWidget::InitEquipment(FName CharacterName)
{
	for (USquareSlotWidget* WeaponsSlot : EquipmentSlots)
	{
		if (WeaponsSlot)
		{
			WeaponsSlot->SetItemIconImage(nullptr);
			WeaponsSlot->SetWeaponGuid(FGuid());
			WeaponsSlot->ClearUpgradeLevel();
		}
	}

	// CharacterName이 None이면 기본 캐릭터인 Elara 이름 넣기
	if (CharacterName.IsNone())
	{
		CharacterName = FName(TEXT("Elara"));
	}

	for (int32 i = 0; i < ParentLobby->CachedPlayerData.OwnedCharacters.Num(); i++)
	{
		FBattleCharacterData& TargetData = ParentLobby->CachedPlayerData.OwnedCharacters[i];

		FGameplayTag CharacterTag = TargetData.CharacterInfo.BattleCharacterInitData.CharacterTag;
		FName ListCharacterName = GetLeafNameFromTag(CharacterTag);

		bool bIsSelected = (ListCharacterName == CharacterName);

		if (bIsSelected)
		{
			UpdateSlotVisuals(TargetData.WeaponSlots);
			UpdateSlotVisuals(TargetData.LegendaryWeaponSlots);
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

	const FGuid WeaponGuid = ClickedSlot->GetWeaponGuid();
	if (!WeaponGuid.IsValid()) return;

	FGameplayTag EquipSlotTag;
	USquareSlotWidget* TargetUI = nullptr;

	switch (SlotIndex)
	{
	case 0:
		EquipSlotTag = Arcanum::Items::ItemSlot::Weapon::Slot1;
		TargetUI = Weapon1Slot;
		break;

	case 1:
		EquipSlotTag = Arcanum::Items::ItemSlot::Weapon::Slot2;
		TargetUI = Weapon2Slot;
		break;

	case 2:
		EquipSlotTag = Arcanum::Items::ItemSlot::Weapon::Legendary;
		TargetUI = LegendaryWeaponSlot;
		break;

	case 3:
		EquipSlotTag = Arcanum::Items::ItemSlot::Armor::Helmet;
		TargetUI = HelmetSlot;
		break;

	case 4:
		EquipSlotTag = Arcanum::Items::ItemSlot::Armor::Chest;
		TargetUI = ChestSlot;
		break;

	case 5:
		EquipSlotTag = Arcanum::Items::ItemSlot::Armor::Glove;
		TargetUI = GloveSlot;
		break;

	case 6:
		EquipSlotTag = Arcanum::Items::ItemSlot::Armor::Boot;
		TargetUI = BootSlot;
		break;

	default:
		break;
	}
	if (!EquipSlotTag.IsValid() || !TargetUI) return;

	//  선택 캐릭터 장비 장착
	if (FPlayerAccountService::EquipItemToCharacter(this, CurrentSelectedCharacterName, EquipSlotTag, WeaponGuid))
	{
		TargetUI->SetItemIconImage(ClickedSlot->IconImg);
	}
}

void UCharacterHUDWidget::HandleCharacterEquipRequested(const FGameplayTag& InEquipSlotTag, const FGuid& InItemGuid)
{
	if (!ParentLobby) return;

	if (CharacterEquipWidget)
	{
		if (!bCurrentSelectedCharacterOwned) return;
		if (FPlayerAccountService::EquipItemToCharacter(this, CurrentSelectedCharacterName, InEquipSlotTag, InItemGuid))
		{
			ParentLobby->RefreshAllLobbyUI();
			InitEquipment(CurrentSelectedCharacterName);
			RefreshArmorStatPanel();
			RefreshSetEffectPanel();
			CharacterEquipWidget->SetEquipSlotTag(InEquipSlotTag);
		}
	}
}

void UCharacterHUDWidget::HandleCharacterUnequipRequested(const FGameplayTag& InEquipSlotTag)
{
	if (!ParentLobby) return;

	if (CharacterEquipWidget)
	{
		if (FPlayerAccountService::UnequipItemFromCharacter(this, CurrentSelectedCharacterName, InEquipSlotTag))
		{
			ParentLobby->RefreshAllLobbyUI();
			InitEquipment(CurrentSelectedCharacterName);
			RefreshArmorStatPanel();
			RefreshSetEffectPanel();
			CharacterEquipWidget->SetEquipSlotTag(InEquipSlotTag);
		}
	}
}

void UCharacterHUDWidget::HandleEquipOpenBtnClicked()
{
	if (CharacterSwitcher)
	{
		if (UWidget* ActiveWidget = CharacterSwitcher->GetWidgetAtIndex(1))
		{
			CharacterSwitcher->SetActiveWidget(ActiveWidget);
		}
	}
	if (CharacterEquipWidget)
	{
		CharacterEquipWidget->ShowEquipmentInventory();
	}
}

void UCharacterHUDWidget::RefreshArmorStatPanel()
{
	FEquippedTotalStatViewData viewData;
	if (FItemDetailHelper::BuildEquippedArmorStatsViewData(this, CurrentSelectedCharacterName, viewData))
	{
		if (CharacterEquipWidget)
		{
			if (UItemStatPanelWidget* armorStatPanelWidget = CharacterEquipWidget->GetArmorStatPanelWidget())
			{
				armorStatPanelWidget->SetStatLines(viewData.StatLines);
			}
		}
	}
	else
	{
		if (CharacterEquipWidget)
		{
			if (UItemStatPanelWidget* armorStatPanelWidget = CharacterEquipWidget->GetArmorStatPanelWidget())
			{
				armorStatPanelWidget->ClearStatLines();
			}
		}
	}
}

void UCharacterHUDWidget::RefreshSetEffectPanel()
{
	if (!CharacterEquipWidget || !ParentLobby) return;

	USetEffectPanelWidget* setEffectPanelWidget = CharacterEquipWidget->GetSetEffectPanelWidget();
	if (!setEffectPanelWidget) return;

	int32 setCount = 0;
	FSetEffectDefinition setEffectDefinition;

	for (const FBattleCharacterData& characterData : ParentLobby->CachedPlayerData.OwnedCharacters)
	{
		const FName characterName = GetLeafNameFromTag(characterData.CharacterInfo.BattleCharacterInitData.CharacterTag);
		if (characterName != CurrentSelectedCharacterName) continue;

		for (const TPair<FGameplayTag, FGuid>& pair : characterData.ArmorSlots)
		{
			const FGuid& itemGuid = pair.Value;
			if (!itemGuid.IsValid()) continue;

			const FEquipmentInfo* foundEquip = nullptr;
			for (const FEquipmentInfo& equip : ParentLobby->CachedPlayerData.Inventory)
			{
				if (equip.ItemGuid == itemGuid)
				{
					foundEquip = &equip;
					break;
				}
			}

			if (foundEquip)
			{
				const FDTEquipmentInfoRow* equipRow = FPlayerAccountService::FindEquipmentInfoRowByTag(this, foundEquip->ItemTag);
				if (equipRow && !equipRow->SetEffect.SetNameText.IsEmpty())
				{
					if (setEffectDefinition.SetNameText.IsEmpty())
					{
						setEffectDefinition = equipRow->SetEffect;
					}

					if (equipRow->SetEffect.SetNameText.EqualTo(setEffectDefinition.SetNameText))
					{
						setCount++;
					}
				}
			}
		}

		break;
	}

	if (setCount <= 0 || setEffectDefinition.SetNameText.IsEmpty())
	{
		setEffectPanelWidget->ClearEffectText();
		return;
	}

	FSetEffectViewData viewData;
	viewData.bVisible = true;
	viewData.bActivated = setCount >= setEffectDefinition.NeedCount;
	viewData.SetNameText = setEffectDefinition.SetNameText;
	viewData.SetCountText = FText::Format(
		FText::FromString(TEXT("({0}/{1})")),
		FText::AsNumber(setCount),
		FText::AsNumber(setEffectDefinition.NeedCount));

	if (viewData.bActivated)
	{
		viewData.SetDescText = setEffectDefinition.ActiveDescText;
	}
	else
	{
		viewData.SetDescText = setEffectDefinition.InactiveDescText;
	}

	setEffectPanelWidget->SetEffectText(viewData);
}