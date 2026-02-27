#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTags/ArcanumTags.h"

#include "DataInfo/BattleCharacter/FBattleCharacterData.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "Core/ArcanumSaveGame.h"
#include "Core/ARPlayerAccountService.h"
#include "ARGameInstance.generated.h"

/* 재화 변경 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrencyChanged); //

/*
 * Version : 1.0.0.0 2026/02/03
 * 클래스 역할 :
 *  1. 서버가 없으므로, 서버 대용
 *  2. 모든 플레이어 데이터는 여기
 */

UCLASS()
class ARCANUM_API UARGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
#pragma region 블루플린트에서 변경 가능한 애들
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00-Global Setting")
    float SupplyRegenTimer = 300.f;
#pragma endregion
	
#pragma region 초기화 관련
public:
    virtual void Init() override;
    /** 처음 플레이시 필요한 게임 데이터 초기화*/
    void InitializeGameData();
private:
    /** 플레이어 관련 초기화*/
    void InitializeNewPlayerData();
#pragma endregion

#pragma region ID/PW용
public:
    bool AddIDPW(FString ID, FString PW);
    bool CheckLogin(FString ID, FString PW);
#pragma endregion

#pragma region 플레이어 데이터 저장/로드
public:
    /** 플레이어 데이터 저장*/
    bool SavePlayerData();
    /** 플레이어 데이터 로드*/
    bool LoadPlayerData();

    friend class FPlayerAccountService;
private:
    /** 해당 태그 재화 있는지 가져오는 게터*/
    int64 GetCurrencyAmount(FGameplayTag InTag) const;
    /** 플레이어 재화 변경시 */
    void AddCurrency(FGameplayTag CurrencyValueTag, int64 Amount);
    /** 치트 발견시 기존 세이브 데이터 삭제 */
    bool DeletePlayerData();
    const FPlayerData& GetPlayerDataCopy() const { return PlayerData; }
    /* 필요하면 Getter만 제공 */
    FPlayerData& GetPlayerData() { return PlayerData; }
    /** 저장할 본체*/
    UPROPERTY() FPlayerData PlayerData;
#pragma endregion


#pragma region 비저장
public:
    UPROPERTY(Transient)
    FGameplayTag CurrentStageTag;
#pragma endregion


#pragma region 고정
protected:
    /** 데이터 저장용 클래스*/
    UPROPERTY() TObjectPtr<UArcanumSaveGame> ArSaveGame;
    // 저장 파일 이름 고정
    const FString SaveSlotName = TEXT("MainSaveSlot");
#pragma endregion




public:
    // 서버가 관리하는 전체 캐릭터 데이터 리스트(안쓸예정/PlayerData 안으로 들어감)
    UPROPERTY()
    TMap<FGameplayTag, FBattleCharacterData> UserCharacterRegistry;

    void InitializeCharacter(FGameplayTag CharacterTag);

#pragma region 재화 변경 알림
public:
    UPROPERTY(BlueprintAssignable)
    FOnCurrencyChanged OnCurrencyChanged; // 델리게이트
#pragma endregion

#pragma region 상점 세이브 접근
public:
    UFUNCTION(BlueprintCallable)
    UArcanumSaveGame* GetArSaveGame() const { return ArSaveGame; } // 상점 세이브
#pragma endregion

#pragma region 테스트 코드
    UFUNCTION(BlueprintCallable)
    bool TestPurchaseEquipment(FName ItemRowName)
    {
        bool bSuccess = FPlayerAccountService::PurchaseEquipment(this, ItemRowName);

        if (bSuccess)
        {
            SavePlayerData();
            UE_LOG(LogTemp, Warning, TEXT("Purchase Success & Saved"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Purchase Failed"));
        }

        return bSuccess;
    }
#pragma endregion
};
