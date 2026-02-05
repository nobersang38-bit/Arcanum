#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkillData.generated.h"

/**
 * 추영호
 * 스킬 런타임 데이터
 * 게임 실행중 변하는 상태만 보관
 * 설계값(DataTable)은 포함하지 않음
 */
USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	/* Arcanum.Ability.Skill */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FGameplayTag SkillID;

	/* 현재 스킬 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	int32 SkillLevel = 1;

	/* 남은 쿨타임(초) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float CooldownRemaining = 0.0f;
};