// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatusActionComponent.h"
#include "Data/DataAssets/DAAction.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"

// Sets default values for this component's properties
UStatusActionComponent::UStatusActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UStatusActionComponent::BeginPlay()
{
	Super::BeginPlay();
	UActorComponent* BattleStatActorComponent = GetOwner()->FindComponentByClass(UCharacterBattleStatsComponent::StaticClass());
	if (BattleStatActorComponent)
	{
		if (UCharacterBattleStatsComponent* BattleStatComponent = Cast<UCharacterBattleStatsComponent>(BattleStatActorComponent))
		{
			CachedCharacterBattleStatsComponent = BattleStatComponent;
		}
	}
	if (CachedCharacterBattleStatsComponent.IsValid())
	{
		// 리젠스탯 바인딩
		Actions.Empty();
		ActionDelegates.Empty();

		const TArray<FRegenStat>& RegenStats = CachedCharacterBattleStatsComponent->GetRegenStats();
		for (int i = 0; i < RegenStats.Num(); i++)
		{
			if (ActionSet.Contains(RegenStats[i].ParentTag))
			{
				UDAAction* ActionInstance = NewObject<UDAAction>(GetOwner(), *ActionSet.Find(RegenStats[i].ParentTag));
				Actions.Add(RegenStats[i].ParentTag, ActionInstance);
				if (FDelegateHandle* ActionDelegate = ActionDelegates.Find(RegenStats[i].ParentTag))
				{
					if (ActionDelegate->IsValid())
					{
						CachedCharacterBattleStatsComponent->OnCharacterRegenStatChanged.Remove(*ActionDelegate);
					}
				}
				else
				{
					ActionDelegates.Add(RegenStats[i].ParentTag, FDelegateHandle());
				}

				*ActionDelegates.Find(RegenStats[i].ParentTag) = CachedCharacterBattleStatsComponent->OnCharacterRegenStatChanged.AddUObject(ActionInstance, &UDAAction::StartAction);
				ActionInstance->StartAction(RegenStats[i]);
			}
		}

		const TArray<FNonRegenStat>& NonRegenStats = CachedCharacterBattleStatsComponent->GetNonRegenStats();
		for (int i = 0; i < NonRegenStats.Num(); i++)
		{
			if (ActionSet.Contains(NonRegenStats[i].TagName))
			{
				UDAAction* ActionInstance = NewObject<UDAAction>(this, *ActionSet.Find(NonRegenStats[i].TagName));
				Actions.Add(NonRegenStats[i].TagName, ActionInstance);
				if (FDelegateHandle* ActionDelegate = ActionDelegates.Find(NonRegenStats[i].TagName))
				{
					if (ActionDelegate->IsValid())
					{
						CachedCharacterBattleStatsComponent->OnCharacterNonRegenStatChanged.Remove(*ActionDelegate);
					}
				}
				else
				{
					ActionDelegates.Add(NonRegenStats[i].TagName, FDelegateHandle());
				}

				*ActionDelegates.Find(NonRegenStats[i].TagName) = CachedCharacterBattleStatsComponent->OnCharacterNonRegenStatChanged.AddUObject(ActionInstance, &UDAAction::StartAction);
				ActionInstance->StartAction(NonRegenStats[i]);
			}
		}
	}
}


// Called every frame
void UStatusActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

