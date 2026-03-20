// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "Data/Types/UnitData.h"
#include "Data/Types/MatchData.h"
#include "DataInfo/SkillData/Data/FBattleWeaponSkillData.h"
#include "BattlePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UInBattleHUDWidget;
struct FInputActionValue;

// 김도현
UCLASS()
class ARCANUM_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
	friend struct FBTPlayerStruct;
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
	ABaseUnitCharacter* Internal_SpawnUnit();

	// 사용할 고기
	UFUNCTION()
	bool UseMeatValue(float Value);

	// 사용할 마나
	UFUNCTION()
	bool UseManaValue(float Value);

	// 쿨타임
	UFUNCTION()
	bool UseCoolTime(FGameplayTag InTag);
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
	void Internal_UnitsCoolTimeTick(float DeltaTime);

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
	UPROPERTY()
	TMap<FGameplayTag, FUnitInfoSetting> UsingAllyUnits;

	UPROPERTY()
	TMap<FGameplayTag, class UBattleAllyUnitSlotWidget*> UsingAllyUnitSlots;

	// Todo KDH : 데이터 가져오게 변경해야함
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

private:
	FTimerHandle PlayerLocationProgressTimeHandle;
	bool bIsAutoManual = false;
	float StageTimeSecond = 0.0f;

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

	/* 궁극기 조준 위치 갱신 */
	void UpdateUltimatePreview();

protected:
	/* 궁극기 조준 중 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate")
	bool bIsUltimateAiming = false;

	/* 마지막 이동 입력 방향 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate")
	FVector LastMoveInputDirection = FVector::ZeroVector;

	/* 궁극기 프리뷰 거리 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate")
	float UltimatePreviewDistance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ultimate")
	TObjectPtr<class APostProcessVolume> UltimatePostProcessVolume = nullptr;

	FTimerHandle UltimateSkillTimerHandle;
#pragma endregion
};
