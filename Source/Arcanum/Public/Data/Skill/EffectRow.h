#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "EffectRow.generated.h"

/**
 * 추영호
 * 효과(이펙트) 설계용 DataTable Row
 * 행동/수치 관리
 */
USTRUCT(BlueprintType)
struct FEffectRow : public FTableRowBase
{
	GENERATED_BODY()
	
	/* 효과 타입 (Arcanum.EffectType.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	FGameplayTag EffectTypeTag;

	/* 대상 태그 
	* Buff   : Arcanum.Effect.Buff.*
	* Debuff : Arcanum.Effect.Debuff.*
	* Stat   : Arcanum.Stat.*
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	FGameplayTag EffectTargetTag;

	/* 적용할 타겟 규칙 (Arcanum.TargetRule.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Target")
	FGameplayTag TargetTypeTag;

	/* 대상 필터 (Arcanum.TargetFilter.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Target")
	FGameplayTag TargetFilterTag;

	/* 공용 수치*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Value")
	float Value = 0.0f;

	/* 범위 (0 = 단일) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Area")
	float Radius = 0.0f;

	/* 지속 시간 (0 = 즉시) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Time")
	float Duration = 0.0f;

	/* 최대 스택 (0 = 스택없음) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Stack")
	int32 MaxStack = 0;
	
	/* 틱 간격 (0 = 미사용) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|Tick")
	float Interval = 0.0f;

	/* 실행할 VFX들 (FVfxRow DataTable RowName) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect|VFX")
	TArray<FName> VfxIDs;
};