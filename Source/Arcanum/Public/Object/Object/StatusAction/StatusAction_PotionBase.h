#pragma once

#include "CoreMinimal.h"
#include "Object/Object/StatusAction/StatusAction.h"
#include "StatusAction_PotionBase.generated.h"

/**
 * 
 */
UCLASS()
class ARCANUM_API UStatusAction_PotionBase : public UStatusAction
{
	GENERATED_BODY()

public:
	UStatusAction_PotionBase();

	virtual void StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
	FGameplayTag PotionTag;
};
