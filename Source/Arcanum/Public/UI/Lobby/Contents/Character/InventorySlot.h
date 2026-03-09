// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UCommonBtnWidget;
class UTextBlock;
class USquareSlotWidget;
class UWrapBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetupBtnClicked);

/**
 * 
 */
UCLASS()
class ARCANUM_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EquipNameTxt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EquipInfoTxt;

	UPROPERTY(BlueprintAssignable)
	FOnSetupBtnClicked OnSetupBtnClicked;

private:
	UFUNCTION()
	void ClickEquipSetupBtn();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SetupBtn;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EquipNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EquipInfoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> EquipGridPanel;

};
