// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Types/UnitData.h"
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



#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion


public:
	UFUNCTION()
	void SetMeatCostProgress(float CurrentMeat, float MaxMeat);

	UFUNCTION()
	void SetManaCostProgress(float CurrentMana, float MaxMana);

	// 맨 뒤에 슬롯 추가
	UFUNCTION()
	UBattleAllyUnitSlotWidget* AddUnitSlot(const FUnitInfoSetting& UnitData);

	// 인덱스 추가 안하면 마지막 제거, 첫번째 인덱스와 마지막 인덱스는 스페이서가 있어서 0을 넣으면 1부터 검사하고, -1을 넣으면 마지막 인덱스인 스페이서를 제외하여 제거함
	UFUNCTION()
	bool RemoveUnitSlot(int32 RemoveIDX = -1);

	UFUNCTION()
	TArray<UBattleAllyUnitSlotWidget*> GetUnitSlots() const;


#pragma region 위젯 바인딩
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UnitSlotClass")
	TSubclassOf<class UBattleAllyUnitSlotWidget> UnitSlotWidgetClass = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleCostBarWidget> MeatCostBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBattleCostBarWidget> ManaCostBar = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> UnitPanel = nullptr;
#pragma endregion


#pragma region 디버그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bUseDebugAddUnitSlot = false;
#pragma endregion
};
