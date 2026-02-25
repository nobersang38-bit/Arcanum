// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundedSlotWidget.generated.h"

/**
 * 
 */

class UImage;
class UBorder;

UCLASS()
class ARCANUM_API URoundedSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetRoundBackgroundColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable)
	void SetIconImage(UTexture2D* Texture);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundColor;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> IconImage;
};
