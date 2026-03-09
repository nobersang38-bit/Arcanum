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