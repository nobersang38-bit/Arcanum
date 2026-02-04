#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "OptionData.generated.h"

/**
 * 추영호
 * 장비에 실제로 붙은 옵션 하나
 * 이미 확정된 결과 값만 저장
 */
USTRUCT(BlueprintType)
struct FOptionData
{
	GENERATED_BODY()

	/* 능력치 (Arcanum.Stat.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	FGameplayTag StatTag;

	/* 능력치에 실제로 더해질 값 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Option")
	float Value = 0.0f;
};
