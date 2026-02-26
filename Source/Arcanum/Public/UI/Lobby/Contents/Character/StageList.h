// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageList.generated.h"

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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StgName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StgInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> StageImg;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> StageImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageInfoText;
};
