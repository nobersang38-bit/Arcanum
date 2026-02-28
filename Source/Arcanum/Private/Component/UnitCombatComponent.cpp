// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/UnitCombatComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Interface/TeamInterface.h"
#include "GameplayTags/ArcanumTags.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Interface/RuntimeUnitDataInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Interface/CombatInterface.h"
#include "Character/BaseUnitCharacter.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Engine/OverlapResult.h"
#include "Object/Object/FSM/UnitStateBase.h"
#include "Object/Object/FSM/Unit/UnitState_Idle.h"
#include "Object/Object/FSM/Unit/UnitState_Move.h"
#include "Object/Object/FSM/Unit/UnitState_Attack.h"
#include "Object/Object/FSM/Unit/UnitState_HitReaction.h"
#include "Object/Object/FSM/Unit/UnitState_Death.h"

// Sets default values for this component's properties
UUnitCombatComponent::UUnitCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UUnitCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UUnitCombatComponent::DeferredBeginPlay);

	// Death바인딩
	if (ABaseUnitCharacter* TempOwner = Cast<ABaseUnitCharacter>(GetOwner()))
	{
		TempOwner->GetCharacterBattleStatsComponent()->OnCharacterRegenStatChanged.AddUObject(this, &UUnitCombatComponent::Death);
	}
}

void UUnitCombatComponent::DeferredBeginPlay()
{
	if (ACharacter* TempOwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter = TempOwnerCharacter;
		OwnerAIC = Cast<AAIController>(OwnerCharacter->GetController());
		OwnerCapsuleComponent = OwnerCharacter->GetCapsuleComponent();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("부모가 ACharacter가 아닙니다"));
		return;
	}

	// AI 세팅 가져오기
	if (GetOwner()->GetClass()->ImplementsInterface(UUnitDataInterface::StaticClass()))
	{
		auto Interface = Cast<IUnitDataInterface>(GetOwner());
		UnitAISetting = Interface->GetUnitData().Info.AISetting;
		UnitData = Interface->GetUnitData();
		TargetPriorityWeight = UnitAISetting.TargetPriorityWeightData;
		AIInitialize();
	}

	SetupStates();
	SetupTick();
}

void UUnitCombatComponent::SetupTick()
{
	float InitialDelay = FMath::FRandRange(UnitAISetting.AITickParams.InitialDelayRange.X, UnitAISetting.AITickParams.InitialDelayRange.Y);
	float UpdateInterval = FMath::FRandRange(UnitAISetting.AITickParams.UpdateIntervalRange.X, UnitAISetting.AITickParams.UpdateIntervalRange.Y);
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	// 틱 재생
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UUnitCombatComponent::TickUpdate, UpdateInterval, true, InitialDelay);
}

void UUnitCombatComponent::TickUpdate()
{
	if (CurrentUnitState.IsValid())
	{
		CurrentUnitState->OnTick(0.0f);
	}
}

