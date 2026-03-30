// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "Data/Types/UnitData.h"
#include "Data/Types/MatchData.h"
#include "DataInfo/SkillData/Data/FBattleWeaponSkillData.h"
#include "DataInfo/SkillData/Data/FSkillInfo.h"
#include "GameplayTags/ArcanumTags.h"
#include "BattlePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInBattleHUDWidget;
struct FInputActionValue;

UENUM()
enum class ESkillType : uint8
{
	BasicAttack,
	BasicSkill,
	UltimateSkill
};

UENUM()
enum class EBattleActionType : uint8
{
	Move,
	BasicAttack,
	CommonSkill,
	UltimatePress,
	UltimateRelease,
	Item1,
	Item2,
	WeaponSwap,
	ReadySpawnUnit
};

// 김도현
UCLASS()
class ARCANUM_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
	friend class UBTPlayerDataObject;
	friend class UBTService_PlayerSelectTarget;
#pragma region 언리얼 기본생성
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
#pragma endregion


#pragma region 디버그(콘솔 호출 후, 함수 이름으로 실행가능)

#pragma region 메인HUD
protected:
	UFUNCTION(Exec)
	void DebugPlayPlayerCharacterHealthBar(float CurrentHealth, float MaxHealth);

	UFUNCTION(Exec)
	void DebugAddPlayerInfoPanelSlot();

	UFUNCTION(Exec)
	void DebugRemovePlayerInfoPanelSlot(int32 RemoveIDX);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|BattleActionButton")
	bool bUseDebugBattleActionButton = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|BattleActionButton")
	float DebugBattleActionButtonCoolTime = 3.0f;

	TMap<FTimerHandle, TWeakObjectPtr<class UBattleActionButtonWidget>> ActionButtons;
#pragma endregion

#pragma endregion


#pragma region HUD 위젯 클래스
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UInBattleHUDWidget> HUDWidgetClass = nullptr;

	UPROPERTY()
	TObjectPtr<UInBattleHUDWidget> HUDWidgetInstance = nullptr;
#pragma endregion

#pragma region 메인HUD
protected:
	void SetupMainHUDWidget();

	UFUNCTION()
	void UpdatePlayerLocationProgress();

	UFUNCTION()
	void UpdateStageTime(int32 TimeSecond);

	UFUNCTION()
	void SetAllyUsingWidget();

	UFUNCTION()
	void UpdateMeatValue(float DeltaTime);

	UFUNCTION()
	void UpdateManaValue(float DeltaTime);

