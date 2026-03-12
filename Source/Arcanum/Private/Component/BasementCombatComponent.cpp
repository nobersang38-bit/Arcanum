// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BasementCombatComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Interface/TeamInterface.h"
#include "GameplayTags/ArcanumTags.h"

// Sets default values for this component's properties
UBasementCombatComponent::UBasementCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBasementCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &UBasementCombatComponent::RecievedDamage);
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UBasementCombatComponent::RecievedDamage);

	if (GetOwner()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
	{
		auto Interface = Cast<ITeamInterface>(GetOwner());
		FGameplayTag Tag = Interface->GetTeamTag();
		if (UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
		{
			if (Tag == BattleSubsystem->AllyTeamTag)
			{
				SetBasementStat(BattleSubsystem->GetAllyBasementStat());
			}
			else
			{
				SetBasementStat(BattleSubsystem->GetEnemyBasementStat());
			}
			MaxHealth = BasementStat.CommandCenterHP.BaseValue;
		}
	}
	
}


// Called every frame
void UBasementCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBasementCombatComponent::SetBasementStat(const FEnemyBasement& InBasementStat)
{
	BasementStat = InBasementStat;
}

void UBasementCombatComponent::RecievedDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	float Health = BasementStat.CommandCenterHP.GetTotalValue();
	BasementStat.CommandCenterHP.BaseValue = FMath::Clamp(Health - Damage, 0, FLT_MAX);
	OnBasementChangeHealth.Broadcast(BasementStat.CommandCenterHP.GetBaseValue(), MaxHealth);

	if (BasementStat.CommandCenterHP.GetTotalValue() <= 0.0f)
	{
		UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		if (BattleSubsystem)
		{
			if (GetOwner()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
			{
				auto Interface = Cast<ITeamInterface>(GetOwner());
				FGameplayTag OwnerTag = Interface->GetTeamTag();

				if (OwnerTag.IsValid())
				{
					FMatchData MatchData;
					MatchData.CurrentMatchState = EMatchState::Ended;

					if (OwnerTag == BattleSubsystem->AllyTeamTag)
					{
						MatchData.bIsVictory = false;
					}
					else if(OwnerTag == BattleSubsystem->EnemyTeamTag)
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

