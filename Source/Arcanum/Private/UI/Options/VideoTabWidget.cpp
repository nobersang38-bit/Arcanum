#include "UI/Options/VideoTabWidget.h"
#include "UI/Options/OptionItemWidget.h"
#include "Core/Settings/GameOptionSettings.h"

#include "UI/Options/Data/OptionDataObject.h"
#include "UI/Options/Data/OptionDataScalar.h"
#include "UI/Options/Data/OptionDataBinary.h"
#include "UI/Options/Data/OptionDataDiscrete.h"

#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "Kismet/KismetSystemLibrary.h"

void UVideoTabWidget::NativeConstruct()
{
    Super::NativeConstruct();

    RefreshOptions();
}
void UVideoTabWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    RefreshOptions();
}
void UVideoTabWidget::RefreshOptions()
{
    if (!ContentBox) return;

    ContentBox->ClearChildren();

    // ==========================================
    // 1. Display Category (디스플레이)
    // ==========================================
    TArray<FText> WindowOptions = {
        FText::FromString(TEXT("전체 화면")),
        FText::FromString(TEXT("전체 창 모드")),
        FText::FromString(TEXT("창 모드"))
    };
    AddOption(FName("WindowMode"), FText::FromString(TEXT("화면 모드")), 1, WindowOptions);
    AddSpacer();
    TArray<FText> ResOptions = { FText::FromString(TEXT("1920 x 1080")), FText::FromString(TEXT("2560 x 1440")) };
    AddOption(FName("ScreenResolution"), FText::FromString(TEXT("해상도")), 0, ResOptions);
    AddSpacer();
    AddSpacer();

    // ==========================================
    // 2. Graphics Category (그래픽 품질)
    // ==========================================
    AddOption(FName("DisplayGamma"), FText::FromString(TEXT("밝기")), 0.5f);
    AddSpacer();
    TArray<FText> QualityLevels = {
        FText::FromString(TEXT("낮음")), FText::FromString(TEXT("보통")),
        FText::FromString(TEXT("높음")), FText::FromString(TEXT("에픽")), FText::FromString(TEXT("시네마틱"))
    };
    AddOption(FName("OverallQuality"), FText::FromString(TEXT("전체 품질")), 2, QualityLevels);
    AddSpacer();
    AddOption(FName("ResolutionScale"), FText::FromString(TEXT("3D 해상도")), 1.0f);
    AddSpacer();
    AddOption(FName("GlobalIlluminationQuality"), FText::FromString(TEXT("글로벌 일루미네이션")), 2, QualityLevels);
    AddSpacer();
    AddOption(FName("ShadowQuality"), FText::FromString(TEXT("그림자 품질")), 2, QualityLevels);
    AddSpacer();
    AddOption(FName("AntiAliasingQuality"), FText::FromString(TEXT("안티 에일리어싱")), 2, QualityLevels);
    AddSpacer();
    AddOption(FName("ViewDistanceQuality"), FText::FromString(TEXT("시야 거리")), 2, QualityLevels);
    AddSpacer();
    AddOption(FName("TextureQuality"), FText::FromString(TEXT("텍스처 품질")), 2, QualityLevels);
    AddSpacer();
    AddOption(FName("VisualEffectQuality"), FText::FromString(TEXT("이펙트 품질")), 2, QualityLevels);
    AddSpacer();
    AddOption(FName("ReflectionQuality"), FText::FromString(TEXT("반사 품질")), 2, QualityLevels);
    AddSpacer();
    AddOption(FName("PostProcessingQuality"), FText::FromString(TEXT("포스트 프로세싱")), 2, QualityLevels);
    AddSpacer();
    AddSpacer();

    // ==========================================
    // 3. Advanced Graphics (고급 설정)
    // ==========================================
    AddOption(FName("VerticalSync"), FText::FromString(TEXT("수직 동기화")), false);
    AddSpacer();
    TArray<FText> FrameOptions = {
        FText::FromString(TEXT("30 FPS")), FText::FromString(TEXT("60 FPS")),
        FText::FromString(TEXT("120 FPS")), FText::FromString(TEXT("제한 없음"))
    };
    AddOption(FName("FrameRateLimit"), FText::FromString(TEXT("프레임 제한")), 3, FrameOptions);
}
void UVideoTabWidget::AddOption(FName ID, FText Name, float Default)
{
    UOptionDataScalar* NewData = NewObject<UOptionDataScalar>(this);
    NewData->DataID = ID;
    NewData->DisplayName = Name;
    NewData->Value = Default;

    NewData->OnOptionValueChanged.AddLambda([this, ID](UOptionDataObject* ChangedData) {
        if (UOptionDataScalar* Scalar = Cast<UOptionDataScalar>(ChangedData)) {
            ApplyVideoSetting(ID, Scalar->Value);
        }
        });

    CreateAndAddWidget(NewData);
}
void UVideoTabWidget::AddOption(FName ID, FText Name, bool bDefault)
{
    UOptionDataBinary* NewData = NewObject<UOptionDataBinary>(this);
    NewData->DataID = ID;
    NewData->DisplayName = Name;
    NewData->bValue = bDefault;

    TWeakObjectPtr<UVideoTabWidget> WeakThis(this);
    NewData->OnOptionValueChanged.AddLambda(
        [WeakThis, ID](UOptionDataObject* InData) {
            if (!WeakThis.IsValid()) return;
            if (UOptionDataBinary* Binary = Cast<UOptionDataBinary>(InData)) {
                WeakThis->ApplyVideoSetting(ID, Binary->bValue ? 1 : 0);
            }
        });

    CreateAndAddWidget(NewData);
}
void UVideoTabWidget::AddOption(FName ID, FText Name, int32 DefaultIndex, const TArray<FText>& Options)
{
    UOptionDataDiscrete* NewData = NewObject<UOptionDataDiscrete>(this);
    NewData->DataID = ID;
    NewData->DisplayName = Name;
    NewData->Index = DefaultIndex;
    NewData->Options = Options;

    NewData->OnOptionValueChanged.AddLambda([this, ID](UOptionDataObject* InData) {
        if (UOptionDataDiscrete* Discrete = Cast<UOptionDataDiscrete>(InData)) {
            ApplyVideoSetting(ID, Discrete->Index);
        }});

    CreateAndAddWidget(NewData);
}
void UVideoTabWidget::AddSpacer()
{
    if (!ContentBox) return;
    USpacer* NewSpacer = NewObject<USpacer>(this);
    if (NewSpacer) {
        NewSpacer->SetSize(FVector2D(1.f, Height));
        ContentBox->AddChild(NewSpacer);
    }
}
void UVideoTabWidget::CreateAndAddWidget(UOptionDataObject* Data)
{
    if (ScalarWidgetClass && ContentBox) {
        UOptionItemWidget* NewWidget = CreateWidget<UOptionItemWidget>(this, ScalarWidgetClass);
        if (NewWidget) {
            NewWidget->Setup(Data);
            ContentBox->AddChild(NewWidget);
        }
    }
}
void UVideoTabWidget::ApplyVideoSetting(FName ID, float NewValue)
{
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (!Settings) return;

    // =========================
    // Display
    // =========================

    if (ID == "WindowMode") Settings->SetFullscreenMode((EWindowMode::Type)(int32)NewValue);
    else if (ID == "ScreenResolution") {
        switch ((int32)NewValue) {
        case 0: Settings->SetScreenResolution(FIntPoint(1920, 1080)); break;
        case 1: Settings->SetScreenResolution(FIntPoint(2560, 1440)); break;
        }
    }

    // =========================
    // Graphics
    // =========================

    else if (ID == "DisplayGamma") {
        if (UGameOptionSettings* Opt = UGameOptionSettings::Get()) Opt->SetCurrentDisplayGamma(NewValue);
    }
    else if (ID == "OverallQuality") Settings->SetOverallScalabilityLevel((int32)NewValue);
    else if (ID == "ResolutionScale") Settings->SetResolutionScaleNormalized(NewValue);
    else if (ID == "GlobalIlluminationQuality") Settings->SetGlobalIlluminationQuality((int32)NewValue);
    else if (ID == "ShadowQuality") Settings->SetShadowQuality((int32)NewValue);
    else if (ID == "AntiAliasingQuality") Settings->SetAntiAliasingQuality((int32)NewValue);
    else if (ID == "ViewDistanceQuality") Settings->SetViewDistanceQuality((int32)NewValue);
    else if (ID == "TextureQuality") Settings->SetTextureQuality((int32)NewValue);
    else if (ID == "VisualEffectQuality") Settings->SetVisualEffectQuality((int32)NewValue);
    else if (ID == "ReflectionQuality") Settings->SetReflectionQuality((int32)NewValue);
    else if (ID == "PostProcessingQuality") Settings->SetPostProcessingQuality((int32)NewValue);

    // =========================
    // Advanced
    // =========================

    else if (ID == "VerticalSync") Settings->SetVSyncEnabled(NewValue > 0.5f);
    else if (ID == "FrameRateLimit") {
        float Limit = 0.f;
        switch ((int32)NewValue) {
        case 0: Limit = 30.f; break;
        case 1: Limit = 60.f; break;
        case 2: Limit = 120.f; break;
        case 3: Limit = 0.f; break;
        }

        Settings->SetFrameRateLimit(Limit);
    }

    // =========================
    // 적용
    // =========================

    Settings->ApplySettings(false);
    Settings->SaveSettings();
}