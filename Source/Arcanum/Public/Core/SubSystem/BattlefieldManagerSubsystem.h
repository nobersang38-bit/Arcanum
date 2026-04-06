// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/Types/CombatStageData.h"
#include "Data/Types/MatchData.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "Data/DataAssets/EnemyWaveData.h"
#include "GameplayTags/ArcanumTags.h"
#include "NativeGameplayTags.h"
#include "Data/Types/UnitData.h"
#include "Data/Types/BattleStageInfo.h"
#include "DataInfo/StageData/StageInfo/Data/FStageDataInfo.h"
#include "DataInfo/SkillData/Data/FBattleWeaponSkillData.h"
#include "DataInfo/SkillData/Data/FSkillInfo.h"
#include "Data/Rows/DTBattleStageInfo.h"
#include "BattlefieldManagerSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FInBattleData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FPlayerData PlayerData;
	UPROPERTY()
	FStageDataInfo StageData;
	UPROPERTY()
	FBattleStageInfo BattleStageInfo;
	UPROPERTY()
	FGradeStatData PlayerBattleStat;
	UPROPERTY()
	FBattleWeaponSkillData BattleWeaponSkill;
	UPROPERTY()
	FPlayerBattleData PlayerBattleData;
	UPROPERTY()
	TArray<FDerivedStatModifier> EquippedOwnerStats;

	TMap<FGameplayTag, TMap<FGameplayTag, FDerivedStatModifier>> WeaponOnHitTarget;
};


DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchEnded, const FMatchData&)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthBarProgress, float, InCurrent, float, InMax);

// 김도현
// 전투 스테이지에 관련된 기능
// Todo : 나중에 스폰관련도 넣어야함
class ACharacter;
class ABattlefieldManagerActor;
UCLASS()
class ARCANUM_API UBattlefieldManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

public:
	// 전투 스테이지가 종료되면 호출, 결과 정보 = FMatchResultData
	FOnMatchEnded OnMatchEnded;

	bool GetIsDebugMode() const { return bIsDebug; }
	void SetIsDebugMode(bool InUseDebug) { bIsDebug = InUseDebug; }

#pragma region 체력관련 프로그래스바
public:
	FOnHealthBarProgress OnChangeEnemyBaseHealth;
	FOnHealthBarProgress OnChangeAllyBaseHealth;
#pragma endregion

#pragma region 스테이지 기본설정
public:
	UFUNCTION()
	AActor* GetAllyBasement() const;

	UFUNCTION()
	AActor* GetEnemyBasement() const;

	UFUNCTION()
	class ASpawnCheckDecal* GetSpawnCheckDecal() const;

	UFUNCTION()
	void SetSpawnCheckDecal(class ASpawnCheckDecal* InCheckDecal);

	const FEnemyBasement& GetAllyBasementStat() const;
	const FEnemyBasement& GetEnemyBasementStat() const;

	UFUNCTION()
	void AddAllyBasement(AActor* InNexus);

	UFUNCTION()
	void AddEnemyBasement(AActor* InNexus);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ABattlefieldManagerActor* GetBattlefieldManagerActor() { return BattlefieldManagerActor.Get(); }

	UFUNCTION()
	void SetABattlefieldManagerActor(ABattlefieldManagerActor* InBattlefieldManagerActor);

	UFUNCTION()
	const TMap<FGameplayTag, FUnitInfoSetting>& GetUsingAllyUnitData();

	UFUNCTION()
	FUnitInfoSetting GetAllyUnitData(FGameplayTag InUnitTag, bool& OutResult) const;
	UFUNCTION()
	FUnitInfoSetting GetEnemyUnitData(FGameplayTag InUnitTag, bool& OutResult) const;

	UFUNCTION()
	const FStageDataInfo& GetStageData() const { return InBattleData.StageData; }

	UFUNCTION()
	void StartTime();

	UFUNCTION()
	void StopTime();

	UFUNCTION()
	void OnTimeChange(int32 Time);


#pragma endregion

#pragma region 디버그
	void DebugSetUsingAllyUnits();
#pragma endregion


