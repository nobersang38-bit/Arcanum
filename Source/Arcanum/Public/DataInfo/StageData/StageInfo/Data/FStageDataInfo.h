#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "DataInfo/StageData/EnemyData/Data/FEnemyData.h"
#include "FStageDataInfo.generated.h"

/**
 * @brief 개별 장비데이터를 보관하는 구조체
 */
USTRUCT(BlueprintType)
struct FStageReward
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 Gold;

    UPROPERTY(EditAnywhere)
    int32 Gem;
    UPROPERTY(EditAnywhere)
    int32 Soul;
};

USTRUCT(BlueprintType)
struct FStageDataInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag StageTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText StageName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText StageDesc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEnemyBasement EnemyBasement;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FEnemySpawnInfo> SpawnInfos;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> StageImg;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FStageReward Reward;
};