// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "Data/Types/UnitData.h"
#include "Data/Types/MatchData.h"
#include "DataInfo/SkillData/Data/FBattleWeaponSkillData.h"
#include "GameplayTags/ArcanumTags.h"
#include "BattlePlayerController.generated.h"

UENUM()
enum class ESkillType : uint8
{
	BasicAttack,
	BasicSkill,
	UltimateSkill
};

class UInputMappingContext;
class UInputAction;
class UInBattleHUDWidget;
struct FInputActionValue;

// 김도현
UCLASS()
class ARCANUM_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
	friend class UBTPlayerDataObject;
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
	void DebugBossHealthBar(float CurrentHealth, float MaxHealth);

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
	void SetBossHealthProgress(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	bool SkillCostChecker(FGameplayTag InSkillTag, int32 InLevel, bool bIsOnlyManaCheck = false);

#pragma endregion


#pragma region 메인
protected:
	UFUNCTION()
	void BasicAttack();

	UFUNCTION()
	void BasicSkill();

	UFUNCTION()
	void WeaponSwap();

	UFUNCTION()
	void Item1();

	UFUNCTION()
	void Item2();

	UFUNCTION()
	bool SkillStarter(FGameplayTag InSkillTag, int32 InLevel, bool bIsUltimate = false);

	UFUNCTION()
	void ReadySkillSet(FGameplayTag InSkillTag, int32 InLevel, bool bIsUltimate = false);

	UFUNCTION()
	void CurrentSelectedSkillStarter();

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
protected:
	/* 궁극기 종료 처리 */
	void UltimateSkillEnd();

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
};
