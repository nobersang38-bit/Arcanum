// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasementHealthBar.generated.h"

class UTextBlock;
class UProgressBar;

/*
 * 역할 : 김유진
 * 1. 기지용 Health 표시 바
 */

UCLASS()
class ARCANUM_API UBasementHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void UpdateBasementHealthBar(float CurrentHealth, float MaxHealth);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentHealthText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHealthText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UProgressBar> BasementHealthBar = nullptr;

};
