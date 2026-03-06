#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTags/ArcanumTags.h"

#include "DataInfo/BattleCharacter/FBattleCharacterData.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "Core/ArcanumSaveGame.h"
#include "Core/ARPlayerAccountService.h"
#include "ARGameInstance.generated.h"

/**특정 값(Amount 등) 반환 시 에러를 나타내기 위한 상수 */
#define INDEX_NONE_LONG -1LL

/** 게임 내 다양한 처리 결과를 나타내는 상태 코드 */
UENUM(BlueprintType)
enum class EGameErrCode : uint8
{
    Success             UMETA(DisplayName = "Success"),             // 성공
    InvalidTag          UMETA(DisplayName = "Invalid Tag"),         // 유효하지 않은 태그 (InTag.IsValid() 실패)
    NotFound            UMETA(DisplayName = "Data Not Found"),      // 데이터를 찾을 수 없음
    InsufficientBalance UMETA(DisplayName = "Insufficient Balance"),// 잔액 부족
    UnknownError        UMETA(DisplayName = "Unknown Error")        // 기타/알 수 없는 에러
};

UENUM(BlueprintType)
enum class EHUDIndex : uint8
{
    BattleMenu = 0,
    CharacterMenu = 1,
    EnhancementMenu = 2,
    ShopMenu = 3,
    GachaMenu = 4,
};

USTRUCT(BlueprintType)
struct FGachaItemResult
{
    GENERATED_BODY()

    // 1. 뽑힌 대상의 식별자 (캐릭터 태그, 아이템 태그 등)
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag ItemTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag GradeTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UDataTable> SourceTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Quantity = 1;

    FGachaItemResult() {}
    FGachaItemResult(FGameplayTag InTag, UDataTable* InTable, int32 InQty = 1)
        : ItemTag(InTag), SourceTable(InTable), Quantity(InQty) {
    }
};

USTRUCT(BlueprintType)
struct FShopProductKey
{
    GENERATED_BODY()

    UPROPERTY()
    FGameplayTag TableTag;

    UPROPERTY()
    FName RowName = NAME_None;
};

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

#pragma region 가챠 관련
private:
    bool GenerateResults(const FDTGachaBannerDataRow* BannerData, int32 PullCount);
    FGameplayTag DetermineGrade(const FDTGachaBannerDataRow* BannerData);
    bool ApplyHardPity(const FDTGachaBannerDataRow* BannerData, FGachaBannerState& BannerState, FGameplayTag& InOutGrade);
    FGameplayTag GetHighestGrade(const FDTGachaBannerDataRow* BannerData);
    FGachaItemResult ResolvePickup(const FDTGachaBannerDataRow* BannerData, const FGachaGradePool& Pool, FGachaBannerState& BannerState, FGameplayTag GachaIndex);
    FGameplayTag GetRandomFromGrade(const FGachaGradePool& Pool, FGameplayTag GachaIndex);
    void AddRandomEquipmentToInventory(FDTEquipmentInfoRow* InRow);
#pragma endregion


#pragma region 고정
protected:
    /** 데이터 저장용 클래스*/
    UPROPERTY() TObjectPtr<UArcanumSaveGame> ArSaveGame;
    // 저장 파일 이름 고정
    const FString SaveSlotName = TEXT("MainSaveSlot");
#pragma endregion

#pragma region 저장 안하는 놈들
public:
    UPROPERTY(Transient) TSoftObjectPtr<UWorld> PendingStageLevel;
    UPROPERTY(Transient) FGameplayTag CurrentStageTag;
private:
    UPROPERTY(Transient) EHUDIndex HUDIndex = EHUDIndex::CharacterMenu;
    UPROPERTY(Transient) TArray<FGachaItemResult> GachaItemResult;
#pragma endregion



public:
    // 서버가 관리하는 전체 캐릭터 데이터 리스트(안쓸예정/PlayerData 안으로 들어감)
    UPROPERTY()
    TMap<FGameplayTag, FBattleCharacterData> UserCharacterRegistry;

    void InitializeCharacter(FGameplayTag CharacterTag);

#pragma region 재화 변경 알림
public:
    UPROPERTY(BlueprintAssignable)
    FOnCurrencyChanged OnCurrencyChanged;
#pragma endregion

#pragma region 런타임 상점 상태
public:
    /* 상점 다음 갱신 시각 */
    UPROPERTY(Transient)
    FDateTime NextShopRefreshTime;

    /* 현재 상점 슬롯 상품 키 (TableTag + RowName) */
    UPROPERTY(Transient)
    TArray<FShopProductKey> CurrentShopKeys;

    /* 현재 상점 슬롯 품절 여부 목록 */
    UPROPERTY(Transient)
    TArray<bool> CurrentShopSoldOutStates;

    /* 전투 진입 타이머 정지했는지 */
    UPROPERTY(Transient)
    bool bShopPaused = false;

    /* 정지 순간 남은 초 */
    UPROPERTY(Transient)
    int32 PausedShopRemainingSeconds = 0;
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
