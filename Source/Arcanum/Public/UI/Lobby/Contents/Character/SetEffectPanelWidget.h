#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SetEffectPanelWidget.generated.h"

class UTextBlock;

USTRUCT(BlueprintType)
struct FSetEffectViewData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SetNameText;

	FText SetCountText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SetDescText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActivated = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVisible = false;
};

/**
 * 추영호
 * 세트 효과 발동 여부 패널
 */

UCLASS()
class ARCANUM_API USetEffectPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/* 세트 효과 텍스트 적용 */
	void SetEffectText(const FSetEffectViewData& InViewData);

	/* 세트 효과 텍스트 초기화 */
	void ClearEffectText();

protected:
	/* 세트 이름 텍스트 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SetNameText;

	/* 세트 개수 텍스트 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SetCountText;

	/* 세트 효과 설명 텍스트 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SetDescText;

	FSlateColor DefaultSetNameColor;
	FSlateColor DefaultSetCountColor;
	FSlateFontInfo DefaultSetCountFont;
	FSlateColor DefaultSetDescColor;
	FSlateFontInfo DefaultSetNameFont;
	FSlateFontInfo DefaultSetDescFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetEffect")
	FSlateColor ActivatedColor = FSlateColor(FLinearColor::Green);
};
