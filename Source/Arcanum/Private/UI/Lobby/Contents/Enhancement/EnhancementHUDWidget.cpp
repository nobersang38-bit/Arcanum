#include "UI/Lobby/Contents/Enhancement/EnhancementHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/InventoryHUDWidget.h"
#include "UI/Lobby/Contents/Inventory/SubLayout/InventoryItemSlotWidget.h"
#include "UI/Lobby/LobbyHUD.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/InventoryData/Data/FInventoryViewSlot.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "DataInfo/EnhancementData/DataTable/DTEnhanceRuleRow.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UEnhancementHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EnhanceButton)
	{
		EnhanceButton->OnClicked.RemoveDynamic(this, &UEnhancementHUDWidget::HandleEnhanceButtonClicked);
		EnhanceButton->OnClicked.AddDynamic(this, &UEnhancementHUDWidget::HandleEnhanceButtonClicked);
	}

	if (RerollButton)
	{
		RerollButton->OnClicked.RemoveDynamic(this, &UEnhancementHUDWidget::HandleRerollButtonClicked);
		RerollButton->OnClicked.AddDynamic(this, &UEnhancementHUDWidget::HandleRerollButtonClicked);
	}

	if (DisassembleButton)
	{
		DisassembleButton->OnClicked.RemoveDynamic(this, &UEnhancementHUDWidget::HandleDisassembleButtonClicked);
		DisassembleButton->OnClicked.AddDynamic(this, &UEnhancementHUDWidget::HandleDisassembleButtonClicked);
	}

	if (DetailChanceButton)
	{
		DetailChanceButton->OnClicked.RemoveDynamic(this, &UEnhancementHUDWidget::HandleDetailChanceButtonClicked);
		DetailChanceButton->OnClicked.AddDynamic(this, &UEnhancementHUDWidget::HandleDetailChanceButtonClicked);
	}

	if (DetailChancePanel)
	{
		DetailChancePanel->SetVisibility(ESlateVisibility::Collapsed);
	}

	RefreshDetailChanceText();

	ClearEnhancementInfo();
}

void UEnhancementHUDWidget::RefreshSelectedItemSlot()
{
	if (!SelectedItemGuid.IsValid())
	{
		if (SelectedItemSlotWidget)
		{
			SelectedItemSlotWidget->ClearSlot(0);
			SelectedItemSlotWidget->SetSelected(false);
		}

		ClearEnhancementInfo();
		return;
	}

	FInventoryViewSlot selectedSlot;
	selectedSlot.Type = EInventoryViewSlotType::Equipment;
	selectedSlot.ItemGuid = SelectedItemGuid;

	const FEquipmentInfo* foundEquip = FindSelectedEquipment(SelectedItemGuid);
	if (!foundEquip)
	{
		if (SelectedItemSlotWidget)
		{
			SelectedItemSlotWidget->ClearSlot(0);
			SelectedItemSlotWidget->SetSelected(false);
		}

		ClearEnhancementInfo();
		return;
	}

	selectedSlot.ItemTag = foundEquip->ItemTag;
	selectedSlot.UpgradeLevel = foundEquip->CurrUpgradeLevel;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (gameInstance)
	{
		UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
		if (dataSubsystem)
		{
			const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, foundEquip->ItemTag);
			if (catalogRow)
			{
				selectedSlot.Icon = catalogRow->Icon;
			}
		}
	}

	if (SelectedItemSlotWidget)
	{
		SelectedItemSlotWidget->SetSlotData(selectedSlot, 0);
		SelectedItemSlotWidget->SetSelected(true);
	}

	RefreshEnhancementInfo(selectedSlot);
}

void UEnhancementHUDWidget::SetParentLobby(ULobbyHUD* InLobby)
{
	ParentLobby = InLobby;

	EnhancementInventoryWidget->SetParentLobby(InLobby);
	EnhancementInventoryWidget->SetCurrentFilter(EInventoryCategoryFilter::Equipment);

	EnhancementInventoryWidget->OnInventorySlotSelected.RemoveDynamic(this, &UEnhancementHUDWidget::HandleEnhancementItemSelected);
	EnhancementInventoryWidget->OnInventorySlotSelected.AddDynamic(this, &UEnhancementHUDWidget::HandleEnhancementItemSelected);

	RefreshEquipmentInventory();
}

void UEnhancementHUDWidget::RefreshEquipmentInventory()
{
	if (EnhancementInventoryWidget)
	{
		EnhancementInventoryWidget->RefreshEquipmentInventory();
	}
}

