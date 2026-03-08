#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "DTEnhanceRuleRow.generated.h"


/**
 * 추영호
 * 장비 강화 단계별 비용/확률 규칙 DT
 */

USTRUCT(BlueprintType)
struct FDTEnhanceRuleRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/* 강화 단계 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 0;

	/* 강화 시 소모 골드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 EnhanceGoldCost = 0;

	/* 리롤 시 소모 소울 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 RerollSoulCost = 0;

	/* 분해 시 획득 소울 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 DisassembleSoulReward = 0;

	/* 강화 성공 확률(%) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnhanceSuccessRate = 100;
};
