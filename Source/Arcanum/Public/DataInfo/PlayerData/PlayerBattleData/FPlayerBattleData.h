#pragma once

#include "CoreMinimal.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "FPlayerBattleData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerBattleData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FRegenStat> Meat;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FNonRegenStat> CommandCenterStamina;
};
