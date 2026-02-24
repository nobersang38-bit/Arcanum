// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleAllyUnitPanelWidget.generated.h"

/**
 * 김도현
 */
class UBattleCostBarWidget;
class UHorizontalBox;
UCLASS()
class ARCANUM_API UBattleAllyUnitPanelWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

public:
	UFUNCTION()
	void SetMeatCostProgress(float CurrentMeat, float MaxMeat);
	UFUNCTION()
	void SetManaCostProgress(float CurrentMana, float MaxMana);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleCostBarWidget> MeatCostBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleCostBarWidget> ManaCostBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> UnitPanel = nullptr;
};
