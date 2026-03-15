#include "UI/Lobby/Contents/ItemDetail/ItemDetailHelper.h"
#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "DataInfo/BattleCharacter/FBattleCharacterData.h"
#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "DataInfo/ItemData/DataTable/DTStatDisplayRow.h"
#include "DataInfo/ItemData/DataTable/DTPotionInfoRow.h"
#include "DataInfo/EnhancementData/DataTable/DTEnhanceRuleRow.h"
#include "Kismet/GameplayStatics.h"

bool FItemDetailHelper::BuildEquipmentDisplayViewData(const UObject* WorldContextObject, const FGuid& InItemGuid, FItemDisplayViewData& OutViewData)
{
	ClearDisplayViewData(OutViewData);

	if (!WorldContextObject) return false;
	if (!InItemGuid.IsValid()) return false;

	FEquipmentInfo equipInfo;
	if (!FindEquipmentByGuid(WorldContextObject, InItemGuid, equipInfo)) return false;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!gameInstance) return false;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(WorldContextObject, equipInfo.ItemTag);
	if (!catalogRow) return false;

	OutViewData.ItemNameText = catalogRow->DisplayName;
	OutViewData.UpgradeLevelText = BuildUpgradeLevelText(equipInfo.CurrUpgradeLevel);
	OutViewData.DescText = catalogRow->Desc;
	OutViewData.SellPriceText = BuildSellPriceText(catalogRow->SellPrice);

	OutViewData.bShowUpgradeLevel = true;

	for (const FDerivedStatModifier& stat : equipInfo.Equipment.OwnerStats)
	{
		FItemStatLineViewData line;
		line.StatNameText = BuildStatNameText(dataSubsystem, stat.StatTag);

		if (const FDTStatDisplayRow* statRow = FindStatDisplayRowByTag(dataSubsystem, stat.StatTag))
		{
			line.StatValueText = BuildCurrentStatValueText(stat, statRow->bUsePercent);
		}
		else
		{
			line.StatValueText = BuildCurrentStatValueText(stat, false);
		}

		line.bVisible = true;
		OutViewData.StatLines.Add(line);
	}

	return true;
}

bool FItemDetailHelper::BuildStackItemDisplayViewData(const UObject* WorldContextObject, const FGameplayTag& InItemTag, FItemDisplayViewData& OutViewData)
{
	ClearDisplayViewData(OutViewData);

	if (!WorldContextObject) return false;
	if (!InItemTag.IsValid()) return false;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!gameInstance) return false;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(WorldContextObject, InItemTag);
	if (!catalogRow) return false;

	OutViewData.ItemNameText = catalogRow->DisplayName;
	OutViewData.DescText = catalogRow->Desc;
	OutViewData.SellPriceText = BuildSellPriceText(catalogRow->SellPrice);

	OutViewData.bShowUpgradeLevel = false;

	if (catalogRow->DetailTableTag.MatchesTagExact(Arcanum::DataTable::Potion))
	{
		const FDTPotionInfoRow* potionRow = dataSubsystem->GetRow<FDTPotionInfoRow>(Arcanum::DataTable::Potion, catalogRow->DetailRowName);
		if (potionRow)
		{
			FItemStatLineViewData cooldownLine;
			cooldownLine.StatNameText = FText::FromString(TEXT("쿨타임"));
			cooldownLine.StatValueText = BuildCooldownText(potionRow->CooldownSeconds);
			cooldownLine.bVisible = true;
			OutViewData.StatLines.Add(cooldownLine);

			for (const FDerivedStatModifier& stat : potionRow->Modifiers)
			{
				FItemStatLineViewData line;
				line.StatNameText = BuildStatNameText(dataSubsystem, stat.StatTag);

				if (const FDTStatDisplayRow* statRow = FindStatDisplayRowByTag(dataSubsystem, stat.StatTag))
				{
					line.StatValueText = BuildCurrentStatValueText(stat, statRow->bUsePercent);
				}
				else
				{
					line.StatValueText = BuildCurrentStatValueText(stat, false);
				}

				line.bVisible = true;
				OutViewData.StatLines.Add(line);
			}
		}
	}

	return true;
}

