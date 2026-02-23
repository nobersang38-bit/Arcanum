#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NativeGameplayTags.h"
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
};