void UEnhancementHUDWidget::HandleEnhancementItemSelected(const FInventoryViewSlot& InSlot)
{
	if (!SelectedItemSlotWidget) return;

	if (InSlot.Type == EInventoryViewSlotType::Equipment)
	{
		SelectedItemGuid = InSlot.ItemGuid;

		SelectedItemSlotWidget->SetSlotData(InSlot, 0);
		SelectedItemSlotWidget->SetSelected(true);
		RefreshEnhancementInfo(InSlot);
	}
	else
	{
		SelectedItemGuid.Invalidate();

		SelectedItemSlotWidget->ClearSlot(0);
		SelectedItemSlotWidget->SetSelected(false);
		ClearEnhancementInfo();
	}
}

const FEquipmentInfo* UEnhancementHUDWidget::FindSelectedEquipment(const FGuid& InItemGuid) const
{
	if (!ParentLobby) return nullptr;
	if (!InItemGuid.IsValid()) return nullptr;

	const FPlayerData& playerData = ParentLobby->GetCachedPlayerData();

	for (const FEquipmentInfo& equip : playerData.Inventory)
	{
		if (equip.ItemGuid == InItemGuid)
		{
			return &equip;
		}
	}

	return nullptr;
}

void UEnhancementHUDWidget::RefreshEnhancementInfo(const FInventoryViewSlot& InSlot)
{
	const FEquipmentInfo* foundEquip = nullptr;
	UARGameInstance* gameInstance = nullptr;
	UGameDataSubsystem* dataSubsystem = nullptr;
	const FDTItemCatalogRow* catalogRow = nullptr;
	const FDTEquipmentInfoRow* equipRow = nullptr;

	if (InSlot.Type == EInventoryViewSlotType::Equipment && InSlot.ItemGuid.IsValid())
	{
		foundEquip = FindSelectedEquipment(InSlot.ItemGuid);
		if (foundEquip)
		{
			gameInstance = Cast<UARGameInstance>(GetGameInstance());
			if (gameInstance)
			{
				dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
				if (dataSubsystem)
				{
					catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, foundEquip->ItemTag);
					if (catalogRow && !catalogRow->DetailRowName.IsNone())
					{
						equipRow = dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
					}
				}
			}
		}
	}

	if (!foundEquip || !gameInstance || !dataSubsystem || !catalogRow || !equipRow)
	{
		ClearEnhancementInfo();
		return;
	}

	ApplyEnhancementInfo(catalogRow, equipRow, foundEquip);
}

