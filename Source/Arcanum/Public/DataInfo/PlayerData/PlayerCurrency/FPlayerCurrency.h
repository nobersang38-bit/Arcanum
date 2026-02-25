#pragma once

#include "CoreMinimal.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "FPlayerCurrency.generated.h"

USTRUCT(BlueprintType)
struct FCurrencyData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 CurrAmount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 MaxAmount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalEarned = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FDateTime LastRegenTime;

    bool operator==(const FCurrencyData& Other) const
    {
        return CurrAmount == Other.CurrAmount && 
               MaxAmount == Other.MaxAmount && 
               TotalEarned == Other.TotalEarned;
    }
};

USTRUCT(BlueprintType)
struct FPlayerCurrency
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    TMap<FGameplayTag, FCurrencyData> CurrencyDatas;

    bool operator==(const FPlayerCurrency& Other) const
    {
        if (CurrencyDatas.Num() != Other.CurrencyDatas.Num()) return false;
        for (const auto& Pair : CurrencyDatas) {
            const FCurrencyData* OtherValue = Other.CurrencyDatas.Find(Pair.Key);
            if (!OtherValue || !(*OtherValue == Pair.Value)) return false;
        }

        return true;
    }
};