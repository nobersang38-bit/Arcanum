#include "Core/ARPlayerAccountService.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "Core/ARGameInstance.h"

// ========================================================
// Battle Widget 관련
// ========================================================

// ========================================================
// Character Widget 관련
// ========================================================

// ========================================================
// Enhancement Widget 관련
// ========================================================

// ========================================================
// Shop Widget 관련
// ========================================================
bool FPlayerAccountService::PurchaseEquipment(UARGameInstance* GameInstance, FName RowName)
{
	if (!GameInstance) return false;

	UGameDataSubsystem* DataSubsystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
	if (!DataSubsystem) return false;

	FPlayerData& PlayerData = GameInstance->GetPlayerData();
	const FDTEquipmentInfoRow* Row = DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, RowName);

	if (!Row) return false;
	if (Row->BaseInfoSteps.IsEmpty()) return false;

	FCurrencyData* CurrencyData = PlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);

	if (!CurrencyData) return false;

	const int64 Price = static_cast<int64>(Row->BuyPrice);
	if (CurrencyData->CurrAmount < Price) return false;

	FEquipmentInfo NewEquip;
	NewEquip.ItemTag = Row->ItemTag;
	NewEquip.ItemGuid = FGuid::NewGuid();
	NewEquip.CurrUpgradeLevel = 0;
	NewEquip.Equipment = Row->BaseInfoSteps[0];
	PlayerData.Inventory.Add(MoveTemp(NewEquip));
	CurrencyData->CurrAmount -= Price;

	GameInstance->OnCurrencyChanged.Broadcast(); // 재화 변경 알림

	GameInstance->SavePlayerData();

	return true;
}
const FDTEquipmentInfoRow* FPlayerAccountService::GetItemDefinition(UGameDataSubsystem* DataSubsystem, const FGameplayTag& ItemTag)
{
	if (!DataSubsystem) return nullptr;
	return DataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, ItemTag.GetTagName());
}

// ========================================================
// Shop Widget 관련
// ========================================================
void FPlayerAccountService::InitializeShop(UARGameInstance* InGameInstance, int32 InShopSlotCount)
{
	if (InGameInstance)
	{
		if (InShopSlotCount > 0)
		{
			// 세이브 데이터 가져오기
			if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
			{
				const bool bDataValid = (saveGame->CurrentShopRowNames.Num() == InShopSlotCount) &&
					(saveGame->CurrentShopSoldOutStates.Num() == InShopSlotCount) &&
					(saveGame->NextShopRefreshTime.GetTicks() > 0);

				if (!bDataValid || IsShopRefreshExpired(InGameInstance))
				{
					RefreshShop(InGameInstance, InShopSlotCount);
				}

				if (UGameTimeSubsystem* gameTimeSubsystem = InGameInstance->GetSubsystem<UGameTimeSubsystem>())
				{
					gameTimeSubsystem->StartShop(saveGame->NextShopRefreshTime);
				}
			}
		}
	}
}

void FPlayerAccountService::RefreshShop(UARGameInstance* InGameInstance, int32 InShopSlotCount)
{
	if (InGameInstance)
	{
		if (InShopSlotCount > 0)
		{
			if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
			{
				// 새 상점 아이템 생성
				GenerateShopItems(InGameInstance, InShopSlotCount);

				// 품절 상태 초기화
				saveGame->CurrentShopSoldOutStates.SetNum(saveGame->CurrentShopRowNames.Num());
				ResetShopSoldOutStates(InGameInstance, saveGame->CurrentShopRowNames.Num());

				// 다음 갱신 시각 저장
				SetNextShopRefreshTime(InGameInstance);

				// 세이브 저장
				InGameInstance->SavePlayerData();

				// 상점 타이머 시작
				if (UGameTimeSubsystem* gameTimeSubsystem = InGameInstance->GetSubsystem<UGameTimeSubsystem>())
				{
					gameTimeSubsystem->StartShop(saveGame->NextShopRefreshTime);
				}
			}
		}
	}
}

bool FPlayerAccountService::GetShopSlotData(UARGameInstance* InGameInstance, int32 InSlotIndex, FName& OutRowName, bool& OutSoldOut)
{
	OutRowName = NAME_None;
	OutSoldOut = false;

	if (InGameInstance)
	{
		if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
		{
			if (saveGame->CurrentShopRowNames.IsValidIndex(InSlotIndex) &&
				saveGame->CurrentShopSoldOutStates.IsValidIndex(InSlotIndex))
			{
				OutRowName = saveGame->CurrentShopRowNames[InSlotIndex];
				OutSoldOut = saveGame->CurrentShopSoldOutStates[InSlotIndex];
				return true;
			}
		}
	}

	return false;
}

bool FPlayerAccountService::SetShopSlotSoldOut(UARGameInstance* InGameInstance, int32 InSlotIndex)
{
	if (InGameInstance)
	{
		if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
		{
			if (saveGame->CurrentShopSoldOutStates.IsValidIndex(InSlotIndex))
			{
				saveGame->CurrentShopSoldOutStates[InSlotIndex] = true;
				InGameInstance->SavePlayerData();

				return true;
			}
		}
	}

	return false;
}

int32 FPlayerAccountService::GetShopRemainingSeconds(UARGameInstance* InGameInstance)
{
	if (InGameInstance)
	{
		if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
		{
			if (saveGame->NextShopRefreshTime.GetTicks() > 0)
			{
				const FDateTime currentTimeKST = GetCurrentTimeKST();
				const FTimespan difference = saveGame->NextShopRefreshTime - currentTimeKST;

				return FMath::Max(0, static_cast<int32>(difference.GetTotalSeconds()));
			}
		}
	}

	return 0;
}

