#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NativeGameplayTags.h"
#include "DataInfo/GachaData/Data/FGachaBannerData.h"
#include "DTGachaBannerData.generated.h"


USTRUCT(BlueprintType)
struct FDTGachaBannerDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FGachaRarityProbability> RarityProbabilities;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FGachaGradePool> GradePools;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 FiveStarPityCount = 90;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bGuaranteePickupOnFail = true;
};