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
 * @brief 개별 장비데이터를 보관하는 구조체
 */
USTRUCT(BlueprintType)
struct FEnemySpawnInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FGameplayTag EnemyTag;

    UPROPERTY(EditAnywhere)
    float SpawnTime;

    UPROPERTY(EditAnywhere)
    int32 Count;
};

