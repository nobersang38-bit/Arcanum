#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "AwakenRow.generated.h"

/**
 * 추영호
 * 각성/패시브 설계용 DataTable Row
 * 캐릭터에게 붙일 각성 상태 디버프/트리거 
 */
USTRUCT(BlueprintType)
struct FAwakenRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 각성 패시브 ID (Arcanum.Ability.Passive.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken")
	FGameplayTag AwakenTag;

	/* 각성 오라 버프 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Aura")
	FGameplayTag AuraStatTag;

	/* 각성 오라 버프 스탯 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Aura")
	float AuraStatValue = 0.0f;

	/* 각성 오라 범위 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Aura")
	float AuraRadius = 0.0f;

	/* 각성 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Aura")
	float AwakenDuration = 0.0f;

	/* 디버프 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Debuff")
	FGameplayTag DebuffTag;

	/* 디버프 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Debuff")
	float DebuffDuration = 0.0f;

	/* 디버프 최대 스택 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Debuff")
	int32 DebuffMaxStack = 0;

	/* 기본 공격 적중 시 게이지 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Gauge")
	float GaugeAttackHit = 0.0f;

	/* 스킬 공격 적중 시 게이지 증가량 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Gauge")
	float GaugeSkillHit = 0.0f;

	/* 연계 트리거 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Awaken|Trigger")
	FGameplayTag TriggerTag;
};


