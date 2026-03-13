// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatusActionComponent.h"
#include "Object/Object/StatusAction/StatusAction.h"
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
	SetupAction();
}


// Called every frame
void UStatusActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatusActionComponent::SetupAction()
{
	if (CachedCharacterBattleStatsComponent.IsValid())
	{
		// 리젠스탯 바인딩

		for (auto& ActionDelegate : ActionDelegates)
		{
			if (ActionDelegate.Value.IsValid())
			{
				CachedCharacterBattleStatsComponent->OnCharacterRegenStatChanged.Remove(ActionDelegate.Value);
				CachedCharacterBattleStatsComponent->OnCharacterNonRegenStatChanged.Remove(ActionDelegate.Value);
			}
		}

		for (auto& Action : Actions)
		{
			if (Action.Value)
			{
				Action.Value->MarkAsGarbage();
			}
		}

		Actions.Empty();
		ActionDelegates.Empty();

		const TArray<FRegenStat>& RegenStat = CachedCharacterBattleStatsComponent->GetRegenStats();
		for (int i = 0; i < RegenStat.Num(); i++)
		{
			if (ActionSet.Contains(RegenStat[i].ParentTag))
			{
				UClass* ActionClass = *ActionSet.Find(RegenStat[i].ParentTag);
				UStatusAction* ActionInstance = NewObject<UStatusAction>(GetOwner(), ActionClass);
				Actions.Add(RegenStat[i].ParentTag, ActionInstance);
				if (FDelegateHandle* ActionDelegate = ActionDelegates.Find(RegenStat[i].ParentTag))
				{
					if (ActionDelegate->IsValid())
					{
						CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(*ActionDelegate);
					}
				}
				else
				{
					ActionDelegates.Add(RegenStat[i].ParentTag, FDelegateHandle());
				}
				*ActionDelegates.Find(RegenStat[i].ParentTag) = CachedCharacterBattleStatsComponent->OnCharacterStatChanged.AddUObject(ActionInstance, &UStatusAction::StartAction);
				ActionInstance->StartAction(RegenStat[i], FNonRegenStat());
				ActionInstance->AddEnableTag(RegenStat[i].ParentTag);
			}
		}

		const TArray<FNonRegenStat>& NonRegenStats = CachedCharacterBattleStatsComponent->GetNonRegenStats();
		for (int i = 0; i < NonRegenStats.Num(); i++)
		{
			if (ActionSet.Contains(NonRegenStats[i].TagName))
			{
				UClass* ActionClass = *ActionSet.Find(NonRegenStats[i].TagName);
				UStatusAction* ActionInstance = NewObject<UStatusAction>(GetOwner(), ActionClass);
				Actions.Add(NonRegenStats[i].TagName, ActionInstance);
				if (FDelegateHandle* ActionDelegate = ActionDelegates.Find(NonRegenStats[i].TagName))
				{
					if (ActionDelegate->IsValid())
					{
						CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(*ActionDelegate);
					}
				}
				else
				{
					ActionDelegates.Add(NonRegenStats[i].TagName, FDelegateHandle());
				}
				*ActionDelegates.Find(NonRegenStats[i].TagName) = CachedCharacterBattleStatsComponent->OnCharacterStatChanged.AddUObject(ActionInstance, &UStatusAction::StartAction);
				ActionInstance->StartAction(FRegenStat(), NonRegenStats[i]);
				ActionInstance->AddEnableTag(NonRegenStats[i].TagName);
			}
		}
	}
}

