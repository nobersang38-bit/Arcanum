#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "FGradeStatData.generated.h"

USTRUCT(BlueprintType)
struct FGradeStatData 
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FRegenStat> RegenStats;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FNonRegenStat> NonRegenStats;
};