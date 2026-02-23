#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "BattleCharacterInfo.generated.h"

/**
 * @brief 개별 장비데이터를 보관하는 구조체
 */
USTRUCT(BlueprintType)
struct FBattleCharacterDef
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    FGameplayTag CharacterTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* CharacterIcon;

    /* Start 다이얼로그 (추후 sound로 변경 가능) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText DefaultDialogue;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText AttackDialogue;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText HitDialogue;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
    FText DeathDialogue;
    /* End 다이얼로그 (추후 sound로 변경 가능) */

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
    FGameplayTag DefaultGrade;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
    FGameplayTag MaxGrade;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
    int32 RequiredShardCount = 10;
};

USTRUCT(BlueprintType)
struct FBattleCharacterInfo {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBattleCharacterDef BattleCharacterInitData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
    int32 CurrShardCount = 0;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
    FGameplayTag CurrGrade;

    /** 성급? 관련 변수, 0이면 미획득 조회시는 +1로 DT에서 찾아야 함.*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
    int32 CurrentGrade = 0;

    /** 얘는 추후 레벨 개념 생기면 넣으려고 미리 생성해놓음*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Growth")
    int32 CurrentLevel = 1;
};