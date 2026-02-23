#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/StageData/StageInfo/Data/FStageDataInfo.h"
#include "FDTStageDataRow.generated.h"

USTRUCT(BlueprintType)
struct FDTStageDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FStageDataInfo StageData;
};