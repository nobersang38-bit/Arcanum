#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UpgradeRow.generated.h"

/**
 * 추영호
 * 장비 강화 설계용 DataTable Row
 * 강화 레벨별 전체 스탯에 곱해질 배율"만 정의
 * 강화된 스탯 = 원래 뽑힌 스탯 * StatMultiplier
 */
USTRUCT(BlueprintType)
struct FUpgradeRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 강화 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	int32 UpgradeLevel = 0;

	/* 스탯 배율 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	float StatMultiplier = 1.0f;
};