#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "AwakenRow.generated.h"

/**
 * 추영호
 * 각성 패시브 설계용 DataTable Row
 * 행동/수치 없음
 * 효과/트리거 참조만 보유
 */
USTRUCT(BlueprintType)
struct FAwakenRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 각성 패시브 ID (Arcanum.Ability.Passive.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken")
	FGameplayTag AwakenTag;

	/* 게이지 최대치 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Gauge")
	float GaugeMax = 100.0f;

	/* 적중 시 게이지 획득량 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Gauge")
	float GaugeGainHit = 0.0f;
	 
	/* 오라 효과들 (FEffectRow) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Effect")
	TArray<FName> EffectIDs;

	/* 트리들 (FTriggerEffectRow) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Trigger")
	TArray<FGameplayTag> TriggerTags;
};


