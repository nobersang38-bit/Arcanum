#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GachaProgressWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class ARCANUM_API UGachaProgressWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable) void UpdateGachaProgress(int32 CurrentCount, int32 MaxPity);
    
    //UPROPERTY(meta = (BindWidget)) class UImage* Image_Background;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* Text_Pickup;
    UPROPERTY(meta = (BindWidget)) class UTextBlock* Text_Pity;
};
