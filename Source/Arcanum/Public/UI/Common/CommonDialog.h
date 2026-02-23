#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "CommonDialog.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogResult, EDialogResult, Result);

UCLASS()
class ARCANUM_API UCommonDialog : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintAssignable)
    FOnDialogResult OnResult;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDialogType DialogType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText TitleTxt;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText MessageTxt;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText OKBtnTxt;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText CancelBtnTxt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> TitleTexture;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> MessageTexture;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UTexture2D> ButtonTexture;

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> TitleImg;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TitleText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> MessageImg;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MessageText;


    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OKButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> OKButtonText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> CancelButton;
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CancelButtonText;

    UFUNCTION()
    void OnOKClicked();

    UFUNCTION()
    void OnCancelClicked();
};
