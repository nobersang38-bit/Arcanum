#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "DataInfo/CommonData/Stats/FStatModifier.h"
#include "NiagaraSystem.h"
#include "FSkillInfo.generated.h"

class ASkillActor;

USTRUCT(BlueprintType)
struct FLevelModifierEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 Level;
    UPROPERTY(EditAnywhere)
    float CastTime = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    float Cooldown;
    UPROPERTY(EditAnywhere)
    TArray<FDerivedStatModifier> Cost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraSystem> Effect;


    /** 스킬이 맞은 상대에게 적용될 StatModifier */
    UPROPERTY(EditAnywhere)
    TArray<FDerivedStatModifier> OtherCharacterModifiers;

    /** 상대에게 보여줄 이펙트 */
    UPROPERTY(EditAnywhere)
    TObjectPtr<UNiagaraSystem> TargetEffect;
};

/**
 * @brief 개별 스킬을 보관하는 구조체
 *
 * 이 구조체는 계산 로직을 가지지 않으며,
 * 오직 현재 상태와 기본 정의값만 저장한다.
 */
USTRUCT(BlueprintType)
struct FSkillInfo {
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag SkillNameTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag SkillTypeTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag TargetTypeTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag TargetFilterTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FGameplayTag> SkillVFXTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float EnabledRange = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<class UAnimMontage> SkillMontage = nullptr;

    /** 실행용 스킬 클래스 (BP) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<ASkillActor> SkillClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FLevelModifierEntry> LevelModifiers;

    friend FArchive& operator<<(FArchive& Ar, FSkillInfo& Data)
    {
        Ar << Data.SkillNameTag;
        Ar << Data.DisplayName;
        return Ar;
    }
};

USTRUCT(BlueprintType)
struct FSkillState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag SkillNameTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Level;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CooldownRemaining;
};