// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InBattleHUDWidget.generated.h"

/**
 * 김도현
 */
class UBattleActionButtonWidget;
class UBattleAllyUnitPanelWidget;
class UBattleToggleWidget;
class UBattleCostBarWidget;
class UBattleHealthBarWidget;
class UBattleStageProgressWidget;

UCLASS()
class ARCANUM_API UInBattleHUDWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성 및 초기화
protected:
	virtual void NativeConstruct() override;
#pragma endregion
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleHealthBarWidget> PlayerCharacterHealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleHealthBarWidget> BossHealthBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleStageProgressWidget> StageProgress = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleActionButtonWidget> BasicAttack = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleActionButtonWidget> UltimateSkill = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleActionButtonWidget> BasicSkill = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleActionButtonWidget> WeaponSwap = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleActionButtonWidget> Item2 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleActionButtonWidget> Item1 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleToggleWidget> AutoManualMode = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleAllyUnitPanelWidget> PlayerInfoPanel = nullptr;
};
