#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "TriggerEffectRow.generated.h"

/**
 * 추영호
 * 트리거 효과 설계용 DataTable Row
 * 조건/스택은 FEffectRow에서 처리
 */
USTRUCT(BlueprintType)
struct FTriggerEffectRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 트리거 ID	 (Arcanum.Ability.Event.Trigger.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger")
	FGameplayTag TriggerTag;

	/* 발동 조건 디버프 (이 디버프가 최대 스택이면 트리거 발동) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Condition")
	FGameplayTag ConditionDebuffTag;

	/* 실행할 효과들 (FEffectRow DataTable의 RowName들) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger|Effect")
	TArray<FName> EffectIDs;
};