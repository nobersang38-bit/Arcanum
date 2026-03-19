#include "Object/Object/StatusAction/StatusAction_SetBonus.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"

UStatusAction_SetBonus::UStatusAction_SetBonus()
{
}

void UStatusAction_SetBonus::InitializeAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	Super::InitializeAction_Implementation(RegenStat, NonRegenStat);

	if (UWorld* world = GetWorld())
	{
		if (UBattlefieldManagerSubsystem* battleSubsystem = world->GetSubsystem<UBattlefieldManagerSubsystem>())
		{

			SetBonusTag = battleSubsystem->GetEquippedSetBonusTag();
			bIsBonusActive = false;

			ChangeEnableTags(Arcanum::BattleStat::Character::Regen::Health::Root);
			ChangeDisableTags(Arcanum::BattleStat::Character::Regen::Health::Root);
		}
	}
}


void UStatusAction_SetBonus::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
}

void UStatusAction_SetBonus::StopAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
}
