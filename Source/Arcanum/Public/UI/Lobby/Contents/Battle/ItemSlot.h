// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

class UCommonBtnWidget;
class UTextBlock;
class USquareSlotWidget;
class UUniformGridPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetItemBtnClicked);
/**
 * 
 */
UCLASS()
class ARCANUM_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemNameTxt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemInfoTxt;

	UPROPERTY(BlueprintAssignable)
	FOnSetItemBtnClicked OnSetItemBtnClicked;

private:
	UFUNCTION()
	void ClickSetItemBtn();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SetItemBtn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemInfoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemGridPanel;
};
