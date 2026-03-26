#pragma once

#include "CoreMinimal.h"
#include "Object/Object/StatusAction/StatusAction_PotionBase.h"
#include "StatusAction_PotionMana.generated.h"

UCLASS()
class ARCANUM_API UStatusAction_PotionMana : public UStatusAction_PotionBase
{
	GENERATED_BODY()

public:
	UStatusAction_PotionMana();

	virtual void StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) override;
};