void UUnitCombatComponent::AIInitialize()
{
	if (!OwnerAIC.IsValid()) return;

	// 비헤이비어트리 실행 및 블랙보드 키 가지고 있기
	if (!UnitAISetting.BehaviorTree.IsNull())
	{
		if (UBehaviorTree* BehaviorTree = UnitAISetting.BehaviorTree)
		{
			OwnerAIC->RunBehaviorTree(BehaviorTree);
			if (UBlackboardComponent* BBComp = OwnerAIC->GetBlackboardComponent())
			{
				if (!UnitAISetting.BBTargetActorName.IsNone())
				{
					TargetActorKey = BBComp->GetKeyID(UnitAISetting.BBTargetActorName);
				}
			}
		}
	}


	// 적 베이스, 아군 베이스 중 공격 대상 가지고 있기, 타겟으로 지정하기(공격하러 가기)
	if (UBattlefieldManagerSubsystem* BattlefieldManager = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		if (BattlefieldManager->GetBasement(AllyTeamTag) && BattlefieldManager->GetBasement(EnemyTeamTag) && GetOwner()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
		{
			auto Interface = Cast<ITeamInterface>(GetOwner());
			FGameplayTag MyTeamID = Interface->GetTeamTag();
			if (MyTeamID.MatchesTag(AllyTeamTag))
			{
				TargetBasement = BattlefieldManager->GetBasement(EnemyTeamTag);
			}
			else if (MyTeamID.MatchesTag(EnemyTeamTag))
			{
				TargetBasement = BattlefieldManager->GetBasement(AllyTeamTag);
			}
			TeamTag = MyTeamID;

			if (TargetBasement.IsValid())
			{
				TargetAssigned(TargetBasement.Get());
			}
		}
	}
}

void UUnitCombatComponent::TargetAssigned(AActor* Target)
{
	if (!Target) return;

	TargetActor = Target;

	if (OwnerAIC.IsValid())
	{
		if (UBlackboardComponent* BBComp = OwnerAIC->GetBlackboardComponent())
		{
			if (OwnerAIC.IsValid() && TargetActor != TargetActorBackup)
			{
				OwnerAIC->StopMovement();
				TargetActorBackup = TargetActor;
			}
			BBComp->SetValue<UBlackboardKeyType_Object>(TargetActorKey, TargetActor.Get());
		}
	}
}

void UUnitCombatComponent::OnBeginDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;					// 자신이면 제외
	if (!OtherActor->Implements<UTeamInterface>()) return;	// 팀 아이디 인터페이스가 없으면 제외

	// 같은 팀이면 제외
	if (OtherActor->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()) &&
		GetOwner()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
	{
		auto OtherInterface = Cast<ITeamInterface>(OtherActor);
		auto OwnerInterface = Cast<ITeamInterface>(GetOwner());

		if (OtherInterface->GetTeamTag() == OwnerInterface->GetTeamTag())
		{
			return;
		}
	}
	else
	{
		return;
	}

	// 캐릭터로 변환가능하면 추가
	if (DetectedActors.Num() < UnitAISetting.MaxTargetCount)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UUnitDataInterface::StaticClass()))
		{
			auto Interface = Cast<IUnitDataInterface>(OtherActor);
			if (Interface->GetUnitData().Info.AISetting.bIsElite)
			{
				UnitRuntimeData.Elites.Add(OtherActor);
			}
		}
		DetectedActors.Add(OtherActor);
	}
}

