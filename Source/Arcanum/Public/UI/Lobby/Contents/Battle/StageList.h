// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStageListClicked, UStageList*, ClickedSlot, FName, StageName);

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class ARCANUM_API UStageList : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stage")
	FOnStageListClicked OnStageListClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StgName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StgInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> StageImg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SelecetedStage = false;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> StageImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageInfoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
	FName StageRowName;


};
