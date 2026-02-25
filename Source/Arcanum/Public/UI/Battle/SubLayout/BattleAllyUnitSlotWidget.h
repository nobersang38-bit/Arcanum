// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "BattleAllyUnitSlotWidget.generated.h"

/**
 * 김도현
 */
class UTextBlock;
class UButton;
class UImage;
UCLASS()
class ARCANUM_API UBattleAllyUnitSlotWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion
public:
	UFUNCTION()
	void SetUnitInfo(int32 InCost, UTexture2D* InImage, FGameplayTag InUnitTag);

	UFUNCTION()
	FORCEINLINE FGameplayTag GetUnitInfo() { return UnitTag; }

	UFUNCTION()
	void SetDisabled(bool InIsDisable);

protected:
	UFUNCTION()
	void SetCost(int32 InCost);

	UFUNCTION()
	void SetImage(UTexture2D* InImage);

	UFUNCTION()
	void SetUnitTag(FGameplayTag InUnitTag);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> Button = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> DisabledImage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CostText = nullptr;

private:
	FGameplayTag UnitTag;
};
