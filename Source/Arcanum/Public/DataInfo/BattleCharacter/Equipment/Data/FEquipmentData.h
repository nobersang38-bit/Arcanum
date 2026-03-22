#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "DataInfo/CommonData/Stats/FStatModifier.h"
#include "FEquipmentData.generated.h"

USTRUCT(BlueprintType)
struct FStatRangeDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag StatTag;

    /** 가챠 시 결정될 최소 수치 (Flat/Mul) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FExternalStatModifier MinValue;

    /** 가챠 시 결정될 최대 수치 (Flat/Mul) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FExternalStatModifier MaxValue;
};

/**
 * 세트 아이템 구조체
 */
USTRUCT(BlueprintType)
struct FSetEffectDefinition
{
    GENERATED_BODY()

public:
    /* 세트 이름 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SetNameText;

    /* 세트 효과 발동 필요 개수 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NeedCount = 4;

    /* 세트 효과 미발동 설명 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText InactiveDescText;

    /* 세트 효과 발동 설명 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ActiveDescText;

    /* 세트 보너스 스킬 태그 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag SetBonusTag;
};

/**
 * @brief 개별 장비데이터를 보관하는 구조체
 */
USTRUCT(BlueprintType)
struct FItemDefinition
{
    GENERATED_BODY()

    /** 장착자에게 적용되는 스탯 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDerivedStatModifier> OwnerStats;

    /** 적에게 적용되는 스탯 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDerivedStatModifier> OnHitTargetStats;

    /** 가챠 범위 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FStatRangeDefinition> RandomStatRanges;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxUpgradeLevel = 5;

    /* 기본 공격 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag BasicAttackSkillTag;

    /** 태그, 레벨(이거 어짜피 FEquipmentInfo 복사뜨기 때문에 관리포인트 만들기 싫어서 여기다가 그냥 넣어놓음) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, int32> Skills;
};


USTRUCT(BlueprintType)
struct FEquipmentInfo {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag ItemTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid ItemGuid;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FItemDefinition Equipment;

    /** 현재 강화 수치 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrUpgradeLevel = 0;
};

/**
 * @brief 메일박스용 구조체
 */
USTRUCT(BlueprintType)
struct FMailItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FEquipmentInfo Equipment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FDateTime ExpireTime;
};