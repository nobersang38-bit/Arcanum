#pragma once

#include "CoreMinimal.h"
#include "Object/Object/StatusAction/StatusAction_PotionBase.h"
#include "StatusAction_PotionHealth.generated.h"

/**
 * 
 */
UCLASS()
class ARCANUM_API UStatusAction_PotionHealth : public UStatusAction_PotionBase
{
	GENERATED_BODY()
	
public:
	UStatusAction_PotionHealth();

	virtual void StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) override;
};
