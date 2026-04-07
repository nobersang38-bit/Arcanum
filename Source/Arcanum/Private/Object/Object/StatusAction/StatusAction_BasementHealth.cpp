// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/StatusAction/StatusAction_BasementHealth.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Interface/TeamInterface.h"

UStatusAction_BasementHealth::UStatusAction_BasementHealth()
{
	EnableTagMergeType = EStatusActionTagType::OnlyStatusActionCompTags;
}

void UStatusAction_BasementHealth::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!BattleSubsystem) return;

	if (GetOuter()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
	{
		auto Interface = Cast<ITeamInterface>(GetOuter());
		FGameplayTag OwnerTag = Interface->GetTeamTag();

		if (OwnerTag == BattleSubsystem->AllyTeamTag)
		{
			BattleSubsystem->OnChangeAllyBaseHealth.Broadcast(RegenStat.Current, RegenStat.GetTotalMax());
		}
		else if (OwnerTag == BattleSubsystem->EnemyTeamTag)
		{
			BattleSubsystem->OnChangeEnemyBaseHealth.Broadcast(RegenStat.Current, RegenStat.GetTotalMax());
		}
	}

	if (RegenStat.Current <= 0.0f)
	{
		
		if (BattleSubsystem)
		{
			if (GetOuter()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
			{
				auto Interface = Cast<ITeamInterface>(GetOuter());
				FGameplayTag OwnerTag = Interface->GetTeamTag();

				if (OwnerTag.IsValid())
				{
					FMatchData MatchData;
					MatchData.CurrentMatchState = EMatchState::Ended;

					if (OwnerTag == BattleSubsystem->AllyTeamTag)
					{
						MatchData.bIsVictory = false;
					}
					else if (OwnerTag == BattleSubsystem->EnemyTeamTag)
					{
						MatchData.bIsVictory = true;
					}
					MatchData.EndTimeSecond = BattleSubsystem->GetCurrentMatchData().EndTimeSecond;
					BattleSubsystem->OnMatchEnded.Broadcast(MatchData);
				}
			}
		}
	}
}
