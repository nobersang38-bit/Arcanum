// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "Data/Types/UnitData.h"
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
#pragma region 언리얼 기본생성
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
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
	void UltimateSkill();

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
	void ReadySpawnUnit(FGameplayTag InTag);

	UFUNCTION()
	void SpawnUnit(FGameplayTag InTag);

	// 사용할 고기
	UFUNCTION()
	bool UseMeatValue(float Value);

	// 사용할 마나
	UFUNCTION()
	bool UseManaValue(float Value);
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
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float PlayerLocationProgressUpdateInterval = 0.05f;

protected:
	UPROPERTY()
	TWeakObjectPtr<AActor> AllyBasement = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> EnemyBasement = nullptr;

	UPROPERTY()
	TSubclassOf<class ABaseUnitCharacter> AllyUnitClass = nullptr;

protected:
	UPROPERTY()
	TMap<FGameplayTag, FUnitData> UsinAllyUnits;

	// Todo KDH : 데이터 가져오게 변경해야함
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRegenStat MeatValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRegenStat ManaValue;

	FTimerHandle MeatTimer;
	FTimerHandle ManaTimer;

	FGameplayTag SpawnTag;

private:
	FTimerHandle PlayerLocationProgressTimeHandle;
	bool bIsAutoManual = false;
};
