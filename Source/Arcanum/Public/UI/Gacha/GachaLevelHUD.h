#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GachaLevelHUD.generated.h"

class UButton;

UCLASS()
class ARCANUM_API UGachaLevelHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget)) UButton* SkipButton;

	UFUNCTION() void OnSkipClicked();
};
