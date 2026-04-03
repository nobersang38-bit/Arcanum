#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "StageList.generated.h"

class UBorder;
class UTextBlock;
class UImage;
class UButton;
class UWidgetAnimation;

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
	void SetStageImg(TObjectPtr<UTexture2D> StageImg);

	void SetText(FString StageName, FString StageInfo);

protected:
	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<UBorder> SelectBorder = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnClick = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<UImage> StageImage = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<UTextBlock> StageNameText = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	TObjectPtr<UTextBlock> StageInfoText = nullptr;
	UPROPERTY(meta = (BindWidgetAnim), Transient) 
	TObjectPtr<UWidgetAnimation> BlinkAnim = nullptr;
	private:
	bool bSelected = false;
	FString DefaultStrName = TEXT("스테이지 0");
	FString DefaultStrInfo = TEXT("스테이지 설명");
};