// 타겟 찾는 함수
void UUnitCombatComponent::SelectBestTarget(const TSet<TWeakObjectPtr<AActor>>& InDetectedCharacters)
{
	AActor* ResultTarget = nullptr;

	int32 WinScore = 0;

	for (const auto DetectedCharacter : InDetectedCharacters)
	{
		ResultTarget = GetHigherPriorityTarget(DetectedCharacter.Get(), ResultTarget, WinScore);
	}

	if (ResultTarget &&
		ResultTarget->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
	{
		if (TargetActor.IsValid() &&
			TargetActor->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
		{
			auto RevertInterface = Cast<IRuntimeUnitDataInterface>(TargetActor.Get());
			if (OwnerCharacter.IsValid())
			{
				RevertInterface->GetUnitRuntimeData().AttackingTargets.Remove(OwnerCharacter);
			}
		}

		auto Interface = Cast<IRuntimeUnitDataInterface>(ResultTarget);
		if (OwnerCharacter.IsValid())
		{
			Interface->GetUnitRuntimeData().AttackingTargets.Add(OwnerCharacter);
		}
		TargetActor = ResultTarget;
		TargetAssigned(TargetActor.Get());
	}
}

AActor* UUnitCombatComponent::GetHigherPriorityTarget(AActor* CurrentTarget, AActor* WinTarget, int32& WinScore)
{
	AActor* ResultCharacter = nullptr;
	float CurrentTargetScore = 0;

	if (OwnerCharacter.IsValid())
	{
		CurrentTargetScore = TargetPriorityWeight.CalculateScore(OwnerCharacter.Get(), CurrentTarget);
	}

	if (!WinTarget) // 처음실행이라면 무조건 CurrentTarget값 냅보내기
	{
		ResultCharacter = CurrentTarget;
		WinScore = CurrentTargetScore;
		return ResultCharacter;
	}

	if (WinScore < CurrentTargetScore)
	{
		WinScore = CurrentTargetScore;
		return CurrentTarget;
	}
	else
	{
		return WinTarget;
	}
}

void UUnitCombatComponent::StateChange(EUnitState InUnitState)
{
	if (UUnitStateBase* UnitState = UnitStates.FindRef(InUnitState))
	{
		if (CurrentUnitState.IsValid())
		{
			CurrentUnitState->OnExit();
		}
		CurrentUnitState = UnitState;

		CurrentUnitState->OnEnter(this);
	}
}

void UUnitCombatComponent::SetupStates()
{
	UnitStates.Empty();

	// Idle
	UUnitStateBase* State_Idle = NewObject<UUnitStateBase>(this, UUnitState_Idle::StaticClass());
	UnitStates.Add(EUnitState::Idle, Cast<UUnitStateBase>(State_Idle));

	// Move
	UUnitStateBase* State_Move = NewObject<UUnitStateBase>(this, UUnitState_Move::StaticClass());
	UnitStates.Add(EUnitState::Move, Cast<UUnitStateBase>(State_Move));

	// Attack
	UUnitStateBase* State_Attack = NewObject<UUnitStateBase>(this, UUnitState_Attack::StaticClass());
	UnitStates.Add(EUnitState::Attack, Cast<UUnitStateBase>(State_Attack));

	// HitReaction
	UUnitStateBase* State_HitReaction = NewObject<UUnitStateBase>(this, UUnitState_HitReaction::StaticClass());
	UnitStates.Add(EUnitState::HitReaction, Cast<UUnitStateBase>(State_HitReaction));

	// Death
	UUnitStateBase* State_Death = NewObject<UUnitStateBase>(this, UUnitState_Death::StaticClass());
	UnitStates.Add(EUnitState::Death, Cast<UUnitStateBase>(State_Death));

	StateChange(EUnitState::Idle);
}

void UUnitCombatComponent::LightHitReaction()
{

}

void UUnitCombatComponent::SendDamage(float InDamage)
{
	if (TargetActor.IsValid())
	{
		UGameplayStatics::ApplyDamage(TargetActor.Get(), InDamage, nullptr, GetOwner(), nullptr);
	}
}

void UUnitCombatComponent::Death(const FRegenStat& InData)
{
	if (!bIsDead && InData.ParentTag == Arcanum::BattleStat::Character::Regen::Health::Root && InData.Current <= 0.0f)
	{
		bIsDead = true;
		StateChange(EUnitState::Death);
		StateReset();
		UAnimMontage* DeathMontage = nullptr;
		int32 IDX = 0;
		if (OwnerCharacter.IsValid() && UnitData.Info.AnimSetting.Deads.Num() > 0)
		{
			IDX = FMath::RandRange(0, (UnitData.Info.AnimSetting.Deads.Num() - 1));
			DeathMontage = UnitData.Info.AnimSetting.Deads[IDX].DeadMontage;
			OwnerCharacter->PlayAnimMontage(DeathMontage);

			FTimerDelegate DeathTimerDelegate;
			DeathTimerDelegate.BindWeakLambda(this, [this]()
				{
					// Todo KDH 풀링 디액티브로 변경해야함
					this->GetOwner()->Destroy();
				});
			GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, DeathTimerDelegate, UnitData.Info.AnimSetting.Deads[IDX].DeactiveTime, false, UnitData.Info.AnimSetting.Deads[IDX].DeactiveTime);
		}
	}
	else return;
}

void UUnitCombatComponent::StateReset()
{
	if (OwnerCharacter.IsValid())
	{
		OwnerCharacter->StopAnimMontage();
	}
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	for (auto It = DetectedActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
	
}
