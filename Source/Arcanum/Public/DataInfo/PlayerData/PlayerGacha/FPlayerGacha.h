#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "FPlayerGacha.generated.h"

USTRUCT(BlueprintType)
struct FGachaBannerState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 PityCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	bool bPickupGuaranteed = false;
};

USTRUCT(BlueprintType)
struct FGachaData
{
	GENERATED_BODY()

public:
	/** 천장? 시즌 정산시 마일리지(Soul)로 변환 예정*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TMap<FGameplayTag, FGachaBannerState> BannerStates;

	/** 무료 뽑기 쿨타임(구조만 잡아 둠)*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	TMap<FGameplayTag, FDateTime> LastFreePullTimes;
};