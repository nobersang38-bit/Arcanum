#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "DataInfo/Stats/Data/FStatModifier.h"
#include "NiagaraSystem.h"
#include "FSkillInfo.generated.h"

USTRUCT(BlueprintType)
struct FWeaponSkillPair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag BasicAttackSkillTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag WeaponSkillTag;
};

USTRUCT(BlueprintType)
struct FLevelModifierEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    int32 Level = 0;
    UPROPERTY(EditAnywhere)
    float CastTime = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logic")
    float Cooldown = 0.f;
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

/* 타겟팅(공통) */
USTRUCT(BlueprintType)
struct FSkillTargetingParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Targeting")
    TSoftObjectPtr<UMaterialInterface> IndicatorDecal;

    /* 사거리(원거리 기본공격/조이스틱 스킬 등) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Targeting")
    float MaxRange = 0.f;

    /* 범위(전방범위/원형범위/블랙홀/메테오 등) - 부채꼴/관통 삭제하고 이거 하나로 통일 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Targeting")
    float Radius = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Targeting")
    float Width = 0.f;
};

/* 넉백/끌어당김(공통) */
USTRUCT(BlueprintType)
struct FSkillCrowdControlParams
{
    GENERATED_BODY()

    /* 밀치기 강도(0이면 미사용) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Control")
    float KnockbackStrength = 0.f;

    /* 끌어당김 강도(0이면 미사용) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Control")
    float PullStrength = 0.f;
};

/* 각성 공통(전부 동일 값이면 여기만 만지면 됨) */
USTRUCT(BlueprintType)
struct FAwakenParams
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Awaken")
    float MaxGauge = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Awaken")
    float GainOnBasicHit = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Awaken")
    float GainOnWeaponSkillHit = 5.f;

    /* 각성 지속(시간) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Awaken")
    float DurationSec = 15.f;

    /* 오라 반경 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Awaken")
    float AuraRadius = 0.f;
};

/* 트리거(각성 디버프 MaxStack + 무기스킬 적중 시 발동) */
USTRUCT(BlueprintType)
struct FSkillTriggerParams
{
    GENERATED_BODY()

    /* 발동할 스킬 태그(혈폭/처단/정화 같은 “트리거 스킬”) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Trigger")
    FGameplayTag TriggerSkillTag;
};

/**
 * @brief 개별 스킬을 보관하는 구조체
 *
 * 이 구조체는 계산 로직을 가지지 않으며,
 * 오직 현재 상태와 기본 정의값만 저장한다.
 */
USTRUCT(BlueprintType)
struct FSkillInfo 
{
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

    /** CurrLevel 0이면 기본 0강(노강) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    int32 CurrLevel = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FLevelModifierEntry> LevelModifiers;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Targeting")
    FSkillTargetingParams Targeting;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Control")
    FSkillCrowdControlParams Control;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Awaken")
    FAwakenParams Awaken;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Trigger")
    FSkillTriggerParams Trigger;

    friend FArchive& operator<<(FArchive& Ar, FSkillInfo& Data)
    {
        Ar << Data.SkillNameTag;
        Ar << Data.DisplayName;
        Ar << Data.CurrLevel;
        return Ar;
    }
};
