#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopHUDWidget.generated.h"

class UCurrencyWidget;

/**
 * 추영호
 * - 상점 UI
 */

UCLASS()
class ARCANUM_API UShopHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TObjectPtr<class UCurrencyWidget> CurrencyWidget;
	
};
