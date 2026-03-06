// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "SquareSlotWidget.generated.h"

class UImage;
class UBorder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotClicked,USquareSlotWidget*, ClickedSlot,int32, SlotIndex);
/**
 * 
 */
UCLASS()
class ARCANUM_API USquareSlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Slot")
	FOnWeaponSlotClicked OnSlotClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BackColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> IconImg;

	UFUNCTION(BlueprintCallable)
	void SetSquareBackgroundColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable)
	void SetItemIconImage(UTexture2D* ItemIcon, bool bIsEquipped);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundColor;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	FGameplayTag WeaponTag;


};
