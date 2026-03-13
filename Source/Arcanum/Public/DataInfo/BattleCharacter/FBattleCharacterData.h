#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "DataInfo/BattleCharacter/CharacterInfo/Data/BattleCharacterInfo.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "FBattleCharacterData.generated.h"

/**
 * @brief   캐릭터 한마리 데이터
 *          게임인스턴스(서버)에 저장할 데이터 모음?
 *          캐릭터 하나당 하나씩 필요
 */
USTRUCT(BlueprintType)
struct FBattleCharacterData
{
    GENERATED_BODY()

    // --- 기본 고유 정보 ---
    /** 플레이어 가능한 캐릭터 중 선택이 되었는가? => 스테이지에서 조정 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSelection = false;
    /** 캐릭터 식별용 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag Character;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBattleCharacterInfo CharacterInfo;

    // --- 장비 데이터(캐릭터가 현재 장착하고 있는) ---
    /// 260312 변경 (변수명 변경 => WeaponSlots)
    /// 용례 <Arcanum.Items.ItemSlot.Weapon.Slot1, Guid>를 통해서 get/set 하시면 됩니다.
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, FGuid> WeaponSlots;


    /// 260312 변경 (변수 추가)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, FGuid> LegendaryWeaponSlots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, FGuid> ArmorSlots;
};