void UEnhancementHUDWidget::ApplyEnhancementInfo(const FDTItemCatalogRow* InCatalogRow, const FDTEquipmentInfoRow* InEquipRow, const FEquipmentInfo* InEquipInfo)
{
	if (!InCatalogRow || !InEquipRow || !InEquipInfo) return;

	if (CurrentTitleText)
	{
		CurrentTitleText->SetText(FText::FromString(TEXT("[현재]")));
	}

	if (NextTitleText)
	{
		NextTitleText->SetText(FText::FromString(TEXT("[강화 후]")));
	}

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	UGameDataSubsystem* dataSubsystem = gameInstance ? gameInstance->GetSubsystem<UGameDataSubsystem>() : nullptr;

	const int32 currentLevel = FMath::Max(0, InEquipInfo->CurrUpgradeLevel);
	const int32 maxLevel = FMath::Max(0, InEquipInfo->Equipment.MaxUpgradeLevel);

	// 장비 이름 + 현재 강화 수치 표시
	if (CurrentUpgradeText)
	{
		CurrentUpgradeText->SetText(FText::FromString(
			FString::Printf(TEXT("%s +%d"),
				*InCatalogRow->DisplayName.ToString(),
				currentLevel)));
	}

	// 이전에 표시된 스탯/확률 텍스트 초기화
	if (CurrentStatText1)
	{
		CurrentStatText1->SetText(FText::GetEmpty());
	}
	if (CurrentStatText2)
	{
		CurrentStatText2->SetText(FText::GetEmpty());
	}
	if (NextStatText1)
	{
		NextStatText1->SetText(FText::GetEmpty());
	}
	if (NextStatText2)
	{
		NextStatText2->SetText(FText::GetEmpty());
	}
	if (EnhanceChanceText)
	{
		EnhanceChanceText->SetText(FText::GetEmpty());
	}
	if (CurrentTitleText)
	{
		CurrentTitleText->SetVisibility(ESlateVisibility::Visible);
	}

	if (NextTitleText)
	{
		NextTitleText->SetVisibility(ESlateVisibility::Visible);
	}

	// 현재 장비의 실제 스탯 표시
	if (InEquipInfo->Equipment.OwnerStats.IsValidIndex(0))
	{
		const FDerivedStatModifier& currentStat0 = InEquipInfo->Equipment.OwnerStats[0];

		FString statDisplayName = currentStat0.StatTag.GetTagName().ToString();
		bool bUsePercent = false;

		if (dataSubsystem)
		{
			if (const FDTStatDisplayRow* statRow = dataSubsystem->FindStatDisplayRowByTag(currentStat0.StatTag))
			{
				statDisplayName = statRow->DisplayName.ToString();
				bUsePercent = statRow->bUsePercent;
			}
		}

		if (CurrentStatText1)
		{
			CurrentStatText1->SetText(FText::FromString(
				bUsePercent
				? FString::Printf(TEXT("%s %.0f%%"), *statDisplayName, currentStat0.Value.Flat)
				: FString::Printf(TEXT("%s %.0f"), *statDisplayName, currentStat0.Value.Flat)));
		}
	}

	if (InEquipInfo->Equipment.OwnerStats.IsValidIndex(1))
	{
		const FDerivedStatModifier& currentStat1 = InEquipInfo->Equipment.OwnerStats[1];

		FString statDisplayName = currentStat1.StatTag.GetTagName().ToString();
		bool bUsePercent = false;

		if (dataSubsystem)
		{
			if (const FDTStatDisplayRow* statRow = dataSubsystem->FindStatDisplayRowByTag(currentStat1.StatTag))
			{
				statDisplayName = statRow->DisplayName.ToString();
				bUsePercent = statRow->bUsePercent;
			}
		}

		if (CurrentStatText2)
		{
			CurrentStatText2->SetText(FText::FromString(
				bUsePercent
				? FString::Printf(TEXT("%s %.0f%%"), *statDisplayName, currentStat1.Value.Flat)
				: FString::Printf(TEXT("%s %.0f"), *statDisplayName, currentStat1.Value.Flat)));
		}
	}

	// 최대 강화면 MAX 표시
	if (currentLevel >= maxLevel)
	{
		if (NextTitleText)
		{
			NextTitleText->SetText(FText::FromString(TEXT("MAX")));
		}
		if (NextStatText1)
		{
			NextStatText1->SetText(FText::FromString(TEXT("MAX")));
		}
		if (EnhanceChanceText)
		{
			EnhanceChanceText->SetText(FText::FromString(TEXT("성공 확률 -")));
		}

		return;
	}

	// 다음 강화 단계와 성공 확률 계산
	const int32 nextLevel = currentLevel + 1;
	static const int32 enhanceChances[5] = { 100, 50, 25, 12, 6 };

	// 다음 강화 후 스탯 표시
	if (InEquipRow->BaseInfoSteps.IsValidIndex(nextLevel) && InEquipRow->BaseInfoSteps[nextLevel].RandomStatRanges.IsValidIndex(0))
	{
		const FStatRangeDefinition& range0 = InEquipRow->BaseInfoSteps[nextLevel].RandomStatRanges[0];

		FString statDisplayName = range0.StatTag.GetTagName().ToString();
		bool bUsePercent = false;

		if (dataSubsystem)
		{
			if (const FDTStatDisplayRow* statRow = dataSubsystem->FindStatDisplayRowByTag(range0.StatTag))
			{
				statDisplayName = statRow->DisplayName.ToString();
				bUsePercent = statRow->bUsePercent;
			}
		}

		const float minFlat0 = FMath::Min(range0.MinValue.Flat, range0.MaxValue.Flat);
		const float maxFlat0 = FMath::Max(range0.MinValue.Flat, range0.MaxValue.Flat);

		if (NextStatText1)
		{
			NextStatText1->SetText(FText::FromString(
				bUsePercent
				? FString::Printf(TEXT("%s %.0f~%.0f%%"), *statDisplayName, minFlat0, maxFlat0)
				: FString::Printf(TEXT("%s %.0f~%.0f"), *statDisplayName, minFlat0, maxFlat0)));
		}
	}

	if (InEquipRow->BaseInfoSteps.IsValidIndex(nextLevel) && InEquipRow->BaseInfoSteps[nextLevel].RandomStatRanges.IsValidIndex(1))
	{
		const FStatRangeDefinition& range1 = InEquipRow->BaseInfoSteps[nextLevel].RandomStatRanges[1];

		FString statDisplayName = range1.StatTag.GetTagName().ToString();
		bool bUsePercent = false;

		if (dataSubsystem)
		{
			if (const FDTStatDisplayRow* statRow = dataSubsystem->FindStatDisplayRowByTag(range1.StatTag))
			{
				statDisplayName = statRow->DisplayName.ToString();
				bUsePercent = statRow->bUsePercent;
			}
		}

		const float minFlat1 = FMath::Min(range1.MinValue.Flat, range1.MaxValue.Flat);
		const float maxFlat1 = FMath::Max(range1.MinValue.Flat, range1.MaxValue.Flat);

		if (NextStatText2)
		{
			NextStatText2->SetText(FText::FromString(
				bUsePercent
				? FString::Printf(TEXT("%s %.0f~%.0f%%"), *statDisplayName, minFlat1, maxFlat1)
				: FString::Printf(TEXT("%s %.0f~%.0f"), *statDisplayName, minFlat1, maxFlat1)));
		}
	}

	// 현재 강화 단계 기준 성공 확률 표시
	if (EnhanceChanceText)
	{
		const int32 chanceIndex = FMath::Clamp(currentLevel, 0, 4);
		EnhanceChanceText->SetText(FText::FromString(
			FString::Printf(TEXT("성공 확률 %d%%"), enhanceChances[chanceIndex])));
	}
}

