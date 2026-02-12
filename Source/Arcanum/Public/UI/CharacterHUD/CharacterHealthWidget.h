// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/Stats/Data/FBattleStats.h"
#include "CharacterHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCANUM_API UCharacterHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetPercent(const FRegenStat& InHealthData);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthProgress = nullptr;
};
