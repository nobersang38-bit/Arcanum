#pragma once

#include "CoreMinimal.h"
#include "DataInfo/PlayerData/PlayerCurrency/FPlayerCurrency.h"
#include "DataInfo/PlayerData/PlayerBattleData/Data/FPlayerBattleData.h"

#include "DataInfo/BattleCharacter/FBattleCharacterData.h"
#include "Data/Types/UnitData.h"

#include "DataInfo/BattleCharacter/Equipment/Data/FEquipmentData.h"
#include "DataInfo/BattleCharacter/BattleStats/DataTable/DTBattleStats.h"

#include "DataInfo/StageData/FStageData.h"

#include "DataInfo/PlayerData/PlayerGacha/FPlayerGacha.h"
#include "DataInfo/PlayerData/PlayerQuest/FPlayerQuest.h"
#include "FPlayerData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerData
{
    GENERATED_BODY()

    /* ==============================
     *        재화 관련
     * ============================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FPlayerCurrency PlayerCurrency;

    /* ==============================
     *   스테이지 진입 시 기본 값
     * ============================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FPlayerBattleData PlayerBattleData;

    /* ==============================
     *     플레이어 보유 캐릭터
     * ============================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    TArray<FBattleCharacterData> OwnedCharacters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    TArray<FUnitInfoSetting> AllyburdenCharacters;

    /* ==============================
     *         인벤토리
     * ============================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    TArray<FEquipmentInfo> Inventory;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    TMap<FGameplayTag, int32> StackCounts;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    int32 InventoryCapacity = 30;

    /* ==============================
     *         메일박스(캐릭터는 처음부터 다 주어지므로 별도 메일박스가 필요 없음)
     * ============================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    TArray<FMailItem> Mailbox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    int32 MailboxCapacity = 100;

    /* ==============================
     *     스테이지 클리어 정보?
     * ============================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    TMap<FGameplayTag, FStageProgressData> StageProgressMap;


    /* ==============================
     *     가챠 천장/카운터용
     * ============================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FGachaData GachaState;

    /* ==============================
     * 퀘스트 진행도용 (빈 구조체라도 파놓음)
     * ============================== */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    FPlayerQuest QuestState;

    bool operator==(const FPlayerData& Other) const
    {
        if (!(PlayerCurrency == Other.PlayerCurrency)) return false;

        return true;
    }
};