#pragma region 중요정보 및 설정

	// 현재 스테이지 플레이 정보(진행중인지, 종료했는지 종료됐다면 승리했는지)
	FORCEINLINE const FMatchData& GetCurrentMatchData() { return CurrentMatchData; }

	FORCEINLINE const FInBattleData& GetInBattleData() const { return InBattleData; }
	void SetInBattleData(const FPlayerData& InPlayerData, FInBattleData& OutInBattleData);

	UFUNCTION()
	void MatchEnded(const FMatchData& MatchData);
	/*
	필요한 정보
	유닛들
	플레이어 캐릭터
	현재 스테이지
	*/
#pragma endregion



protected:
	UFUNCTION()
	void CheckMatchEnded(int32 Time);

#pragma region 디버그
	void SetupUnits();
#pragma endregion


protected:
#pragma region 스테이지 기본설정
	FMatchData CurrentMatchData;

	UPROPERTY()
	TWeakObjectPtr<ABattlefieldManagerActor> BattlefieldManagerActor = nullptr;
#pragma endregion

#pragma region 데이터 캐시
	UPROPERTY()
	FInBattleData InBattleData;

	UPROPERTY()
	TWeakObjectPtr<AActor> AllyBasement;

	UPROPERTY()
	TWeakObjectPtr<AActor> EnemyBasement;

	UPROPERTY()
	TWeakObjectPtr<class ASpawnCheckDecal> CachedSpawnCheckDecal = nullptr;

	UPROPERTY()
	FEnemyBasement AllyBasementStat;

	UPROPERTY()
	FEnemyBasement EnemyBasementStat;

	UPROPERTY()
	TMap<FGameplayTag, FUnitInfoSetting> AllyUnitDatas;

	UPROPERTY()
	TMap<FGameplayTag, FUnitInfoSetting> EnemyUnitDatas;

#pragma endregion

private:
	bool bIsDebug = false;

protected:
#pragma region 디버그(나중에 삭제)
	void DebugBasementSet();
	UFUNCTION()
	void DebugEndedMessage(const FMatchData& MatchData);

	UFUNCTION()
	void DebugPlayerCharacterSet();
#pragma endregion

public:
	const FGameplayTag AllyTeamTag = Arcanum::Unit::Faction::Ally::Root;
	const FGameplayTag EnemyTeamTag = Arcanum::Unit::Faction::Enemy::Root;
	const FGameplayTag AllyBaseStaminaTag = Arcanum::BattleStat::Player::NonRegen::CommandCenterStamina::Value;

#pragma region 스킬 장비 캐시
public:
	/* 현재 무기 장착 몽타주 */
	UAnimMontage* GetCurrentWeaponEquipMontage() const;

	/* 현재 무기 슬롯 태그 */
	FGameplayTag GetCurrentWeaponSlotTag() const;

	/* 현재 무기 기본 공격 스킬 태그 */
	FGameplayTag GetCurrentBasicAttackSkillTag() const;

	/* 현재 무기 기본 공격 스킬 레벨 */
	int32 GetCurrentBasicAttackSkillLevel() const;

	/* 현재 무기 기본 스킬 태그 */
	FGameplayTag GetCurrentBasicSkillTag() const;

	/* 현재 무기 기본 스킬 레벨 */
	int32 GetCurrentBasicSkillLevel() const;

	/* 전설 무기 궁극기 스킬 태그 */
	FGameplayTag GetLegendaryUltimateSkillTag() const;

	/* 전설 무기 궁극기 스킬 레벨 */
	int32 GetLegendaryUltimateSkillLevel() const;

	/* 현재 활성 무기 아이콘 */
	UTexture2D* GetCurrentWeaponIcon() const;

	/* 전설 무기 아이콘 */
	UTexture2D* GetLegendaryWeaponIcon() const;

	/* 일반 스킬 아이콘 */
	UTexture2D* GetCurrentBasicSkillIcon() const;

	/* 전설 스킬 아이콘 */
	UTexture2D* GetLegendaryUltimateSkillIcon() const;

	/* 현재 선택 캐릭터의 4세트 발동 스킬 태그 반환 */
	UFUNCTION()
	FGameplayTag GetEquippedSetBonusTag() const;

	/* 현재 활성 무기 슬롯 태그 (스왑) */
	void SetCurrentWeaponSlotTag(const FGameplayTag& InWeaponSlotTag);

	/* 궁극기 사용 시작 */
	void BeginLegendaryWeaponMode();

	/* 궁극기 종료 후 이전 무기로 복귀 */
	void EndLegendaryWeaponMode();

	/* 궁극기 활성화 여부*/
	bool IsLengendaryWeapon() const { return bUsingLegendaryWeapon; }

	/* 현재 활성 무기 스켈레탈 메시 */
	USkeletalMesh* GetCurrentWeaponMesh() const;

	/* 전설 무기 스켈레탈 메시 */
	USkeletalMesh* GetLegendaryWeaponMesh() const;

	/* 현재 활성 무기의 장착 타입 태그 */
	FGameplayTag GetCurrentWeaponSlotTypeTag() const;

	/* 현재 기본공격 스킬 정보*/
	const FSkillInfo* GetCurrentBasicAttackSkillInfo() const;

	/* 현재 스킬 정보*/
	const FSkillInfo* GetCurrentBasicSkillInfo() const;

	/* 현재 기본공격 스킬 캐시 Get */
	const FBattleSkillData* GetCurrentBasicAttackSkillData() const;

	/* 현재 일반스킬 캐시 Get */
	const FBattleSkillData* GetCurrentBasicSkillData() const;

	/* 현재 전설스킬 캐시 Get */
	const FBattleSkillData* GetCurrentLegendarySkillData() const;

	/* 현재 선택된 플레이어 캐릭터 반환 */
	const FBattleCharacterData* GetSelectedCharacterData() const;

	/* 쿨타임 */
	float GetCurrentBasicAttackCooldown() const;
	float GetCurrentBasicSkillCooldown() const;
	float GetLegendaryUltimateCooldown() const;

	/* 스킬 태그로 스킬 정보 */
	const FSkillInfo* FindSkillInfoByTag(const FGameplayTag& InSkillTag) const;

