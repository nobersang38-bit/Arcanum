#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "FBattlePotionSlotData.generated.h"

/**
 * 추영호
 * 전투 시작 전 물약 슬롯 저장 데이터 
 */
USTRUCT(BlueprintType)
struct FBattlePotionSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	FGameplayTag PotionTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Count = 0;
};