#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/Skills/Data/FSkillInfo.h"
#include "DTSkillMapping.generated.h"

class USkillBase;

USTRUCT(BlueprintType)
struct FDTSkillMappingRow : public FTableRowBase
{
    GENERATED_BODY()

    /** 스킬 식별 태그 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag SkillTag;

    /** 실행용 스킬 클래스 (BP) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftClassPtr<USkillBase> SkillClass;
};