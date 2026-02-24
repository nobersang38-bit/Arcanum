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
				// 슬롯 배열 크기 맞춤
				saveGame->CurrentShopRowNames.SetNum(InShopSlotCount);
				saveGame->CurrentShopSoldOutStates.SetNum(InShopSlotCount);

				// 새 상점 4칸(또는 가변 슬롯 수) 생성
				GenerateShopItems(InGameInstance, InShopSlotCount);

				// 품절 상태 초기화
				ResetShopSoldOutStates(InGameInstance, InShopSlotCount);

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

// TODO : 확률 데이터 따로 만들어야 됨.
//        함수 나누어야 됨. 
//        오류 확인 안됨.
//void FPlayerAccountService::GenerateShopItems(UARGameInstance* InGameInstance, int32 InShopSlotCount)
//{
//	if (InGameInstance)
//	{
//		if (InShopSlotCount > 0)
//		{
//			if (UArcanumSaveGame* saveGame = InGameInstance->GetArSaveGame())
//			{
//				if (UGameDataSubsystem* dataSubsystem = InGameInstance->GetSubsystem<UGameDataSubsystem>())
//				{
//					UDataTable* equipmentTable = nullptr;
//
//					if (UDataTable** tablePtr = dataSubsystem->MasterDataTables.Find(Arcanum::DataTable::Equipment))
//					{
//						if (equipmentTable = *tablePtr)
//						{
//							TArray<FName> commonRows;
//							TArray<FName> setRows;
//							TArray<FName> legendaryRows;
//
//							const FGameplayTag commonRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Rarity.Common"), false);
//							const FGameplayTag setRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Rarity.SetItem"), false);
//							const FGameplayTag legendaryRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Rarity.Legendary"), false);
//
//							for (const TPair<FName, uint8*>& rowPair : equipmentTable->GetRowMap())
//							{
//								if (const FDTEquipmentInfoRow* row = reinterpret_cast<const FDTEquipmentInfoRow*>(rowPair.Value))
//								{
//									if (row->BaseInfoSteps.IsEmpty())
//									{
//										continue;
//									}
//
//									// 노강 장비만
//									const FGameplayTag itemTag = row->ItemTag;
//
//									if (itemTag.IsValid())
//									{
//										if (legendaryRootTag.IsValid() && itemTag.MatchesTag(legendaryRootTag))
//										{
//											legendaryRows.Add(rowPair.Key);
//										}
//										else if (setRootTag.IsValid() && itemTag.MatchesTag(setRootTag))
//										{
//											setRows.Add(rowPair.Key);
//
//										}
//										else
//										{
//											commonRows.Add(rowPair.Key);
//										}
//									}
//								}
//							}
//
//							saveGame->CurrentShopRowNames.SetNum(InShopSlotCount);
//
//							auto pickAndRemove = [](TArray<FName>& InRows, FName& OutRowName) -> bool
//								{
//									if (InRows.Num() <= 0)
//									{
//										return false;
//									}
//
//									const int32 randomIndex = FMath::RandRange(0, InRows.Num() - 1);
//									OutRowName = InRows[randomIndex];
//									InRows.RemoveAt(randomIndex);
//									return true;
//								};
//
//							for (int32 slotIndex = 0; slotIndex < InShopSlotCount; slotIndex++)
//							{
//								FName pickedRowName = NAME_None;
//
//								// 확률: 전설 5%, 세트 10%, 일반 85%
//								const int32 roll = FMath::RandRange(1, 100);
//
//								if (roll <= 5)
//								{
//									if (!pickAndRemove(legendaryRows, pickedRowName))
//									{
//										if (!pickAndRemove(setRows, pickedRowName))
//										{
//											pickAndRemove(commonRows, pickedRowName);
//										}
//									}
//								}
//								else if (roll <= 15)
//								{
//									if (!pickAndRemove(setRows, pickedRowName))
//									{
//										if (!pickAndRemove(legendaryRows, pickedRowName))
//										{
//											pickAndRemove(commonRows, pickedRowName);
//										}
//									}
//								}
//								else
//								{
//									if (!pickAndRemove(commonRows, pickedRowName))
//									{
//										if (!pickAndRemove(setRows, pickedRowName))
//										{
//											pickAndRemove(legendaryRows, pickedRowName);
//										}
//									}
//								}
//
//								saveGame->CurrentShopRowNames[slotIndex] = pickedRowName;
//							}
//						}
//					}
//				}
//			}
//		}
//	}
//}

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