protected:
	/* 장착 방어구 OwnerStats 데이터 */
	void EquippedArmorOwnerStats(
		const FPlayerData& InPlayerData,
		const FBattleCharacterData& InSelectedCharacter,
		TArray<FDerivedStatModifier>& OutEquippedOwnerStats) const;

	/* 장착 무기 OnHitTargetStats 데이터 */
	void EquippedWeaponOnHitTarget(
		const FPlayerData& InPlayerData,
		const FBattleCharacterData& InSelectedCharacter,
		TMap<FGameplayTag, TMap<FGameplayTag, FDerivedStatModifier>>& OutWeaponOnHitTarget) const;

	/* 스킬 캐스트타임 */
	float FindSkillCastTime(const FGameplayTag& InSkillTag, int32 InSkillLevel) const;

	/* 스킬 쿨타임 */
	float FindSkillCooldown(const FGameplayTag& InSkillTag, int32 InSkillLevel) const;

	/* 스킬 마나 */
	float FindSkillManaCost(const FGameplayTag& InSkillTag, int32 InSkillLevel) const;

	/* 스킬 아이콘 */
	UTexture2D* FindSkillIcon(const FGameplayTag& InSkillTag) const;

	/* 전투 시작 시 무기 스킬 캐시 생성 */
	void BuildBattleWeaponSkillCache(FInBattleData& OutInBattleData);

	/* Guid로 인벤토리에서 장비 찾기 */
	const FEquipmentInfo* FindEquipmentByGuid(const FPlayerData& InPlayerData, const FGuid& InItemGuid) const;

	/* 현재 선택 캐릭터가 지정한 세트 루트 태그를 4개 장착했는지 확인 */
	UFUNCTION()
	bool HasEquippedFullSet(const FGameplayTag& InSetRootTag) const;
protected:
	/* 궁극기 사용 이전 무기 슬롯 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|SkillCache")
	FGameplayTag PreviousWeaponSlotTag;

	/* 현재 궁극기 사용 중 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle|SkillCache")
	bool bUsingLegendaryWeapon = false;
#pragma endregion

#pragma region 물약
public:
	UFUNCTION(BlueprintCallable)
	void BuildBattlePotionCache(FInBattleData& OutInBattleData);

	UFUNCTION(BlueprintCallable)
	void DecreaseBattlePotionCount(int32 InSlotIndex);

	UFUNCTION(BlueprintCallable)
	TArray<FBattlePotionRuntimeSlotData>& GetBattlePotionRuntimeSlots();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Potion", meta = (AllowPrivateAccess = "true"))
	TArray<FBattlePotionRuntimeSlotData> BattlePotionRuntimeSlots;
#pragma endregion
};
