#include "UI/Options/AudioTabWidget.h"
#include "UI/Options/OptionItemWidget.h"
#include "Core/Settings/GameOptionSettings.h"

#include "UI/Options/Data/OptionDataObject.h"
#include "UI/Options/Data/OptionDataScalar.h"
#include "UI/Options/Data/OptionDataBinary.h"
#include "UI/Options/Data/OptionDataDiscrete.h"

#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundMix.h"

/// 260318 추가
void UAudioTabWidget::NativeConstruct()
{
    Super::NativeConstruct();

    RefreshOptions();
}
void UAudioTabWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    RefreshOptions();
}
void UAudioTabWidget::RefreshOptions()
{
    if (!ContentBox) return;

    ContentBox->ClearChildren();

    UGameOptionSettings* Settings = UGameOptionSettings::Get();
    if (!Settings) return;

    AddOption(FName("OverallVolume"), FText::FromString(TEXT("전체 볼륨")), Settings->GetOverallVolume());
    AddSpacer();

    AddOption(FName("MusicVolume"), FText::FromString(TEXT("배경음")), Settings->GetMusicVolume());
    AddSpacer();

    AddOption(FName("SoundFXVolume"), FText::FromString(TEXT("효과음")), Settings->GetSoundFXVolume());
    AddSpacer();

    AddOption(FName("AllowBackgroundAudio"), FText::FromString(TEXT("백그라운드 재생")), Settings->GetAllowBackgroundAudio());
    AddSpacer();

    AddOption(FName("UseHDRAudioMode"), FText::FromString(TEXT("HDR 오디오")), Settings->GetUseHDRAudioMode());
}
void UAudioTabWidget::AddOption(FName ID, FText Name, float Default)
{
    UOptionDataScalar* NewData = NewObject<UOptionDataScalar>(this);
    NewData->DataID = ID;
    NewData->DisplayName = Name;
    NewData->Value = Default;

    TWeakObjectPtr<UAudioTabWidget> WeakThis(this);
    NewData->OnOptionValueChanged.AddLambda(
        [WeakThis, ID](UOptionDataObject* ChangedData) {
            if (!WeakThis.IsValid()) return;
            if (UOptionDataScalar* Scalar = Cast<UOptionDataScalar>(ChangedData)) {
                WeakThis->ApplyAudioVolume(ID, Scalar->Value);
            }
        });

    CreateAndAddWidget(NewData);
}
void UAudioTabWidget::AddOption(FName ID, FText Name, bool bDefault)
{
    UOptionDataBinary* NewData = NewObject<UOptionDataBinary>(this);
    NewData->DataID = ID;
    NewData->DisplayName = Name;
    NewData->bValue = bDefault;

    TWeakObjectPtr<UAudioTabWidget> WeakThis(this);

    NewData->OnOptionValueChanged.AddLambda(
        [WeakThis, ID](UOptionDataObject* ChangedData) {
            if (!WeakThis.IsValid()) return;
            if (UOptionDataBinary* Binary = Cast<UOptionDataBinary>(ChangedData)) {
                WeakThis->ApplyAudioBinarySetting(ID, Binary->bValue);
            }
        });

    CreateAndAddWidget(NewData);
}
void UAudioTabWidget::AddOption(FName ID, FText Name, int32 DefaultIndex, const TArray<FText>& Options)
{
    UOptionDataDiscrete* NewData = NewObject<UOptionDataDiscrete>(this);
    NewData->DataID = ID;
    NewData->DisplayName = Name;
    NewData->Index = DefaultIndex;
    NewData->Options = Options;

    CreateAndAddWidget(NewData);
}
void UAudioTabWidget::AddSpacer()
{
    if (!ContentBox) return;
    USpacer* NewSpacer = NewObject<USpacer>(this);
    if (NewSpacer) {
        NewSpacer->SetSize(FVector2D(1.f, Height));
        ContentBox->AddChild(NewSpacer);
    }
}
void UAudioTabWidget::CreateAndAddWidget(UOptionDataObject* Data)
{
    if (ScalarWidgetClass && ContentBox) {
        UOptionItemWidget* NewWidget = CreateWidget<UOptionItemWidget>(this, ScalarWidgetClass);
        if (NewWidget) {
            NewWidget->Setup(Data);
            ContentBox->AddChild(NewWidget);
        }
    }
}

void UAudioTabWidget::ApplyAudioVolume(FName ID, float NewValue)
{
    if (UGameOptionSettings* Settings = UGameOptionSettings::Get()) {
        if (ID == FName("MasterVolume")) Settings->SetOverallVolume(NewValue);
        else if (ID == FName("MusicVolume")) Settings->SetMusicVolume(NewValue);
        else if (ID == FName("SoundFXVolume")) Settings->SetSoundFXVolume(NewValue);

        Settings->SaveSettings();
    }

    TSoftObjectPtr<USoundClass> TargetClassPtr;
    if (ID == FName("MasterVolume")) TargetClassPtr = MasterSoundClass;
    else if (ID == FName("MusicVolume")) TargetClassPtr = MusicSoundClass;
    else if (ID == FName("SoundFXVolume")) TargetClassPtr = SFXSoundClass;

    if (TargetClassPtr.IsValid() && DefaultSoundMix.IsValid()) {
        USoundClass* TargetClass = TargetClassPtr.LoadSynchronous();
        USoundMix* MixAsset = DefaultSoundMix.LoadSynchronous();
        if (TargetClass && MixAsset) {
            UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MixAsset, TargetClass, NewValue, 1.f, 0.5f, true);
            UGameplayStatics::PushSoundMixModifier(GetWorld(), MixAsset);
        }
    }
}
void UAudioTabWidget::ApplyAudioBinarySetting(FName ID, bool bValue)
{
    if (UGameOptionSettings* Settings = UGameOptionSettings::Get()) {
        if (ID == FName("AllowBackgroundAudio")) Settings->SetAllowBackgroundAudio(bValue);
        else if (ID == FName("UseHDRAudioMode")) Settings->SetUseHDRAudioMode(bValue);

        Settings->SaveSettings();
    }
}