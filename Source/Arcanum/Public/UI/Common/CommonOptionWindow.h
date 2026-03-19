#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonOptionWindow.generated.h"

class UButton;
class UCommonTabList;
class UCommonBtnWidget;

UCLASS()
class ARCANUM_API UCommonOptionWindow : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget)) TObjectPtr<UCommonTabList> TabNavigation;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UCommonBtnWidget> CloseBtn;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UCommonBtnWidget> SaveBtn;

private:
    UFUNCTION() void OnCloseButtonClicked();
    UFUNCTION() void OnSaveButtonClicked();
};
