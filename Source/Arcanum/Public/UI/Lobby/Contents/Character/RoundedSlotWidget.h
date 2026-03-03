// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RoundedSlotWidget.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterSlotClicked, URoundedSlotWidget*, ClickedSlot, int32, SlotIndex);

class UImage;
class UBorder;

UCLASS()
class ARCANUM_API URoundedSlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Slot")
	FOnCharacterSlotClicked OnCharacterSlotClicked;

	UFUNCTION(BlueprintCallable)
	void SetRoundBackgroundColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable)
	void SetIconImage(UTexture2D* Texture);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor RoundColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> IconImg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowEmptySlotOverlay = true;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundColor;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	// 캐릭터 소지 안 하면 어둡게 표출되도록
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlotDimOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 SlotIndex;
};
