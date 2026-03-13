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
	UCharacterBattleStatsComponent* BattleStatComponent = GetOwner()->FindComponentByClass<UCharacterBattleStatsComponent>();
	if (BattleStatComponent)
	{
		CachedCharacterBattleStatsComponent = BattleStatComponent;
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

		for (auto& ActionDelegate : StartActionDelegates)
		{
			if (ActionDelegate.Value.IsValid())
			{
				CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(ActionDelegate.Value);
			}
		}

		Instanced_Actions.Empty();
		StartActionDelegates.Empty();
		StopActionDelegates.Empty();

		const TArray<FRegenStat>& RegenStat = CachedCharacterBattleStatsComponent->GetRegenStats();
		//SetupStat<UStatusAction>(RegenStat,
		//	[](const FRegenStat& InStat)
		//	{
		//		return InStat.ParentTag;
		//	},
		//	[](UStatusAction* InStatusActionInst, const FRegenStat& InRegenStat)
		//	{
		//		InStatusActionInst->Internal_StartAction(InRegenStat, FNonRegenStat());
		//	},
		//	[](UStatusAction* InStatusActionInst, const FRegenStat& InRegenStat)
		//	{
		//		InStatusActionInst->Internal_InitializeAction(InRegenStat, FNonRegenStat());
		//	}
		//);

		for (int i = 0; i < RegenStat.Num(); i++)
		{
			if (ActionSet.Contains(RegenStat[i].ParentTag))
			{
				UClass* ActionClass = *ActionSet.Find(RegenStat[i].ParentTag);
				UStatusAction* ActionInstance = NewObject<UStatusAction>(GetOwner(), ActionClass);
				Instanced_Actions.Add(RegenStat[i].ParentTag, ActionInstance);

				// 델리게이트들 초기화 및 추가
				if (FDelegateHandle* StartActionDelegate = StartActionDelegates.Find(RegenStat[i].ParentTag))
				{
					if (StartActionDelegate->IsValid())
					{
						CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(*StartActionDelegate);
					}
				}
				else
				{
					StartActionDelegates.Add(RegenStat[i].ParentTag, FDelegateHandle());
				}

				if (FDelegateHandle* StopActionDelegate = StopActionDelegates.Find(RegenStat[i].ParentTag))
				{
					if (StopActionDelegate->IsValid())
					{
						CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(*StopActionDelegate);
					}
				}
				else
				{
					StopActionDelegates.Add(RegenStat[i].ParentTag, FDelegateHandle());
				}

				// 델리게이트 바인딩
				*StartActionDelegates.Find(RegenStat[i].ParentTag) = CachedCharacterBattleStatsComponent->OnCharacterStatChanged.AddUObject(ActionInstance, &UStatusAction::Internal_StartAction);
				*StopActionDelegates.Find(RegenStat[i].ParentTag) = CachedCharacterBattleStatsComponent->OnCharacterStatChanged.AddUObject(ActionInstance, &UStatusAction::Internal_StopAction);
				
				// 초기설정
				switch (ActionInstance->EnableTagMergeType)
				{
				case EStatusActionTagType::OnlyThisTags:
					break;
				case EStatusActionTagType::OnlyStatusActionCompTags:
					ActionInstance->ChangeEnableTags(RegenStat[i].ParentTag);
					break;
				case EStatusActionTagType::AddTags:
					ActionInstance->AddEnableTag(RegenStat[i].ParentTag);
					break;
				default:
					break;
				}

				switch (ActionInstance->DisableTagMergeType)
				{
				case EStatusActionTagType::OnlyThisTags:
					break;
				case EStatusActionTagType::OnlyStatusActionCompTags:
					ActionInstance->ChangeDisableTags(RegenStat[i].ParentTag);
					break;
				case EStatusActionTagType::AddTags:
					ActionInstance->AddDisableTag(RegenStat[i].ParentTag);
					break;
				default:
					break;
				}

				// 초기화 함수 실행
				ActionInstance->Internal_InitializeAction(RegenStat[i], FNonRegenStat());

				if (ActionInstance->bUseStartOnStartAction)
				{
					// 액션 함수 실행
					ActionInstance->Internal_StartAction(RegenStat[i], FNonRegenStat());
				}
			}
		}

		const TArray<FNonRegenStat>& NonRegenStats = CachedCharacterBattleStatsComponent->GetNonRegenStats();
		for (int i = 0; i < NonRegenStats.Num(); i++)
		{
			if (ActionSet.Contains(NonRegenStats[i].TagName))
			{
				UClass* ActionClass = *ActionSet.Find(NonRegenStats[i].TagName);
				UStatusAction* ActionInstance = NewObject<UStatusAction>(GetOwner(), ActionClass);
				Instanced_Actions.Add(NonRegenStats[i].TagName, ActionInstance);

				// 델리게이트들 초기화 및 추가
				if (FDelegateHandle* StartActionDelegate = StartActionDelegates.Find(NonRegenStats[i].TagName))
				{
					if (StartActionDelegate->IsValid())
					{
						CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(*StartActionDelegate);
					}
				}
				else
				{
					StartActionDelegates.Add(NonRegenStats[i].TagName, FDelegateHandle());
				}

				if (FDelegateHandle* StopActionDelegate = StopActionDelegates.Find(NonRegenStats[i].TagName))
				{
					if (StopActionDelegate->IsValid())
					{
						CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(*StopActionDelegate);
					}
				}
				else
				{
					StopActionDelegates.Add(NonRegenStats[i].TagName, FDelegateHandle());
				}

				// 델리게이트 바인딩
				*StartActionDelegates.Find(NonRegenStats[i].TagName) = CachedCharacterBattleStatsComponent->OnCharacterStatChanged.AddUObject(ActionInstance, &UStatusAction::Internal_StartAction);
				*StopActionDelegates.Find(NonRegenStats[i].TagName) = CachedCharacterBattleStatsComponent->OnCharacterStatChanged.AddUObject(ActionInstance, &UStatusAction::Internal_StopAction);

				// 초기설정
				switch (ActionInstance->EnableTagMergeType)
				{
				case EStatusActionTagType::OnlyThisTags:
					break;
				case EStatusActionTagType::OnlyStatusActionCompTags:
					ActionInstance->ChangeEnableTags(NonRegenStats[i].TagName);
					break;
				case EStatusActionTagType::AddTags:
					ActionInstance->AddEnableTag(NonRegenStats[i].TagName);
					break;
				default:
					break;
				}

				switch (ActionInstance->DisableTagMergeType)
				{
				case EStatusActionTagType::OnlyThisTags:
					break;
				case EStatusActionTagType::OnlyStatusActionCompTags:
					ActionInstance->ChangeDisableTags(NonRegenStats[i].TagName);
					break;
				case EStatusActionTagType::AddTags:
					ActionInstance->AddDisableTag(NonRegenStats[i].TagName);
					break;
				default:
					break;
				}

				// 초기화 함수 실행
				ActionInstance->Internal_InitializeAction(FRegenStat(), NonRegenStats[i]);

				if (ActionInstance->bUseStartOnStartAction)
				{
					// 액션 함수 실행
					ActionInstance->Internal_StartAction(FRegenStat(), NonRegenStats[i]);
				}
			}
		}
	}
}

