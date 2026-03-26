#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "DataInfo/CommonData/Stats/FStatModifier.h"
#include "DTPotionInfoRow.generated.h"

class UNiagaraSystem;

/**
 * 추영호
 * 포션 테이블
 */

USTRUCT(BlueprintType)
struct FDTPotionInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	/* 포션 식별 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PotionTag;
	
	/* 물약 쿨타임 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownSeconds = 0.f;

	/* 먹는 몽타주 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> UseMontage;

	/* 사용 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> UseNiagara;

	/* 사용 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> UseSound;

	/* 포션 효과(스탯 기반) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDerivedStatModifier> Modifiers;
};
