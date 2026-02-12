#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/Skills/Data/FSkillInfo.h"
#include "DTSkillsData.generated.h"

USTRUCT(BlueprintType)
struct FDTSkillsDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSkillInfo SkillData;
};