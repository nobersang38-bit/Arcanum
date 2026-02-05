#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnhancementChanceRow.generated.h"

/**
 * 추영호
 * 강화 시도 확률 설계용 DataTable Row
 * 강화 단계별 성공 확률만 관리
 * 강화 레벨 5단까지
 * 확률% : 100 / 50 / 25 / 12 / 6 
 */
USTRUCT(BlueprintType)
struct FEnhancementChanceRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 강화 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhancement")
	int32 UpgradeLevel = 1;

	/* 성공 확률 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhancement")
	float SuccessChance = 100.0f;
};
