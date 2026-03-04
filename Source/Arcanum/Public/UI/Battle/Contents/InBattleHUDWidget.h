// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "InBattleHUDWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickActionButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleToggle, bool, bIsChecked);

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
	//// 드롭 이벤트 처리
	//virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
#pragma endregion


#pragma region 플레이어, 보스 체력바(건물아님)
public:
	UFUNCTION()
	void SetPlayerCharacterHealthBarProgress(float CurrentHealth, float MaxHealth);
	UFUNCTION()
	void SetBossHealthBarProgress(float CurrentHealth, float MaxHealth);

	UFUNCTION()
	void ShowBosHealthPBar(bool bIsShow);
#pragma endregion


#pragma region 스테이지 진행도
	UFUNCTION()
	void SetPlayerLocationProgress(const FVector& InAllyNexusLocation, const FVector& InEnemyNexusLocation, const FVector& InPlayerLocation);
	UFUNCTION()
	void SetTime(int32 TimeMS);
#pragma endregion


#pragma region 전투 관련 버튼들
public:
	FOnClickActionButton OnClickBasicAttack;
	FOnClickActionButton OnClickUltimateSkill;
	FOnClickActionButton OnClickBasicSkill;
	FOnClickActionButton OnClickWeaponSwap;
	FOnClickActionButton OnClickItem1;
	FOnClickActionButton OnClickItem2;
	FOnBattleToggle OnToggleAutoManualMode;

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

public:
	UFUNCTION()
	const FGameplayTag& GetSelectUnitTag() const {return SelectedUnitTag;}

	UFUNCTION()
	const FVector& GetSelectUnitDropLocation() const {return SelectedUnitDropLocation;}
#pragma endregion

#pragma region 플레이어 유닛, 코스트 패널
public:
	FORCEINLINE UBattleAllyUnitPanelWidget* GetPlayerInfoPanel() const { return PlayerInfoPanel; }
#pragma endregion



#pragma region 바인딩
protected:
	void BindCallbacks();

protected:
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
	TObjectPtr<UBattleActionButtonWidget> Item1 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleActionButtonWidget> Item2 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleToggleWidget> AutoManualMode = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleAllyUnitPanelWidget> PlayerInfoPanel = nullptr;
#pragma endregion

private:
	FGameplayTag SelectedUnitTag;
	FVector SelectedUnitDropLocation;
};
