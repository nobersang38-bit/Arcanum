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
    FGameplayTag BannerTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MetaData")
    FGameplayTag GachaTypeTag;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MetaData")
    int32 DisplayPriority = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Display")
    FText BannerName;

    /** 배너 이미지 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> BannerImage;

    /** 배너 상세 이미지? */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> DescriptionImage;

    /** 시작 시간 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString StartTime;

    /** 종료 시간 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FCurrencyCost PullCost;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FGachaRarityProbability> RarityProbabilities;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FGachaGradePool> GradePools;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 FiveStarPityCount = 90;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool bGuaranteePickupOnFail = true;
};