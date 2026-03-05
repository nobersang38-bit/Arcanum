// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTags/ArcanumTags.h"
#include "RoundedSlotWidget.generated.h"
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCharacterSlotClicked, URoundedSlotWidget*, ClickedSlot, FName, CharacterName, bool, SlotCharacterOwned);

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
	void SetIconImage(UTexture2D* CharacterIcon, bool OwnedCharacter ,FName CharacterName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor RoundColor = FLinearColor::White;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> IconImg;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundColor;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	// 캐릭터 미보유시 어둡게 표출되도록
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SlotDimOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	FName SlotCharacterName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	bool SlotCharacterOwned;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	bool bClicked;
};
