#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SkillContext.generated.h"

class AActor;

/**
 * 추영호
 * 입력/타켓/위치 등 순간 정보 보관
 * 저장/설계/지속 상태 없음
 */
USTRUCT(BlueprintType)
struct FSkillContext
{
	GENERATED_BODY()

	/* Arcanum.Ability.Skill */
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	FGameplayTag SkillID;

	/* 시전자 */
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	TWeakObjectPtr<AActor> Instigator;

	/* 첫 타겟 (유도/체인/단일 타겟용) */
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	TWeakObjectPtr<AActor> TargetActor;

	/* 타켓 위치 (범위 지정) */
	UPROPERTY(BlueprintReadOnly, Category = "Skill")
	FVector TargetLocation = FVector::ZeroVector;
};
