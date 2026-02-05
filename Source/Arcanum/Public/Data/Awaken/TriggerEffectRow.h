#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "TriggerEffectRow.generated.h"

/**
 * 추영호
 * 트리거 효과 설계용 DataTable Row
 * 조건/스택은 FAwakenRow에서 처리
 */
USTRUCT(BlueprintType)
struct FTriggerEffectRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 트리거 ID	 (Arcanum.Ability.Event.Trigger.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	FGameplayTag TriggerTag;

	/* 추가 피해 계수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Damage")
	float DamageMultiplier = 0.0f;

	/* 범위 (0 이면 단일) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Area")
	float Radius = 0.0f;

	/* 연쇄 횟수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Chain")
	int32 ChainCount = 0;

	/* 연쇄 감소율 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Chain")
	float ChainFalloff = 1.0f;

	/* 추가 디버프 (스턴용) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Extra")
	FGameplayTag ExtraDebuffTag;

	/* 추가 디버프 지속 시간 (스턴용) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Extra")
	float ExtraDebuffDuration = 0.0f;
};