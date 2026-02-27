// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "UnitHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCANUM_API UUnitHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetPercent(const FRegenStat& InHealthData);
	void SetPercent(float CurrentHealth, float MaxHealth);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthProgress = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> HealthText = nullptr;
};
