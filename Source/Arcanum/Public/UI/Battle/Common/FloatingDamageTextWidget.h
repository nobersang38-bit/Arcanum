// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingDamageTextWidget.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UFloatingDamageTextWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetText(FText InText);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> HideAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> FloatingText = nullptr;
};
