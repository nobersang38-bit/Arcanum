#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SkillRow.generated.h"

/**
 * 추영호
 * 스킬 설계용 DataTable Row
 * 행동/수치 없음
 * 실행할 Effect들만 참조
 */
USTRUCT(BlueprintType)
struct FSkillRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 스킬 ID (Arcanum.Ability.Skill.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FGameplayTag SkillID;

	/* 쿨타임 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float Cooldown = 0.0f;

	/* 마나 소모 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float ManaCost = 0.0f;

	/* 실행할 효과들 (FEffectRow DataTable RowName) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Effect")
	TArray<FName> EffectIDs;

	/* 패턴 파라미터 1 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Pattern")
	int32 PatternValue1 = 0;

	/* 패턴 파라미터 2 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Pattern")
	int32 PatternValue2 = 0;
};