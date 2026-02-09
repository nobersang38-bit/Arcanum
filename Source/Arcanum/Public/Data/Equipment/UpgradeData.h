#pragma once

#include "CoreMinimal.h"
#include "UpgradeData.generated.h"

/**
 * 추영호
 * 장비 강화 런타임 데이터
 * 현재 강화 상태만 보관
 */
USTRUCT(BlueprintType)
struct FUpgradeData
{
	GENERATED_BODY()

	/* 현재 강화 레벨 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	int32 UpgradeLevel = 0;
};