bool FItemDetailHelper::BuildEquippedTotalStatsViewData(const UObject* WorldContextObject, const FName& InCharacterName, FEquippedTotalStatViewData& OutViewData)
{
	ClearEquippedTotalStatViewData(OutViewData);

	if (!WorldContextObject) return false;
	if (InCharacterName.IsNone()) return false;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!gameInstance) return false;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FPlayerData playerData = FPlayerAccountService::GetPlayerDataCopy(WorldContextObject);

	const FBattleCharacterData* foundCharacter = nullptr;
	for (const FBattleCharacterData& characterData : playerData.OwnedCharacters)
	{
		const FName characterName = GetLeafNameFromTag(characterData.CharacterInfo.BattleCharacterInitData.CharacterTag);
		if (characterName == InCharacterName)
		{
			foundCharacter = &characterData;
			break;
		}
	}
	if (!foundCharacter) return false;

	TMap<FGameplayTag, FExternalStatModifier> totalStats;
	auto AccumulateEquipmentStats =
		[&playerData, &totalStats](const TMap<FGameplayTag, FGuid>& InEquipmentMap)
		{
			for (const TPair<FGameplayTag, FGuid>& pair : InEquipmentMap)
			{
				const FGuid& itemGuid = pair.Value;
				if (itemGuid.IsValid())
				{
					const FEquipmentInfo* foundEquip = nullptr;
					for (const FEquipmentInfo& equip : playerData.Inventory)
					{
						if (equip.ItemGuid == itemGuid)
						{
							foundEquip = &equip;
							break;
						}
					}
					if (foundEquip)
					{
						for (const FDerivedStatModifier& stat : foundEquip->Equipment.OwnerStats)
						{
							if (stat.StatTag.IsValid())
							{
								FExternalStatModifier& totalValue = totalStats.FindOrAdd(stat.StatTag);
								totalValue.Flat += stat.Value.Flat;
								totalValue.Mul += stat.Value.Mul;
							}
						}
					}
				}
			}
		};

	AccumulateEquipmentStats(foundCharacter->WeaponSlots);
	AccumulateEquipmentStats(foundCharacter->LegendaryWeaponSlots);
	AccumulateEquipmentStats(foundCharacter->ArmorSlots);

	TArray<FGameplayTag> sortedTags;
	totalStats.GetKeys(sortedTags);

	sortedTags.Sort(
		[dataSubsystem](const FGameplayTag& InA, const FGameplayTag& InB)
		{
			int32 sortOrderA = MAX_int32;
			int32 sortOrderB = MAX_int32;

			if (const FDTStatDisplayRow* statRowA = FItemDetailHelper::FindStatDisplayRowByTag(dataSubsystem, InA))
			{
				sortOrderA = statRowA->SortOrder;
			}

			if (const FDTStatDisplayRow* statRowB = FItemDetailHelper::FindStatDisplayRowByTag(dataSubsystem, InB))
			{
				sortOrderB = statRowB->SortOrder;
			}

			if (sortOrderA != sortOrderB)
			{
				return sortOrderA < sortOrderB;
			}

			return InA.GetTagName().LexicalLess(InB.GetTagName());
		});

	for (const FGameplayTag& statTag : sortedTags)
	{
		const FExternalStatModifier* totalValue = totalStats.Find(statTag);
		if (totalValue)
		{
			FItemStatLineViewData line;
			line.StatNameText = BuildStatNameText(dataSubsystem, statTag);

			FDerivedStatModifier totalStat;
			totalStat.StatTag = statTag;
			totalStat.Value = *totalValue;

			if (const FDTStatDisplayRow* statRow = FindStatDisplayRowByTag(dataSubsystem, statTag))
			{
				line.StatValueText = BuildCurrentStatValueText(totalStat, statRow->bUsePercent);
			}
			else
			{
				line.StatValueText = BuildCurrentStatValueText(totalStat, false);
			}

			line.bVisible = true;
			OutViewData.StatLines.Add(line);
		}
	}

	return true;
}

