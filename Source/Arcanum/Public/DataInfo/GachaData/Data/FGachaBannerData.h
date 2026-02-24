#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "FGachaBannerData.generated.h"

USTRUCT(BlueprintType)
struct FGachaRarityProbability
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag GradeTag;

    /** 확률(0.0 ~ 1.0)*/
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Probability;
};

USTRUCT(BlueprintType)
struct FGachaGradePool
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag GradeTag;

    // 이번 배너 주인공 (직접 입력)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FGameplayTag> PickupCharacters;

    // 미리 만들어둔 아이템/캐릭터 마스터 JSON 테이블 연결
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UDataTable> CommonPoolTable;

    // 픽업 vs 상시 비율 (0.5면 50:50)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float PickupRatio = 0.5f;
};

//USTRUCT(BlueprintType)
//struct FGachaBannerData
//{
//    GENERATED_BODY()
//    
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    FText BannerName;
//
//    /** 레어도 등장 확률 */
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    TArray<FGachaRarityProbability> RarityProbabilities;
//
//    /** 레어도별 풀 */
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    TArray<FGachaGradePool> GradePools;
//
//    /** 5성 천장 */
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    int32 FiveStarPityCount = 90;
//
//    /** 픽업 실패 시 다음 5성 확정 */
//    UPROPERTY(EditAnywhere, BlueprintReadOnly)
//    bool bGuaranteePickupOnFail = true;
//};
