#include "Object/Object/StatusAction/StatusAction_PotionHealth.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"

UStatusAction_PotionHealth::UStatusAction_PotionHealth()
{
	EnableTagMergeType = EStatusActionTagType::OnlyStatusActionCompTags;
}

void UStatusAction_PotionHealth::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	Super::StartAction_Implementation(RegenStat, NonRegenStat);

	if (AActor* ownerActor = GetTypedOuter<AActor>())
	{
		if (UCharacterBattleStatsComponent* battleStatsComponent = ownerActor->FindComponentByClass<UCharacterBattleStatsComponent>())
		{
			battleStatsComponent->ChangeStatValue(Arcanum::BattleStat::Character::Regen::Health::Root, 5.0f, ownerActor);
		}
	}
}
