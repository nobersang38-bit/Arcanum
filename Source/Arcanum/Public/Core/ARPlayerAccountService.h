#pragma once
#include "CoreMinimal.h"
#include "GameplayTags/ArcanumTags.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "DataInfo/PlayerData/FPlayerData.h"

#include "DataInfo/BattleCharacter/Equipment/DataTable/DTEquipment.h"


class UARGameInstance;

/**
 * PlayerAccountService
 * - PlayerData를 수정하는 유일한 진입점
 * - UI 또는 외부 시스템은 PlayerData를 직접 수정하지 않는다.
 * - 추후 해당 로직 및 GameInstance(DB)는 서버로 그대로 옮기면 됨.
 */

class FPlayerAccountService
{
#pragma region PlayerData Getter
public:
    // 플레이어 전체 데이터 카피
    static const FPlayerData GetPlayerDataCopy(const UARGameInstance* GameInstance);

    // 재화
    UFUNCTION(BlueprintCallable)
    const FPlayerCurrency& GetPlayerCurrency(const UARGameInstance* GameInstance) const;
    // 전투 기본 데이터
    UFUNCTION(BlueprintCallable)
    const FPlayerBattleData& GetPlayerBattleData(const UARGameInstance* GameInstance) const;
    // 보유 캐릭터
    UFUNCTION(BlueprintCallable)
    const TArray<FBattleCharacterData>& GetOwnedCharacters(const UARGameInstance* GameInstance) const;
    // 인벤토리
    UFUNCTION(BlueprintCallable)
    const TArray<FEquipmentInfo>& GetInventory(const UARGameInstance* GameInstance) const;
    // 스테이지 진행도
    UFUNCTION(BlueprintCallable)
    const TMap<FGameplayTag, FStageProgressData>& GetStageProgressMap(const UARGameInstance* GameInstance) const;
    // 가챠 상태
    UFUNCTION(BlueprintCallable)
    const FGachaData& GetGachaState(const UARGameInstance* GameInstance) const;
    // 퀘스트 상태
    UFUNCTION(BlueprintCallable)
    const FPlayerQuest& GetQuestState(const UARGameInstance* GameInstance) const;
#pragma endregion



#pragma region Battle Widget 관련
public:

#pragma endregion

#pragma region Character Widget 관련
public:

#pragma endregion

#pragma region Enhancement Widget 관련

#pragma endregion

#pragma region Shop Widget 관련
public:
    /** DT의 장비목록을 반환하는 함수*/
    TArray<FName> GetEquipmentList(UARGameInstance* GameInstance);
    /** 상점에서 구매시 사용하는 함수*/
    static bool PurchaseEquipment(UARGameInstance* GameInstance, FName RowName);
    /** */
    static const FDTEquipmentInfoRow* GetItemDefinition(UGameDataSubsystem* DataSubsystem, const FGameplayTag& ItemTag);
#pragma endregion

#pragma region Gacha Widget 관련
public:

#pragma endregion

};

//static bool PromoteCharacter(FPlayerData& Data, FGameplayTag CharacterID);
//static bool SpendCurrency(FPlayerData& Data, int32 GoldCost);
//static bool RollEquipmentGacha(FPlayerData& Data);

///** 특정 슬롯의 스택을 마일리지(Soul)로 환전하고 스택을 0으로 리셋 */
//static void SettleGachaStackToMileage(FPlayerData& Data, FGameplayTag SlotTag)
//{
//    if (int32* StackPtr = Data.GachaData.PityCounters.Find(SlotTag))
//    {
//        int32 StackCount = *StackPtr;
//        if (StackCount > 0) {
//            AddCurrency(Data, Arcanum::Currency::Soul, StackCount);
//            Data.GachaData.PityCounters.Add(SlotTag, 0);
//        }
//    }
//}

///** 가챠 실행 후 결과를 데이터에 기록. 
//  * bIsPickupHit가 true이면(주인공 뽑음) 스택 리셋, 아니면 +1
//  */
//static void ApplyGachaResult(FPlayerData& Data, FGameplayTag SlotTag, bool bIsPickupHit)
//{
//    int32& CurrentStack = Data.GachaData.PityCounters.FindOrAdd(SlotTag);

//    if (bIsPickupHit || CurrentStack >= 99) CurrentStack = 0;
//    else CurrentStack++;
//}