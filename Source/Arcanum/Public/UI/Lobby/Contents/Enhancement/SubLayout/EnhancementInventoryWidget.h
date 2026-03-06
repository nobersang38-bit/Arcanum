#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancementInventoryWidget.generated.h"

/**
 * 추영호
 * - 강화 인벤토리
 */
UCLASS()
class ARCANUM_API UEnhancementInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
};
