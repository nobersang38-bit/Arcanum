#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/Skills/Data/FSkillInfo.h"
#include "DTSkillsData.generated.h"

class USkillBase;

USTRUCT(BlueprintType)
struct FDTSkillsDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSkillInfo SkillData;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
    TSubclassOf<USkillBase> SkillClass;
};

/*  무기 - 스킬 매핑 데이터 행 */
USTRUCT(BlueprintType)
struct FDTWeaponSkillMappingRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag WeaponTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FWeaponSkillPair SkillPair;
};