void FItemDetailHelper::ClearDisplayViewData(FItemDisplayViewData& OutViewData)
{
	OutViewData.ItemNameText = FText::GetEmpty();
	OutViewData.UpgradeLevelText = FText::GetEmpty();
	OutViewData.StatLines.Empty();
	OutViewData.DescText = FText::GetEmpty();
	OutViewData.SellPriceText = FText::GetEmpty();

	OutViewData.bShowUpgradeLevel = false;
}

bool FItemDetailHelper::BuildEnhancementNextStatViewData(const UObject* WorldContextObject, const FGuid& InItemGuid, FEnhancementNextStatViewData& OutViewData)
{
	ClearEnhancementNextStatViewData(OutViewData);

	if (!WorldContextObject) return false;
	if (!InItemGuid.IsValid()) return false;

	FEquipmentInfo equipInfo;
	if (!FindEquipmentByGuid(WorldContextObject, InItemGuid, equipInfo)) return false;

	UARGameInstance* gameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!gameInstance) return false;

	UGameDataSubsystem* dataSubsystem = gameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!dataSubsystem) return false;

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(WorldContextObject, equipInfo.ItemTag);
	if (!catalogRow) return false;
	if (!catalogRow->DetailTableTag.MatchesTagExact(Arcanum::DataTable::Equipment)) return false;
	if (catalogRow->DetailRowName.IsNone()) return false;

	const FDTEquipmentInfoRow* equipRow = dataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
	if (!equipRow) return false;

	const int32 nextLevel = equipInfo.CurrUpgradeLevel + 1;
	if (!equipRow->BaseInfoSteps.IsValidIndex(nextLevel)) return false;

	const FName ruleRowName = FName(*FString::Printf(TEXT("Level_%d"), equipInfo.CurrUpgradeLevel));
	const FDTEnhanceRuleRow* ruleRow = dataSubsystem->GetRow<FDTEnhanceRuleRow>(Arcanum::DataTable::EnhanceRule, ruleRowName);
	if (ruleRow)
	{
		OutViewData.EnhanceChanceText = BuildEnhanceChanceText(ruleRow->EnhanceSuccessRate);
	}

	for (const FStatRangeDefinition& statRange : equipRow->BaseInfoSteps[nextLevel].RandomStatRanges)
	{
		FItemStatLineViewData line;
		line.StatNameText = BuildStatNameText(dataSubsystem, statRange.StatTag);

		if (const FDTStatDisplayRow* statRow = FindStatDisplayRowByTag(dataSubsystem, statRange.StatTag))
		{
			line.StatValueText = BuildNextStatValueText(statRange, statRow->bUsePercent);
		}
		else
		{
			line.StatValueText = BuildNextStatValueText(statRange, false);
		}

		line.bVisible = true;
		OutViewData.NextStatLines.Add(line);
	}

	return true;
}

void FItemDetailHelper::ClearEnhancementNextStatViewData(FEnhancementNextStatViewData& OutViewData)
{
	OutViewData.NextStatLines.Empty();
	OutViewData.EnhanceChanceText = FText::GetEmpty();
}

void FItemDetailHelper::ClearEquippedTotalStatViewData(FEquippedTotalStatViewData& OutViewData)
{
	OutViewData.StatLines.Empty();
}

bool FItemDetailHelper::FindEquipmentByGuid(const UObject* WorldContextObject, const FGuid& InItemGuid, FEquipmentInfo& OutEquipmentInfo)
{
	OutEquipmentInfo = FEquipmentInfo();

	if (!WorldContextObject) return false;
	if (!InItemGuid.IsValid()) return false;

	const FPlayerData playerData = FPlayerAccountService::GetPlayerDataCopy(WorldContextObject);

	for (const FEquipmentInfo& equip : playerData.Inventory)
	{
		if (equip.ItemGuid == InItemGuid)
		{
			OutEquipmentInfo = equip;
			return true;
		}
	}

	return false;
}

