#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTags/ArcanumTags.h"
#include "CommonBtnWidget.generated.h"

/** 버튼 타입(해당 클래스에만 사용)*/
UENUM(BlueprintType)
enum class EButtonType : uint8
{
    TextOnly    UMETA(DisplayName = "Text Only"),
    IconOnly    UMETA(DisplayName = "Icon Only"),
    TextAndIcon UMETA(DisplayName = "Text and Icon")
};

// 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCommonButtonClicked);

UCLASS()
class ARCANUM_API UCommonBtnWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 1. 초기 설정 데이터 (텍스트는 자주 바뀌므로 유지)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings", meta = (ExposeOnSpawn = "true"))
    FText DisplayText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings", meta = (ExposeOnSpawn = "true"))
    class UTexture2D* NormalTexture;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings", meta = (ExposeOnSpawn = "true"))
    class UTexture2D* HoverTexture;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings", meta = (ExposeOnSpawn = "true"))
    class UTexture2D* ClickTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings")
    EButtonType ButtonType = EButtonType::TextAndIcon;

    // 이 버튼이 어떤 기능을 수행할지 식별하는 태그
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings")
    FGameplayTag ButtonTargetTag;

    // 클릭 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCommonButtonClicked OnClicked;

    // --- 블루프린트 호출 함수 ---
    UFUNCTION(BlueprintCallable, Category = "Button Settings")
    void SetButtonAppearance(FText InText, UTexture2D* InIcon, EButtonType InType);

    UFUNCTION(BlueprintCallable, Category = "Button Settings")
    void SetButtonText(FText InText);

    UFUNCTION(BlueprintCallable, Category = "Button Settings")
    void SetIconOpacity(float InOpacity);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Button Settings")
    void SetSelectedState(bool bIsSelected);
    virtual void SetSelectedState_Implementation(bool bIsSelected);
   
    // --- 위젯 바인딩 (WBP의 컴포넌트와 이름 일치 필수) ---
    UPROPERTY(meta = (BindWidget))
    class UButton* MainButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ButtonLabel;

    UPROPERTY(meta = (BindWidget))
    class UImage* BackGroundImg;

    UPROPERTY(meta = (BindWidget))
    class UButton* IconButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, ShowOnlyInnerProperties), Category = "Button Settings")
    class UCommonButtonBase* HQUI_Button;

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

    // 에디터에서 배경 고정 등을 강제하기 위함
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

    // 비주얼 업데이트 (런타임/에디터 공용)
    void UpdateVisuals();
    void ApplyButtonStyle(class UButton* InButton, class UTexture2D* Normal, class UTexture2D* Hover, class UTexture2D* Click);

    UFUNCTION()
    void HandleButtonClicked();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button State")
    bool bIsSelectedState;
};
