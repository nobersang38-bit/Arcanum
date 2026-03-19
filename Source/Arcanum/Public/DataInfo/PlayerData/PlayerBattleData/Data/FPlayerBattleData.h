#pragma once

#include "CoreMinimal.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "DataInfo/SkillData/Data/FBattleWeaponSkillData.h"
#include "FPlayerBattleData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerBattleData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FRegenStat> PlayerBattleRegenStat;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FNonRegenStat> PlayerBattleNonRegenStat;
};
