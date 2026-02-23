#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NativeGameplayTags.h"
#include "DataInfo/BattleCharacter/BattleStats/Data/FGradeStatData.h"
#include "DTBattleStats.generated.h"

USTRUCT(BlueprintType)
struct FDTBattleStatsContainerRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag BattleCharacterTag;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FGradeStatData> GradeDataSteps;
};