public:
	UFUNCTION()
	void SetPlayerHealthProgress(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	bool SkillCostChecker(FGameplayTag InSkillTag, int32 InLevel, bool bIsOnlyManaCheck = false);

#pragma endregion


#pragma region 메인
protected:
	//UFUNCTION()
	//void BasicAttack();

	//UFUNCTION()
	//void BasicSkill();

	UFUNCTION()
	void WeaponSwap();

	UFUNCTION()
	void Item1();

	UFUNCTION()
	void Item2();

	UFUNCTION()
	void UseBattlePotion(int32 InSlotIndex);

    //UFUNCTION()
	//bool SkillStarter(FGameplayTag InSkillTag, int32 InLevel, bool bIsUltimate = false);

	UFUNCTION()
	void ReadySkillSet(FGameplayTag InSkillTag, int32 InLevel, bool bIsUltimate = false);

	//UFUNCTION()
	//void CurrentSelectedSkillStarter();


	//USkillBase* GetOrCreateSkillBase(const FBattleSkillData& InSkillData, const FSkillInfo* InSkillInfo, const FVector& InTargetLocation);

	//bool SpawnAndActivateSkillActor(USkillBase* InSkillBase, UClass* InSkillActorClass, const FVector& InTargetLocation);

	UFUNCTION()
	void SkillCancel();

	UFUNCTION()
	void AutoManualModeMobile(bool bIsChecked);

	UFUNCTION()
	void AutoManualModePC();

	UFUNCTION()
	void SlotSelectCancel();

	UFUNCTION()
	void CommonButton();

	UFUNCTION()
	void ReadySpawnUnit(FGameplayTag InTag, UBattleAllyUnitSlotWidget* Slot);

	UFUNCTION()
	void SetSpawnDecalActive(bool bIsOn);

	UFUNCTION()
	ABaseUnitCharacter* Internal_SpawnUnit(const FVector InSpawnLocation = FVector::ZeroVector);

	// 사용할 고기
	UFUNCTION()
	bool UseMeatValue(float Value);

	// 사용할 마나
	UFUNCTION()
	bool UseManaValue(float Value);

	// 쿨타임
	UFUNCTION()
	bool UseCoolTime(FGameplayTag InTag);

	// 스킬 쿨타임
	UFUNCTION()
	bool UseSkillCost(FGameplayTag InTag);
#pragma endregion

#pragma region 전투 종료
	UFUNCTION()
	void BattleEnd(const FMatchData& MatchData);

	UFUNCTION()
	void OpenLobbyLevel();
#pragma endregion



#pragma region 내부 함수
	UFUNCTION()
	bool IsUnitUsingEnable(FGameplayTag InTag);

	UFUNCTION()
	bool UsingUnitCost(FGameplayTag InTag);

	// 쿨타임을 계속 줄임
	UFUNCTION()
	void Internal_CoolTimeTick(float DeltaTime);

	UFUNCTION()
	void InitialSkillBase();

#pragma endregion

#pragma region 범위
protected:
	void RangeDecalOn(float DecalSize);
	void Internal_RangeDecalOn(float DecalSize);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RangeTime")
	float RangeTime = 1.0f;

	FTimerHandle RangeDecalTimerHandle;
#pragma endregion



#pragma region 인풋모드 설정
protected:
	void SetupInputMode();
#pragma endregion

#pragma region 입력 관련
protected:
	UFUNCTION()
	void InputMove(const FInputActionValue& InputValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BasicAttack = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BasicSkill = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_UltimateSkill = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Item1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Item2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_WeaponSwap = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_AutoManual = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_InputCancel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_CommonButton = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BasicAttackSkill = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_CommonSkill = nullptr;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float PlayerLocationProgressUpdateInterval = 0.05f;

protected:
	UPROPERTY()
	TWeakObjectPtr<AActor> AllyBasement = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> EnemyBasement = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FUnitInfoSetting> UsingAllyUnits;

	UPROPERTY()
	TMap<FGameplayTag, class UBattleAllyUnitSlotWidget*> UsingAllyUnitSlots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRegenStat MeatValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRegenStat ManaValue;

	FTimerHandle MeatTimer;
	FTimerHandle ManaTimer;
	FTimerHandle CoolTimeTimer;

	FGameplayTag SpawnTag;

	UPROPERTY()
	TWeakObjectPtr<UBattleAllyUnitSlotWidget> SelectedSlot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ABaseUnitCharacter> SelectedUnit = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ABaseUnitCharacter> SelectedUnit2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TSubclassOf<class ASelectedArrow> SelectedArrowClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<class ASelectedArrow> SelectedArrowInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TSubclassOf<class ASkillRangeDecal> SkillRangeDecalClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<class ASkillRangeDecal> SkillRangeDecalInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTag MeatTag = Arcanum::BattleStat::Player::Regen::Meat::Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTag ManaValueTag = Arcanum::BattleStat::Character::Regen::Mana::Value;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTag ManaTag = Arcanum::BattleStat::Character::Regen::Mana::Root;

private:
	FTimerHandle PlayerLocationProgressTimeHandle;
	bool bIsAutoManual = false;
	float StageTimeSecond = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, float> SkillCoolTimes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, TObjectPtr<class USkillBase>> SkillBaseInstances;

	UPROPERTY()
	TWeakObjectPtr<class APlayerCharacter> CachedPlayerCharacter = nullptr;

	UPROPERTY()
	TWeakObjectPtr<class USkillBase> CurrentSelectedSkillBase = nullptr;

	FVector SkillLocation = FVector::ZeroVector;

	bool bIsSkillSuccess = false;

	UPROPERTY()
	TWeakObjectPtr<class APostProcessVolume> CachedPostProcessVolume = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> SkillTargetActor = nullptr;

#pragma region 궁극기 처리
public:
	/* 궁극기 종료 처리 */
	void UltimateSkillEnd();

protected:
	/* 궁극기 조준 시작 */
	UFUNCTION()
	void UltimateSkillPressed();

	/* 궁극기 조준 해제 */
	UFUNCTION()
	void UltimateSkillReleased();

	/* 궁극기 자동 발사 */
	void ExecuteUltimateSkill();

protected:
	/* 궁극기 조준 중 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate")
	bool bIsUltimateAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate")
	TObjectPtr<class APostProcessVolume> UltimatePostProcessVolume = nullptr;

	/* 마지막 이동 입력 방향 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate")
	FVector LastMoveInputDirection = FVector::ZeroVector;

	/* 궁극기 프리뷰 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate")
	float UltimatePreviewDistance = 300.0f;

	/* 궁극기 프리뷰 위치 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate")
	FVector CurrentUltimatePreviewLocation = FVector::ZeroVector;

	/* 궁극기 프리뷰 이동 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate")
	float UltimatePreviewMoveSpeed = 1000.0f;

	/* 궁극기 프리뷰 최대 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate")
	float UltimatePreviewMaxDistance = 1200.0f;

	FTimerHandle UltimateSkillTimerHandle;
#pragma endregion

#pragma region 기본공격, 일반 스킬 처리
public:
	/* 기본공격 스킬 실행 */
	UFUNCTION()
	void TriggerBasicAttackHit();

	/* 일반스킬 실제 실행 */
	UFUNCTION()
	void TriggerSkill();

protected:
	/* 기본공격 입력 */
	UFUNCTION()
	void InputBasicAttack();

	/* 스킬 입력 */
	UFUNCTION()
	void InputSkill();
	
private:
	/* 스왑 몽타주 종료 */
	UFUNCTION()
	void OnWeaponSwapMontageEnded(UAnimMontage* InMontage, bool bInterrupted);

	/* 무기 스왑 중 여부 */
	bool bIsWeaponSwapping = false;
#pragma endregion

#pragma region 궁극기 연출
public:
	/* 궁극기 Press 연출 시작 */
	void StartUltimatePresentation();

	/* 궁극기 Release 직전 피크 연출 */
	void PlayUltimateReleasePresentation();

	/* 궁극기 연출 복귀 시작 */
	void EndUltimatePresentation();

protected:
	/* 궁극기 카메라/FOV/후처리 보간 갱신 */
	void UpdateUltimatePresentation(float DeltaTime);

protected:
	/* 궁극기 연출 진행 중 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	bool bIsUltimatePresentationActive = false;

	/* 궁극기 연출 복귀 중 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	bool bIsUltimatePresentationRestoring = false;

	/* Release 직전 2차 확대 진행 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	bool bIsUltimateReleaseZoomActive = false;

	/* 기본 카메라 FOV */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimateDefaultFOV = 0.0f;

	/* Press 시 1차 목표 FOV */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimatePressTargetFOV = 80.0f;

	/* Release 시 2차 목표 FOV */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimateReleaseTargetFOV = 120.0f;

	/* Press 줌 보간 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimatePressZoomInterpSpeed = 6.0f;

	/* Release 줌 보간 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimateReleaseZoomInterpSpeed = 12.0f;

	/* 원복 보간 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimateRestoreInterpSpeed = 5.0f;

	/* 현재 목표 PostProcess 블렌드 웨이트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimateTargetPostProcessBlendWeight = 0.0f;

	/* Press 시 목표 PostProcess 블렌드 웨이트 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimatePressPostProcessBlendWeight = 1.0f;

	/* PostProcess 보간 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	float UltimatePostProcessInterpSpeed = 5.0f;

	/* Release 순간 재생할 카메라 쉐이크 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate|Presentation")
	TSubclassOf<class UCameraShakeBase> UltimateCameraShakeClass = nullptr;
	UPROPERTY(Transient)
	TObjectPtr<class UCameraShakeBase> ActiveUltimateCameraShake = nullptr;
#pragma endregion

#pragma region 플레이어 스킬 쿨타임
protected:
	/* 스킬 쿨타임 시작 */
	void StartSkillCooldown(const FGameplayTag& InSkillTag, float InCooldown);

	/* 스킬 남은 쿨타임 반환 */
	float GetSkillCooldownRemaining(const FGameplayTag& InSkillTag) const;

	/* 스킬 전체 쿨타임 반환 */
	float GetSkillCooldown(const FGameplayTag& InSkillTag) const;

protected:
	/* 스킬별 전체 쿨타임 */
	TMap<FGameplayTag, float> SkillCooldownMap;

	/* 스킬별 남은 쿨타임 */
	TMap<FGameplayTag, float> SkillCooldownRemainingMap;

protected:
	/* 스킬 쿨타임 타이머 갱신 */
	void UpdateSkillCooldown();

	/* 기본 공격 쿨타임 시작 */
	void StartBasicAttackCooldown();

	/* 일반 스킬 쿨타임 시작 */
	void StartBasicSkillCooldown();

	/* 궁극기 쿨타임 시작 */
	void StartUltimateCooldown();

	/* 스킬 쿨타임 UI 갱신 */
	void RefreshSkillCooldown();

	/* 플레이어 스킬 쿨타임 타이머 핸들 */
	FTimerHandle SkillCooldownTimerHandle;

	/* 기본 공격 남은 쿨타임 */
	float BasicAttackCooldownRemaining = 0.0f;

	/* 일반 스킬 남은 쿨타임 */
	float BasicSkillCooldownRemaining = 0.0f;

	/* 궁극기 남은 쿨타임 */
	float UltimateCooldownRemaining = 0.0f;

	/* 기본 공격 쿨타임 */
	float BasicAttackCooldown = 0.0f;

	/* 일반 스킬 쿨타임 */
	float BasicSkillCooldown = 0.0f;

	/* 궁극기 쿨타임 */
	float UltimateCooldown = 0.0f;

	float SkillCooldownTickInterval = 0.02f;
#pragma endregion

#pragma region 스킬 마나 코스트
	/* 스킬 마나 코스트 UI 갱신 */
	void RefreshSkillCost();
#pragma endregion

#pragma region 물약
protected:
	/* 전투 물약 UI 갱신 */
	UFUNCTION()
	void RefreshBattlePotion();

	/* 전투 물약 쿨타임 갱신 */
	UFUNCTION()
	void UpdateBattlePotionCooldown(float InDeltaTime);

	void OnPotionMontageEnded(UAnimMontage* InMontage, bool bInterrupted);

protected:
	FTimerHandle BattlePotionCooldownTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
	float BattlePotionCooldownTickInterval = 0.02f;

private:
	bool bIsPotionMontagePlaying = false;
#pragma endregion

#pragma region 전투 행동 실행 가능 여부
private:
	bool TryExecuteBattleAction(EBattleActionType InActionType);
#pragma endregion

#pragma region 버프
protected:
	UFUNCTION()
	void BindBuffUI();

	UFUNCTION()
	void HandleBuffUpdated(const FGameplayTag& InBuffTag, float InPercent);

	UFUNCTION()
	void HandleBuffRemoved(const FGameplayTag& InBuffTag);
#pragma endregion

};
