#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "CommonTabList.generated.h"

class UPanelWidget;

/// 260318 추가

UCLASS()
class ARCANUM_API UCommonTabList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitializeTabs();
	void GoToNext();
	void GoToPrev();
	void SetCurrentTab(FGameplayTag NewTag);
	
protected:
	virtual void NativeConstruct() override;

	TMap<FGameplayTag, UUserWidget*> TabInstances;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "00-Global") TMap<FGameplayTag, TSubclassOf<UUserWidget>> TabClasses;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "00-Global") TArray<FGameplayTag> TabOrder;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UPanelWidget> Container;
	
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UButton> PrevBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UButton> NextBtn;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UTextBlock> TabNameText;
	
	UPROPERTY(meta = (BindWidget)) TObjectPtr<class UWidgetSwitcher> TabSwitcher;
private:
	FGameplayTag CurrentTag;

	UFUNCTION() void OnPrevButtonClicked();
	UFUNCTION() void OnNextButtonClicked();
};