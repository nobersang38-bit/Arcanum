// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "StatusActionComponent.generated.h"

//김도현
class UStatusAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UStatusActionComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region 언리얼 엔진 기본 생성
public:
	// Sets default values for this component's properties
	UStatusActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma endregion

public:
	UFUNCTION()
	void SetupAction();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, TSubclassOf<UStatusAction>> ActionSet;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime|Debug", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, UStatusAction*> Instanced_Actions;

	TMap<FGameplayTag, FDelegateHandle> StartActionDelegates;
	TMap<FGameplayTag, FDelegateHandle> StopActionDelegates;

	UPROPERTY()
	TWeakObjectPtr<class UCharacterBattleStatsComponent> CachedCharacterBattleStatsComponent = nullptr;

//protected:
//	template<typename StatusActionClass, typename Stats, typename FindTagFunction, typename ActionStartFunction, typename ActionInitializeFunction>
//	void SetupStat(const Stats& InStat, FindTagFunction InFindTagFunction, ActionStartFunction InActionStartFunction, ActionInitializeFunction InActionInitializeFunction);
};

//template<typename StatusActionClass, typename Stats, typename FindTagFunction, typename ActionStartFunction, typename ActionInitializeFunction>
//inline void UStatusActionComponent::SetupStat(const Stats& InStat, FindTagFunction InFindTagFunction, ActionStartFunction InActionStartFunction, ActionInitializeFunction InActionInitializeFunction)
//{
//	for (int i = 0; i < InStat.Num(); i++)
//	{
//		FGameplayTag StatTag = InFindTagFunction(InStat[i]);
//		if (ActionSet.Contains(StatTag))
//		{
//			UClass* ActionClass = *ActionSet.Find(StatTag);
//			UStatusAction* ActionInstance = NewObject<UStatusAction>(GetOwner(), ActionClass);
//			Instanced_Actions.Add(StatTag, ActionInstance);
//
//			// 델리게이트들 초기화 및 추가
//			if (FDelegateHandle* StartActionDelegate = StartActionDelegates.Find(StatTag))
//			{
//				if (StartActionDelegate->IsValid())
//				{
//					CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(*StartActionDelegate);
//				}
//			}
//			else
//			{
//				StartActionDelegates.Add(StatTag, FDelegateHandle());
//			}
//
//			if (FDelegateHandle* StopActionDelegate = StopActionDelegates.Find(StatTag))
//			{
//				if (StopActionDelegate->IsValid())
//				{
//					CachedCharacterBattleStatsComponent->OnCharacterStatChanged.Remove(*StopActionDelegate);
//				}
//			}
//			else
//			{
//				StopActionDelegates.Add(StatTag, FDelegateHandle());
//			}
//
//			// 델리게이트 바인딩
//			*StartActionDelegates.Find(StatTag) = CachedCharacterBattleStatsComponent->OnCharacterStatChanged.AddUObject(ActionInstance, &UStatusAction::Internal_StartAction);
//			*StopActionDelegates.Find(StatTag) = CachedCharacterBattleStatsComponent->OnCharacterStatChanged.AddUObject(ActionInstance, &UStatusAction::Internal_StopAction);
//		
//			// 초기설정
//			switch (ActionInstance->EnableTagMergeType)
//			{
//			case EStatusActionTagType::OnlyThisTags:
//				ActionInstance->ChangeEnableTags(StatTag);
//				break;
//			case EStatusActionTagType::OnlyStatusActionCompTags:
//				break;
//			case EStatusActionTagType::AddTags:
//				ActionInstance->AddEnableTag(StatTag);
//				break;
//			default:
//				break;
//			}
//
//			switch (ActionInstance->DisableTagMergeType)
//			{
//			case EStatusActionTagType::OnlyThisTags:
//				ActionInstance->ChangeDisableTags(StatTag);
//				break;
//			case EStatusActionTagType::OnlyStatusActionCompTags:
//				break;
//			case EStatusActionTagType::AddTags:
//				ActionInstance->AddDisableTag(StatTag);
//				break;
//			default:
//				break;
//			}
//
//			// 초기화 함수 실행
//			InActionInitializeFunction(ActionInstance, InStat[i]);
//			//ActionInstance->Internal_InitializeAction(RegenStat[i], FNonRegenStat());
//
//			if (ActionInstance->bUseStartOnStartAction)
//			{
//				// 액션 함수 실행
//				InActionStartFunction(ActionInstance, InStat[i]);
//				//ActionInstance->Internal_StartAction(RegenStat[i], FNonRegenStat());
//			}
//		}
//	}
//}