bool FPlayerAccountService::IsShopRefreshExpired(UARGameInstance* InGameInstance)
{
	if (InGameInstance)
	{
		if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
		{
			if (saveGame->NextShopRefreshTime.GetTicks() > 0)
			{
				const FDateTime currentTimeKST = GetCurrentTimeKST();

				return (currentTimeKST >= saveGame->NextShopRefreshTime);
			}
		}
	}

	return true;
}

void FPlayerAccountService::GenerateShopItems(UARGameInstance* InGameInstance, int32 InShopSlotCount)
{
	if (InGameInstance)
	{
		if (UArcanumSaveGame* SaveGame = InGameInstance->GetArSaveGame())
		{
			if (InShopSlotCount > 0)
			{
				FShopItemPools ItemPools;
				BuildShopItemPools(InGameInstance, ItemPools);

				// 세이브 데이터 초기화
				SaveGame->CurrentShopRowNames.Reset();

				for (int32 i = 0; i < InShopSlotCount; ++i)
				{
					const EShopRarityType RarityType = PickShopRarityType(InGameInstance);
					const FName PickedRow = PickShopItemRow(RarityType, ItemPools);

					if (PickedRow != NAME_None)
					{
						SaveGame->CurrentShopRowNames.Add(PickedRow);
					}
				}
			}
		}
	}
}

void FPlayerAccountService::BuildShopItemPools(UARGameInstance* InGameInstance, FShopItemPools& OutItemPools)
{
	OutItemPools.CommonRows.Reset();
	OutItemPools.SetRows.Reset();
	OutItemPools.LegendaryRows.Reset();

	if (InGameInstance)
	{
		if (UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>())
		{
			UDataTable* equipmentTable = nullptr;

			if (UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment))
			{
				equipmentTable = *tablePtr;
			}

			if (equipmentTable)
			{
				const FGameplayTag setRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Rarity.SetItem"), false);
				const FGameplayTag legendaryRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Rarity.Legendary"), false);

				for (const TPair<FName, uint8*>& rowPair : equipmentTable->GetRowMap())
				{
					const FDTEquipmentInfoRow* row = reinterpret_cast<const FDTEquipmentInfoRow*>(rowPair.Value);

					if (row && !row->BaseInfoSteps.IsEmpty() && row->ItemTag.IsValid())
					{
						if (legendaryRootTag.IsValid() && row->ItemTag.MatchesTag(legendaryRootTag))
						{
							OutItemPools.LegendaryRows.Add(rowPair.Key);
						}
						else if (setRootTag.IsValid() && row->ItemTag.MatchesTag(setRootTag))
						{
							OutItemPools.SetRows.Add(rowPair.Key);
						}
						else
						{
							OutItemPools.CommonRows.Add(rowPair.Key);
						}
					}
				}
			}
		}
	}
}

EShopRarityType FPlayerAccountService::PickShopRarityType(UARGameInstance* InGameInstance)
{
	// TODO: 확률 DT 에서 가져올수도 있음
	const int32 legendaryChancePercent = 5;
	const int32 setChancePercent = 10;

	const int32 roll = FMath::RandRange(1, 100);

	if (roll <= legendaryChancePercent)
	{
		return EShopRarityType::Legendary;
	}

	if (roll <= (legendaryChancePercent + setChancePercent))
	{
		return EShopRarityType::Set;
	}

	return EShopRarityType::Common;
}

FName FPlayerAccountService::PickShopItemRow(EShopRarityType InRarityType, FShopItemPools& InOutItemPools)
{
	TArray<FName>* targetRows = nullptr;

	if (InRarityType == EShopRarityType::Legendary)
	{
		targetRows = &InOutItemPools.LegendaryRows;
	}
	else if (InRarityType == EShopRarityType::Set)
	{
		targetRows = &InOutItemPools.SetRows;
	}
	else
	{
		targetRows = &InOutItemPools.CommonRows;
	}

	if (!targetRows || targetRows->Num() <= 0)
	{
		return NAME_None;
	}

	const int32 randomIndex = FMath::RandRange(0, targetRows->Num() - 1);
	const FName pickedRowName = (*targetRows)[randomIndex];

	targetRows->RemoveAt(randomIndex);

	return pickedRowName;
}


void FPlayerAccountService::ResetShopSoldOutStates(UARGameInstance* InGameInstance, int32 InShopSlotCount)
{
	if (InGameInstance)
	{
		if (InShopSlotCount > 0)
		{
			if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
			{
				saveGame->CurrentShopSoldOutStates.SetNum(InShopSlotCount);

				for (int32 slotIndex = 0; slotIndex < InShopSlotCount; slotIndex++)
				{
					saveGame->CurrentShopSoldOutStates[slotIndex] = false;
				}
			}
		}
	}
}

void FPlayerAccountService::SetNextShopRefreshTime(UARGameInstance* InGameInstance)
{
	if (InGameInstance)
	{
		if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
		{
			if (UGameTimeSubsystem* gameTimeSubsystem = InGameInstance->GetSubsystem<UGameTimeSubsystem>())
			{
				const FDateTime currentTimeKST = GetCurrentTimeKST();
				saveGame->NextShopRefreshTime = currentTimeKST + FTimespan(0, 0, gameTimeSubsystem->ShopRefreshSeconds);
			}
		}
	}
}

FDateTime FPlayerAccountService::GetCurrentTimeKST()
{
	return FDateTime::UtcNow() + FTimespan(9, 0, 0);
}
