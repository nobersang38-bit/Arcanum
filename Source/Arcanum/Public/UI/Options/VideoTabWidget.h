#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VideoTabWidget.generated.h"

/// 260318 추가

class UOptionItemWidget;
class UOptionDataObject;

UCLASS()
class ARCANUM_API UVideoTabWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;
    virtual void SynchronizeProperties() override;
    void RefreshOptions();

    UPROPERTY(meta = (BindWidget)) TObjectPtr<class UVerticalBox> ContentBox;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<class UScrollBox> MainScrollBox;
    UPROPERTY(EditAnywhere) TSubclassOf<UOptionItemWidget> ScalarWidgetClass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)  float Height = 20.f;

private:
    void AddOption(FName ID, FText Name, float Default);
    void AddOption(FName ID, FText Name, bool bDefault);
    void AddOption(FName ID, FText Name, int32 DefaultIndex, const TArray<FText>& Options);

    void AddSpacer();
    void CreateAndAddWidget(UOptionDataObject* Data);

    void ApplyVideoSetting(FName ID, float NewValue);
};
