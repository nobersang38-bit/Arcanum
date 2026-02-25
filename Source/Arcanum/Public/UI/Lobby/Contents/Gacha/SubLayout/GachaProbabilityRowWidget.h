#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GachaProbabilityRowWidget.generated.h"

class UImage;
class UTextBlock;
class UHorizontalBox;

UCLASS()
class ARCANUM_API UGachaProbabilityRowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void SetAsHeader(FText GradeName, float Probability, FLinearColor HeaderColor);
    void SetAsCharacter(UTexture2D* Icon, FText Name, float Probability, bool bIsPickup);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> Background;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> CharacterIcon;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> DisplayName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ProbabilityText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> PickupBadge;
};
