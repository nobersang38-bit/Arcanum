#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/BattleCharacter/Equipment/Data/FEquipmentData.h"
#include "DTEquipment.generated.h"

USTRUCT(BlueprintType)
struct FDTEquipmentInfoRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    /** 아이템 식별 태그 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag ItemTag;

    /** 아이템 슬롯(편의상) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag SlotTag;

    /** 아이템 디스크립션 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Desc;

    /** 스켈레톤 메시 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

    /** 장착 몽타주 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UAnimMontage> EquipMontage;

    /** 아이콘, 설명 등 변하지 않는 연출용 데이터 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 BuyPrice;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 SellPrice;

    /* 세트 효과 구조체 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FSetEffectDefinition SetEffect;

    /** 기본 스펙을 담고 있는 구조체 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FItemDefinition> BaseInfoSteps;
};