#include "Object/Object/StatusAction/StatusAction_PotionMana.h"

UStatusAction_PotionMana::UStatusAction_PotionMana()
{
	EnableTagMergeType = EStatusActionTagType::OnlyStatusActionCompTags;
}

void UStatusAction_PotionMana::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	Super::StartAction_Implementation(RegenStat, NonRegenStat);
}
