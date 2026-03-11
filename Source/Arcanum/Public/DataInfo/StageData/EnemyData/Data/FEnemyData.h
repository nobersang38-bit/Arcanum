#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "FEnemyData.generated.h"


USTRUCT(BlueprintType)
struct FEnemyBasement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FNonRegenStat CommandCenterHP;
};

/**
 * @brief 적 스폰정보
 */
USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FGameplayTag EnemyTag;

    UPROPERTY(EditAnywhere)
    TSoftClassPtr<class ABaseUnitCharacter> EnemyClass;

    /** 적 계수*/
    UPROPERTY(EditAnywhere)
    float SpawnMultiplier;
};

