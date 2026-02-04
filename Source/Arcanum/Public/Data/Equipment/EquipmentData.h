#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/Equipment/UpgradeData.h"
#include "Data/Option/OptionData.h"
#include "EquipmentData.generated.h"

/**
 * 추영호
 * 장비 인스턴스 데이터
 * 장비 뽑기 결과물 하나
 */
USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

	/* 장비 (Arcanum.Item.Equipment.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment")
	FGameplayTag EquipmentTag;

	/* 현재 강화 상태 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Upgrade")
	FUpgradeData Upgrade;

	/* 현재 옵션 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipment|Option")
	TArray<FOptionData> Options;
};
