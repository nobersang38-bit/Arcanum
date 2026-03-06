// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "StageList.generated.h"

class UBorder;
class UTextBlock;
class UImage;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageClicked, UStageList*, StageWidget);

UCLASS()
class ARCANUM_API UStageList : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(BlueprintAssignable) FOnStageClicked OnStageClicked;
private:
	UFUNCTION() void HandleClick();
	
protected:
	virtual void NativePreConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag StageTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StgName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText StgInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> StageImg;

	void SetSelected(bool bInSelected);

	void SetText(FString StageName, FString StageInfo);

protected:
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UBorder> SelectBorder;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> BtnClick;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> StageImage;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> StageNameText;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UTextBlock> StageInfoText;
private:
	bool bSelected = false;
	FString DefaultStrName = TEXT("스테이지 0");
	FString DefaultStrInfo = TEXT("스테이지 설명");
};
