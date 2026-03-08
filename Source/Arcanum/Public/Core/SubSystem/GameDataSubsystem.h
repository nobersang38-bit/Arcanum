#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"
#include "Arcanum/Public/GameplayTags/ArcanumTags.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
#include "DataInfo/ItemData/DataTable/DTStatDisplayRow.h"
#include "Core/Settings/DataTableSettings.h"
#include "GameDataSubsystem.generated.h"

UCLASS()
class ARCANUM_API UGameDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FGameplayTag, UDataTable*> MasterDataTables;

	template<typename T>
	T* GetRow(FGameplayTag TableTag, FName RowName)
	{
		if (UDataTable** TablePtr = MasterDataTables.Find(TableTag)) return (*TablePtr)->FindRow<T>(RowName, TEXT(""));
		else return nullptr;
	}

	virtual void Initialize(FSubsystemCollectionBase& Collection) override
	{
		Super::Initialize(Collection);

		const UDataTableSettings* Settings = GetDefault<UDataTableSettings>();
		if (!Settings) return;

		for (const auto& Pair : Settings->DataTableConfig) {
			const FGameplayTag& TableTag = Pair.Key;
			const TSoftObjectPtr<UDataTable>& TablePtr = Pair.Value;
			if (UDataTable* LoadedTable = TablePtr.LoadSynchronous())
				MasterDataTables.Add(TableTag, LoadedTable);
		}
	}

#pragma region 아이템 카탈로그 DT 조회
public:
	/* ItemCatalog DT를 순회해 ItemTag와 일치하는 Row행을 조회 */
	const FDTItemCatalogRow* FindItemCatalogRowByTag(const FGameplayTag& InItemTag) const
	{
		if (!InItemTag.IsValid()) return nullptr;

		UDataTable* const* tablePtr = MasterDataTables.Find(Arcanum::DataTable::ItemCatalog);
		if (!tablePtr || !(*tablePtr)) return nullptr;

		UDataTable* table = *tablePtr;

		for (const TPair<FName, uint8*>& pair : table->GetRowMap())
		{
			const FDTItemCatalogRow* row = reinterpret_cast<const FDTItemCatalogRow*>(pair.Value);
			if (row && row->ItemTag.MatchesTagExact(InItemTag))
			{
				return row;
			}
		}

		return nullptr;
	}
#pragma endregion

#pragma region 스탯 디스플레이 DT 조회
public:
	const FDTStatDisplayRow* FindStatDisplayRowByTag(const FGameplayTag& InStatTag) const
	{
		if (!InStatTag.IsValid())
		{
			return nullptr;
		}

		UDataTable* const* tablePtr = MasterDataTables.Find(Arcanum::DataTable::StatDisplay);
		if (!tablePtr || !(*tablePtr))
		{
			return nullptr;
		}

		UDataTable* table = *tablePtr;

		for (const TPair<FName, uint8*>& pair : table->GetRowMap())
		{
			const FDTStatDisplayRow* row = reinterpret_cast<const FDTStatDisplayRow*>(pair.Value);
			if (row && row->StatTag.MatchesTagExact(InStatTag))
			{
				return row;
			}
		}

		return nullptr;
	}
#pragma endregion
};
