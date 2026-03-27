#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "DataInfo/CommonData/Stats/FStatModifier.h"
#include "DTPotionInfoRow.generated.h"

class UNiagaraSystem;

UENUM(BlueprintType)
enum class EPotionEffectType : uint8
{
	Instant		UMETA(DisplayName = "즉시"),
	Duration	UMETA(DisplayName = "지속")
};

UENUM(BlueprintType)
enum class EPotionTooltipValueType : uint8
{
	Flat		UMETA(DisplayName = "고정값"),
	Percent		UMETA(DisplayName = "퍼센트")
};

/**
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

	/* 툴팁 표시 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TooltipStatName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPotionEffectType EffectType = EPotionEffectType::Duration;

	/* 툴팁 값 표시 방식 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPotionTooltipValueType TooltipValueType = EPotionTooltipValueType::Flat;

	/* 포션 즉발 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDerivedStatModifier InstantModifier;


	/* 포션 지속 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDerivedStatModifier> Modifiers;
};
