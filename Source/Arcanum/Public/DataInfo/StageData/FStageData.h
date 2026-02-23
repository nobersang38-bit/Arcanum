#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "FStageData.generated.h"


USTRUCT(BlueprintType)
struct FStageProgressData
{
    GENERATED_BODY()

    UPROPERTY(SaveGame)
    FGameplayTag StageTag;

    UPROPERTY(SaveGame)
    int32 StarCount = 0;

    UPROPERTY(SaveGame)
    int32 BestClearTimeMS = 0;
};

/*  int32 BestClearTime 변환 예 1분11초633ms(71633)
int32 TotalMS = 71633;
int32 Minutes = TotalMS / 60000;
int32 Seconds = (TotalMS % 60000) / 1000;
int32 Milli   = TotalMS % 1000;
*/