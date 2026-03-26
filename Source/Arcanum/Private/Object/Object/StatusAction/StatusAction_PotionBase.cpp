#include "Object/Object/StatusAction/StatusAction_PotionBase.h"

UStatusAction_PotionBase::UStatusAction_PotionBase()
{
	EnableTagMergeType = EStatusActionTagType::OnlyStatusActionCompTags;
}

void UStatusAction_PotionBase::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	Super::StartAction_Implementation(RegenStat, NonRegenStat);
}
