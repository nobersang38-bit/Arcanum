#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"
#include "Arcanum/Public/GameplayTags/ArcanumTags.h"
#include "DataInfo/ItemData/DataTable/DTItemCatalogRow.h"
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

		BuildItemCatalogRowCache();
	}

#pragma region 아이템 카탈로그 DT 캐시
public:
	/* ItemTag로 캐시에서 ItemCatalog RowPtr 조회 */
	const FDTItemCatalogRow* FindItemCatalogRowByTag(const FGameplayTag& InItemTag) const
	{
		if (!InItemTag.IsValid()) return nullptr;
		if (const FDTItemCatalogRow* const* found = ItemCatalogRowByTag.Find(InItemTag))
		{
			return *found;
		}
		return nullptr;
	}

private:
	/* ItemCatalog DT를 한 번 훑어서 ItemTag -> RowPtr 캐시 구축 */
	void BuildItemCatalogRowCache()
	{
		ItemCatalogRowByTag.Reset();

		UDataTable** tablePtr = MasterDataTables.Find(Arcanum::DataTable::ItemCatalog);
		if (!tablePtr || !(*tablePtr)) return;

		UDataTable* table = *tablePtr;
		for (const TPair<FName, uint8*>& pair : table->GetRowMap())
		{
			if (const FDTItemCatalogRow* row = reinterpret_cast<const FDTItemCatalogRow*>(pair.Value))
			{
				if (row->ItemTag.IsValid())
				{
					if (!ItemCatalogRowByTag.Contains(row->ItemTag))
					{
						ItemCatalogRowByTag.Add(row->ItemTag, row);
					}
				}
			}
		}
	}

private:
	/* ItemTag -> ItemCatalog RowPtr 캐시 */
	TMap<FGameplayTag, const FDTItemCatalogRow*> ItemCatalogRowByTag;
#pragma endregion

};
