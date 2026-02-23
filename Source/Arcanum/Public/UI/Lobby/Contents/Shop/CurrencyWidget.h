#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrencyWidget.generated.h"

class UTextBlock;

/**
 * 추영호
 * - 재화 UI
 */

UCLASS()
class ARCANUM_API UCurrencyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

#pragma region 재화 표시
protected:
	/* 재화 표시 갱신 */
	UFUNCTION()
	void RefreshCurrencyUI();

protected:
	/** 골드 텍스트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldText;

	/** 조각 텍스트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ShardText;

	/** 소울 텍스트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SoulText;
#pragma endregion
};

