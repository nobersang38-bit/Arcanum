#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleBuffSlotWidget.generated.h"

class UImage;
class UMaterialInstanceDynamic;

UCLASS()
class ARCANUM_API UBattleBuffSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
	/* 아이콘 */
	void SetBuffIcon(UTexture2D* InIcon);

	/* 지속시간 */
	void SetDurationPercent(float InPercent);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> BuffIcon = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> BuffDuration = nullptr;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> BuffDurationMID = nullptr;
};
