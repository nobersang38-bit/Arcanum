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
	
public:
    virtual void Init() override;
protected:
    UPROPERTY()
    TObjectPtr<UArcanumSaveGame> ArSaveGame;

    // 저장 파일 이름 고정
    const FString SaveSlotName = TEXT("MainSaveSlot");

#pragma region ID/PW용
public:
    bool AddIDPW(FString ID, FString PW);
    bool CheckLogin(FString ID, FString PW);
    void SavePlayerData();
#pragma endregion

public:
    UFUNCTION(BlueprintCallable)
    const FPlayerData& GetPlayerDataCopy() const { return PlayerData; }

    /* 필요하면 Getter만 제공 */
    UFUNCTION(BlueprintCallable)
    FPlayerData& GetPlayerData() { return PlayerData; }
private:
    UPROPERTY() FPlayerData PlayerData;

public:
    // 서버가 관리하는 전체 캐릭터 데이터 리스트(안쓸예정/PlayerData 안으로 들어감)
    UPROPERTY()
    TMap<FGameplayTag, FBattleCharacterData> UserCharacterRegistry;

    void InitializeCharacter(FGameplayTag CharacterTag);
    void SaveAllData();
    void LoadAllData();

#pragma region 재화 변경 알림
public:
    UPROPERTY(BlueprintAssignable)
    FOnCurrencyChanged OnCurrencyChanged; // 델리게이트
#pragma endregion

#pragma region 런타임 상점 상태
public:
    /* 상점 다음 갱신 시각 */
    UPROPERTY()
    FDateTime NextShopRefreshTime;

    /* 현재 상점 슬롯 아이템 RowName 목록 */
    UPROPERTY()
    TArray<FName> CurrentShopRowNames;

    /* 현재 상점 슬롯 품절 여부 목록 */
    UPROPERTY()
    TArray<bool> CurrentShopSoldOutStates;
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

    UFUNCTION(BlueprintCallable)
    void AddCurrency(FGameplayTag CurrencyValueTag, int64 Amount)
    {
        if (!CurrencyValueTag.IsValid() || Amount == 0) return;

        FCurrencyData& CurrencyData = PlayerData.PlayerCurrency.CurrencyDatas.FindOrAdd(CurrencyValueTag);

        CurrencyData.CurrAmount += Amount;

        if (CurrencyData.MaxAmount > 0) {
            CurrencyData.CurrAmount = FMath::Clamp(CurrencyData.CurrAmount, int64(0), CurrencyData.MaxAmount);
        }

        if (Amount > 0) {
            CurrencyData.TotalEarned += Amount;

            OnCurrencyChanged.Broadcast();
        }
    }
#pragma endregion
};
