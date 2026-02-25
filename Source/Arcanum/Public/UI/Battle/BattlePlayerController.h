// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePlayerController.generated.h"

//김도현
class UInBattleHUDWidget;
UCLASS()
class ARCANUM_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
#pragma region 언리얼 기본생성
protected:
	virtual void BeginPlay() override;
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

protected:
	UFUNCTION()
	void ClickBasicAttack();

	UFUNCTION()
	void ClickUltimateSkill();

	UFUNCTION()
	void ClickBasicSkill();

	UFUNCTION()
	void ClickWeaponSwap();

	UFUNCTION()
	void ClickItem1();

	UFUNCTION()
	void ClickItem2();

	UFUNCTION()
	void ToggleAutoManualMode(bool bIsChecked);
#pragma endregion


#pragma region 인풋모드 설정
protected:
	void SetupInputMode();
#pragma endregion
};
