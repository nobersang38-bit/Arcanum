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
};

USTRUCT(BlueprintType)
struct FPlayerCurrency
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    TMap<FGameplayTag, FCurrencyData> CurrencyDatas;
};