const FDTStatDisplayRow* FItemDetailHelper::FindStatDisplayRowByTag(UGameDataSubsystem* InDataSubsystem, const FGameplayTag& InStatTag)
{
	if (!InDataSubsystem) return nullptr;
	if (!InStatTag.IsValid()) return nullptr;

	UDataTable** tablePtr = InDataSubsystem->MasterDataTables.Find(Arcanum::DataTable::StatDisplay);
	if (!tablePtr || !(*tablePtr)) return nullptr;

	UDataTable* table = *tablePtr;
	for (const TPair<FName, uint8*>& pair : table->GetRowMap())
	{
		const FDTStatDisplayRow* row = reinterpret_cast<const FDTStatDisplayRow*>(pair.Value);
		if (row)
		{
			if (row->StatTag.MatchesTagExact(InStatTag))
			{
				return row;
			}
		}
	}

	return nullptr;
}

FText FItemDetailHelper::BuildStatNameText(UGameDataSubsystem* InDataSubsystem, const FGameplayTag& InStatTag)
{
	if (!InDataSubsystem) return FText::GetEmpty();
	if (!InStatTag.IsValid()) return FText::GetEmpty();

	const FDTStatDisplayRow* statRow = FindStatDisplayRowByTag(InDataSubsystem, InStatTag);
	if (statRow)
	{
		return statRow->DisplayName;
	}

	return FText::FromName(GetLeafNameFromTag(InStatTag));
}

FText FItemDetailHelper::BuildCurrentStatValueText(const FDerivedStatModifier& InStat, bool bInUsePercent)
{
	if (bInUsePercent)
	{
		const float percentValue = InStat.Value.Mul * 100.0f;
		return FText::FromString(FString::Printf(TEXT("+%.0f%%"), percentValue));
	}

	return FText::FromString(FString::Printf(TEXT("+%.0f"), InStat.Value.Flat));
}

FText FItemDetailHelper::BuildNextStatValueText(const FStatRangeDefinition& InRange, bool bInUsePercent)
{
	if (bInUsePercent)
	{
		const float minValue = FMath::Min(InRange.MinValue.Mul, InRange.MaxValue.Mul) * 100.0f;
		const float maxValue = FMath::Max(InRange.MinValue.Mul, InRange.MaxValue.Mul) * 100.0f;

		if (FMath::IsNearlyEqual(minValue, maxValue))
		{
			return FText::FromString(FString::Printf(TEXT("+%.0f%%"), minValue));
		}

		return FText::FromString(FString::Printf(TEXT("+%.0f%% ~ +%.0f%%"), minValue, maxValue));
	}

	const float minValue = FMath::Min(InRange.MinValue.Flat, InRange.MaxValue.Flat);
	const float maxValue = FMath::Max(InRange.MinValue.Flat, InRange.MaxValue.Flat);

	if (FMath::IsNearlyEqual(minValue, maxValue))
	{
		return FText::FromString(FString::Printf(TEXT("+%.0f"), minValue));
	}

	return FText::FromString(FString::Printf(TEXT("+%.0f ~ +%.0f"), minValue, maxValue));
}

FText FItemDetailHelper::BuildEnhanceChanceText(int32 InSuccessRate)
{
	return FText::FromString(FString::Printf(TEXT("%d%%"), InSuccessRate));
}

FText FItemDetailHelper::BuildSellPriceText(int64 InSellPrice)
{
	return FText::FromString(FString::Printf(TEXT("%lld"), InSellPrice));
}

FText FItemDetailHelper::BuildCooldownText(float InCooldownSeconds)
{
	return FText::FromString(FString::Printf(TEXT("%.1f초"), InCooldownSeconds));
}

FText FItemDetailHelper::BuildUpgradeLevelText(int32 InUpgradeLevel)
{
	return FText::FromString(FString::Printf(TEXT("+%d"), InUpgradeLevel));
}
