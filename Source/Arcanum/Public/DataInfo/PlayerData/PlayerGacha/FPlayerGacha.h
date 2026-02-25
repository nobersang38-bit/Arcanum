#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "FPlayerGacha.generated.h"

USTRUCT(BlueprintType)
struct FGachaData
{
	GENERATED_BODY()

public:
	/** 천장? 시즌 정산시 마일리지(Soul)로 변환 예정*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> PityCounters;

	/** 무료 뽑기 쿨타임*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FDateTime> LastFreePullTimes;
};