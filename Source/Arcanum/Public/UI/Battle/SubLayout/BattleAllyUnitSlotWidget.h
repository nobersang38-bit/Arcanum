// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "BattleAllyUnitSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClickUnitSlot, FGameplayTag, UnitTag, UBattleAllyUnitSlotWidget*, SlotWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPressUnitSlot, FGameplayTag, UnitTag, UBattleAllyUnitSlotWidget*, SlotWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReleasedUnitSlot, FGameplayTag, UnitTag, UBattleAllyUnitSlotWidget*, SlotWidget);

/**
 * 김도현
 */
class UTextBlock;
class UButton;
class UImage;
class UProgressBar;
class UBorder;
UCLASS()
class ARCANUM_API UBattleAllyUnitSlotWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

#pragma endregion

public:
	FOnClickUnitSlot OnClickUnitSlot;
	FOnPressUnitSlot OnPressUnitSlot;
	FOnReleasedUnitSlot OnReleasedUnitSlot;

	UFUNCTION()
	void SetUnitInfo(int32 InCost, UTexture2D* InImage, FGameplayTag InUnitTag);

	UFUNCTION()
	void SetActivateCost(bool InIsDisable);

	UFUNCTION()
	void SetCoolTimeProgress(float CurrentProgress, float MaxProgress);

	UFUNCTION()
	void SetUnitTag(FGameplayTag InUnitTag);

	UFUNCTION()
	void SetSelectSlot(bool InIsSelected);

	UFUNCTION()
	FGameplayTag GetUnitTag() const { return UnitTag; }


#pragma region 내부 함수
protected:
	UFUNCTION()
	void SetCost(int32 InCost);

	UFUNCTION()
	void SetImage(UTexture2D* InImage);

	UFUNCTION()
	void SetProgressesVisible(bool IsVisible);

	UFUNCTION()
	void ClickUnitSlot();

	UFUNCTION()
	void PressUnitSlot();

	UFUNCTION()
	void ReleasedUnitSlot();
#pragma endregion


#pragma region 위젯 바인딩
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> SlotBorder = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> DisabledImage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CostText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CoolTimeText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> CoolTimeProgress = nullptr;
#pragma endregion


#pragma region 디버그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|CoolTime")
	bool bIsDebugCoolTimeStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|CoolTime")
	float DebugCoolTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|CoolTime")
	float DebugMaxCoolTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|Cost")
	bool bIsDebugCostDisableImage = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|Cost")
	bool bIsSetDebugCostDisableImage = false;
#pragma endregion


private:
	FGameplayTag UnitTag;
};
