// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BasementCombatComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Interface/TeamInterface.h"
#include "GameplayTags/ArcanumTags.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Object/Basement/Basement.h"

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
			//MaxHealth = BasementStat.CommandCenterHP.BaseValue;
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
	//BasementStat = InBasementStat;
	ABasement* OwnerBasement = Cast<ABasement>(GetOwner());
	if (OwnerBasement)
	{
		FGradeStatData GradeStatData;
		FRegenStat RegenStat;
		RegenStat.ParentTag = Arcanum::BattleStat::Character::Regen::Health::Root;
		RegenStat.BaseMax = InBasementStat.CommandCenterHP.GetTotalValue();
		RegenStat.Current = InBasementStat.CommandCenterHP.GetTotalValue();
		GradeStatData.RegenStats.Add(RegenStat);
		OwnerBasement->GetStatComponent()->SetData(GradeStatData);
	}
}