#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SetBonusRow.generated.h"

/*
 * 추영호
 * DT_SetBonus 용 Row
 * - 세트 장비 효과 4셋만 지원
 * - ManaCycle / ItemEffectBonus 두 개만 사용
 */

USTRUCT(BlueprintType)
struct FSetBonusRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 세트 효과 ID (예: Arcanum.Item.SetBonus.ManaCycle) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SetBonusTag;

	/* 이 세트를 구성하는 장비 태그들(4개) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SetItemTag;

	/* 필요 개수 (4셋) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RequiredPieces = 4;

	/* 트리거 수치 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ConditionValue = 0.0f;

	/* 적용 수치 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value = 0.0f;

	/* 내부 쿨타임 (필요 없으면 0) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InternalCooldown = 0.0f;
};