void UEnhancementHUDWidget::ClearEnhancementInfo()
{
	if (CurrentUpgradeText)
	{
		CurrentUpgradeText->SetText(FText::GetEmpty());
	}

	if (CurrentStatText1)
	{
		CurrentStatText1->SetText(FText::GetEmpty());
	}
	if (CurrentStatText2)
	{
		CurrentStatText2->SetText(FText::GetEmpty());
	}

	if (NextStatText1)
	{
		NextStatText1->SetText(FText::GetEmpty());
	}
	if (NextStatText2)
	{
		NextStatText2->SetText(FText::GetEmpty());
	}

	if (EnhanceChanceText)
	{
		EnhanceChanceText->SetText(FText::GetEmpty());
	}

	if (CurrentTitleText)
	{
		CurrentTitleText->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (NextTitleText)
	{
		NextTitleText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UEnhancementHUDWidget::HandleEnhanceButtonClicked()
{
	if (!SelectedItemGuid.IsValid()) return;

	if (!FPlayerAccountService::EnhanceEquipment(this, SelectedItemGuid)) return;

	if (ParentLobby)
	{
		ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
	}

	RefreshEquipmentInventory();

	RefreshSelectedItemSlot();
}

void UEnhancementHUDWidget::HandleRerollButtonClicked()
{
	if (!SelectedItemGuid.IsValid()) return;

	if (!FPlayerAccountService::RerollEquipment(this, SelectedItemGuid)) return;

	if (ParentLobby)
	{
		ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
	}

	RefreshEquipmentInventory();

	RefreshSelectedItemSlot();
}

void UEnhancementHUDWidget::HandleDisassembleButtonClicked()
{
	if (!SelectedItemGuid.IsValid()) return;

	if (!FPlayerAccountService::DisassembleEquipment(this, SelectedItemGuid)) return;

	SelectedItemGuid.Invalidate();

	if (ParentLobby)
	{
		ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
	}

	RefreshEquipmentInventory();

	if (SelectedItemSlotWidget)
	{
		SelectedItemSlotWidget->ClearSlot(0);
		SelectedItemSlotWidget->SetSelected(false);
	}

	ClearEnhancementInfo();
}

void UEnhancementHUDWidget::HandleDetailChanceButtonClicked()
{
	if (!DetailChanceText) return;

	const ESlateVisibility currentVisibility = DetailChancePanel->GetVisibility();
	DetailChancePanel->SetVisibility(
		currentVisibility == ESlateVisibility::Collapsed ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

}

void UEnhancementHUDWidget::RefreshDetailChanceText()
{
	if (!DetailChanceText) return;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(GetGameInstance());
	if (!gameInstance)
	{
		DetailChanceText->SetText(FText::GetEmpty());
		return;
	}

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem)
	{
		DetailChanceText->SetText(FText::GetEmpty());
		return;
	}

	FString chanceText;

	for (int32 level = 0; level < 5; level++)
	{
		const FName rowName = FName(*FString::Printf(TEXT("Level_%d"), level));
		const FDTEnhanceRuleRow* ruleRow = dataSubsystem->GetRow<FDTEnhanceRuleRow>(Arcanum::DataTable::EnhanceRule, rowName);

		if (ruleRow)
		{
			chanceText += FString::Printf(
				TEXT("+%d → +%d : %d%%"),
				level,
				level + 1,
				ruleRow->EnhanceSuccessRate);

			if (level < 4)
			{
				chanceText += TEXT("\n");
			}
		}
	}

	DetailChanceText->SetText(FText::FromString(chanceText));

}

