#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NativeGameplayTags.h"
#include "DataInfo/BattleCharacter/BattleStats/Data/FGradeStatData.h"
#include "DTPlayerBattleStats.generated.h"

USTRUCT(BlueprintType)
struct FDTPlayerBattleStatsRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FRegenStat> PlayerBattleRegenStat;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FNonRegenStat> PlayerBattleNonRegenStat;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FGameplayTag, float> UpgradeValue;
};