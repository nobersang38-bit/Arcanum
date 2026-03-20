#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sound/SoundClass.h"
#include "AudioTabWidget.generated.h"

/// 260318 추가
class UOptionItemWidget;
class UOptionDataObject;

UCLASS()
class ARCANUM_API UAudioTabWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;
    virtual void SynchronizeProperties() override;
    void RefreshOptions();

    UPROPERTY(meta = (BindWidget)) TObjectPtr<class UVerticalBox> ContentBox;
    UPROPERTY(EditAnywhere) TSubclassOf<UOptionItemWidget> ScalarWidgetClass;
    UPROPERTY(EditAnywhere,BlueprintReadWrite)  float Height = 20.f;
    
    UPROPERTY(EditAnywhere, Category = "Audio Settings") TSoftObjectPtr<USoundClass> MasterSoundClass;
    UPROPERTY(EditAnywhere, Category = "Audio Settings") TSoftObjectPtr<USoundClass> MusicSoundClass;
    UPROPERTY(EditAnywhere, Category = "Audio Settings") TSoftObjectPtr<USoundClass> SFXSoundClass;
    UPROPERTY(EditAnywhere, Category = "Audio Settings") TSoftObjectPtr<USoundMix> DefaultSoundMix;
    
private:
    void AddOption(FName ID, FText Name, float Default);
    void AddOption(FName ID, FText Name, bool bDefault);
    void AddOption(FName ID, FText Name, int32 DefaultIndex, const TArray<FText>& Options);

    void AddSpacer();
    void CreateAndAddWidget(UOptionDataObject* Data);
    
    void ApplyAudioVolume(FName ID, float NewValue);
    void ApplyAudioBinarySetting(FName ID, bool bValue);
};