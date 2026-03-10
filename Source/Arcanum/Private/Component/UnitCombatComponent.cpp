// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/UnitCombatComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
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
#include "Interface/RuntimeUnitDataInterface.h"
#include "Core/SubSystem/PoolingSubsystem.h"
#include "Data/DataAssets/Actions/DAAction_MoveSpeed.h"
#include "Object/Actor/FloatingDamageText.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
UUnitCombatComponent::UUnitCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UUnitCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UUnitCombatComponent::DeferredBeginPlay);
}


// ========================================================
// 액션
// ========================================================
void UUnitCombatComponent::SendDamage(float InDamage)
{
	if (TargetActor.IsValid() && IsCanAttackRange())
	{
		UGameplayStatics::ApplyDamage(TargetActor.Get(), InDamage, nullptr, GetOwner(), nullptr);
	}
}

void UUnitCombatComponent::UnitActivate()
{
	DeferredBeginPlay();

	OwnerCharacter->GetMesh()->SetHiddenInGame(false);
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UUnitCombatComponent::UnitDeactive()
{
	OwnerCharacter->GetMesh()->SetHiddenInGame(true);
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OwnerCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
	OwnerAIC->GetBrainComponent()->StopLogic(TEXT("비활성화!!"));
}


// ========================================================
// 생명 주기
// ========================================================
void UUnitCombatComponent::DeferredBeginPlay()
{
	bIsDead = false;

	UCharacterBattleStatsComponent* StatComponent = nullptr;
	// 스탯 바인딩
	if (ABaseUnitCharacter* TempOwner = Cast<ABaseUnitCharacter>(GetOwner()))
	{
		StatComponent = TempOwner->GetCharacterBattleStatsComponent();
		StatComponent->OnCharacterRegenStatChanged.RemoveAll(this);
		StatComponent->OnCharacterNonRegenStatChanged.RemoveAll(this);
		StatComponent->OnCharacterRegenStatChanged.AddUObject(this, &UUnitCombatComponent::Death);
		StatComponent->OnCharacterRegenStatChanged.AddUObject(this, &UUnitCombatComponent::SetRegenStat);
		StatComponent->OnCharacterNonRegenStatChanged.AddUObject(this, &UUnitCombatComponent::SetNonRegenStat);
	}

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
		UnitData = Interface->GetUnitData();
		AIInitialize();
	}

	SetupStates();
	SetupTick();
	if (StatComponent)
	{
		StatComponent->BroadcastAllStats();
	}
}

void UUnitCombatComponent::TickUpdate()
{
	if (CurrentUnitState.IsValid())
	{
		CurrentUnitState->OnTick(0.0f);
	}
}


// ========================================================
// 초기설정
// ========================================================
void UUnitCombatComponent::AIInitialize()
{
	if (!OwnerAIC.IsValid()) return;

	// 비헤이비어트리 실행 및 블랙보드 키 가지고 있기
	if (!UnitData.Info.AISetting.BehaviorTree.IsNull())
	{
		if (UBehaviorTree* BehaviorTree = UnitData.Info.AISetting.BehaviorTree)
		{
			OwnerAIC->RunBehaviorTree(BehaviorTree);
			if (UBlackboardComponent* BBComp = OwnerAIC->GetBlackboardComponent())
			{
				if (!UnitData.Info.AISetting.BBTargetActorName.IsNone())
				{
					TargetActorKey = BBComp->GetKeyID(UnitData.Info.AISetting.BBTargetActorName);
				}

				if (!UnitData.Info.AISetting.BBAttackRangeName.IsNone())
				{
					AttackRangeKey = BBComp->GetKeyID(UnitData.Info.AISetting.BBAttackRangeName);
					BBComp->SetValue<UBlackboardKeyType_Float>(AttackRangeKey, UnitData.Info.AISetting.AttackRange);
					BBComp->SetValue<UBlackboardKeyType_Float>(BBComp->GetKeyID(FName("MoveRange")), UnitData.Info.AISetting.AttackRange - 10.0f);
				}
			}
		}
	}


	// 적 베이스, 아군 베이스 중 공격 대상 가지고 있기, 타겟으로 지정하기
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

void UUnitCombatComponent::SetupTick()
{
	float InitialDelay = FMath::FRandRange(UnitData.Info.AISetting.AITickParams.InitialDelayRange.X, UnitData.Info.AISetting.AITickParams.InitialDelayRange.Y);
	float UpdateInterval = FMath::FRandRange(UnitData.Info.AISetting.AITickParams.UpdateIntervalRange.X, UnitData.Info.AISetting.AITickParams.UpdateIntervalRange.Y);
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	// 틱 재생
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UUnitCombatComponent::TickUpdate, UpdateInterval, true, InitialDelay);
}

void UUnitCombatComponent::SetupStates()
{
	if (!UnitStates.IsEmpty())
	{
		StateChange(EUnitState::Idle);
		return;
	}

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


// ========================================================
// 헬퍼
// ========================================================
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

void UUnitCombatComponent::EnemyFind()
{
	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape MySphere = FCollisionShape::MakeSphere(UnitData.Info.AISetting.TargetPriorityWeightData.GetDetectDistance());
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	// 특정 위치(GetActorLocation)에서 한 프레임 즉시 검사
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OutOverlaps,
		GetOwner()->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn, // 설정한 채널
		MySphere,
		Params
	);

	DetectedActors.Empty();

	for (int i = 0; i < OutOverlaps.Num(); i++)
	{
		if (!OutOverlaps[i].GetActor()) continue;
		if (OutOverlaps[i].GetActor() == GetOwner()) continue;					// 자신이면 제외
		if (!OutOverlaps[i].GetActor()->Implements<UTeamInterface>()) continue;	// 팀 아이디 인터페이스가 없으면 제외


		if (OutOverlaps[i].GetActor()->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
		{
			auto Interface = Cast<IRuntimeUnitDataInterface>(OutOverlaps[i].GetActor());
			if (Interface && Interface->GetIsDead())
			{
				continue;
			}
		}

		// 같은 팀이면 제외
		if (OutOverlaps[i].GetActor()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()) &&
			GetOwner()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
		{
			auto OtherInterface = Cast<ITeamInterface>(OutOverlaps[i].GetActor());
			auto OwnerInterface = Cast<ITeamInterface>(GetOwner());

			if (OtherInterface->GetTeamTag() == OwnerInterface->GetTeamTag())
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		if (DetectedActors.Num() < UnitData.Info.AISetting.MaxTargetCount)
		{
			if (OutOverlaps[i].GetActor()->GetClass()->ImplementsInterface(UUnitDataInterface::StaticClass()))
			{
				auto Interface = Cast<IUnitDataInterface>(OutOverlaps[i].GetActor());
				if (Interface->GetUnitData().Info.AISetting.bIsElite)
				{
					UnitRuntimeData.Elites.Add(OutOverlaps[i].GetActor());
				}
			}
			DetectedActors.Add(OutOverlaps[i].GetActor());
		}
	}
}

void UUnitCombatComponent::TargetAssigned(AActor* Target)
{
	TargetActor = Target;
}

void UUnitCombatComponent::MoveToTarget(AActor* Target)
{
	if (OwnerAIC.IsValid())
	{
		if (UBlackboardComponent* BBComp = OwnerAIC->GetBlackboardComponent())
		{
			if (BBComp->GetValue<UBlackboardKeyType_Object>(TargetActorKey) == Target) return;
			BBComp->SetValue<UBlackboardKeyType_Object>(TargetActorKey, nullptr);
			BBComp->SetValue<UBlackboardKeyType_Object>(TargetActorKey, Target);
		}
	}
}

// 타겟 찾는 함수
void UUnitCombatComponent::SelectBestTarget(const TSet<TWeakObjectPtr<AActor>>& InDetectedCharacters)
{
	AActor* ResultTarget = nullptr;

	float WinScore = 0.0f;

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
		
	}
	if (ResultTarget)
	{
		TargetActor = ResultTarget;
		TargetAssigned(TargetActor.Get());
	}
}

AActor* UUnitCombatComponent::GetHigherPriorityTarget(AActor* CurrentTarget, AActor* WinTarget, float& WinScore)
{
	AActor* ResultCharacter = nullptr;
	float CurrentTargetScore = 0;

	if (OwnerCharacter.IsValid())
	{
		CurrentTargetScore = UnitData.Info.AISetting.TargetPriorityWeightData.CalculateScore(OwnerCharacter.Get(), CurrentTarget);
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

bool UUnitCombatComponent::IsCanAttackRange()
{
	if (TargetActor.IsValid())
	{
		FVector OutPoint;
		float Distance = TargetActor->ActorGetDistanceToCollision(GetOwner()->GetActorLocation(), ECollisionChannel::ECC_Pawn, OutPoint);
		//Distance *= Distance;
		//float Distance = (TargetActor->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
		if (Distance <= UnitData.Info.AISetting.AttackRange)
		{
			return true;
		}
	}
	return false;
}

void UUnitCombatComponent::SetRegenStat(const FRegenStat& InValue)
{
	const FGameplayTag& Tag = InValue.ParentTag;
	if (ActionSet.Contains(Tag))
	{
		ActionSet.Find(Tag)->GetDefaultObject()->StartAction(GetOwner(), InValue);
	}
}

void UUnitCombatComponent::SetNonRegenStat(const FNonRegenStat& InValue)
{
	const FGameplayTag& Tag = InValue.TagName;

	if (ActionSet.Contains(Tag))
	{
		ActionSet.Find(Tag)->GetDefaultObject()->StartAction(GetOwner(), InValue);
	}
}


// ========================================================
// 상태
// ========================================================
void UUnitCombatComponent::LightHitReaction(float InDamage)
{
	//UE_LOG(LogTemp, Error, TEXT("InDamage : %f"), InDamage);
	UPoolingSubsystem* PoolingSystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
	if (PoolingSystem && FloatingTextActorClass)
	{
		FTransform Transform;
		Transform.SetLocation((GetOwner()->GetActorLocation() + (GetOwner()->GetActorUpVector() * 100.0f)) + (FMath::VRand() * 10.0f));
		AActor* FloatingActor = PoolingSystem->SpawnFromPool(FloatingTextActorClass, Transform);
		if (AFloatingDamageText* FloatingText = Cast<AFloatingDamageText>(FloatingActor))
		{
			FloatingText->SetText(InDamage);
		}
	}
}

void UUnitCombatComponent::Death(const FRegenStat& InData)
{
	if (!bIsDead && InData.ParentTag == Arcanum::BattleStat::Character::Regen::Health::Root && InData.Current <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
		bIsDead = true;
		StateChange(EUnitState::Death);
		OwnerAIC->BrainComponent->StopLogic(TEXT("정지"));
		OwnerCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//OwnerCharacter->StopAnimMontage();
		UAnimMontage* DeathMontage = nullptr;
		int32 IDX = 0;
		FTimerDelegate DeathTimerDelegate;
		DeathTimerDelegate.BindWeakLambda(this, [this]()
			{
				UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
				if (PoolingSubsystem)
				{
					if (!PoolingSubsystem->DeactiveItem(GetOwner()))
					{
						GetOwner()->Destroy();
					}
				}
			});

		if (OwnerCharacter.IsValid() && UnitData.Info.AnimSetting.Deads.Num() > 0)
		{
			IDX = FMath::RandRange(0, (UnitData.Info.AnimSetting.Deads.Num() - 1));
			DeathMontage = UnitData.Info.AnimSetting.Deads[IDX].DeadMontage;
			OwnerCharacter->PlayAnimMontage(DeathMontage);

			GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, DeathTimerDelegate, UnitData.Info.AnimSetting.Deads[IDX].DeactiveTime, false, UnitData.Info.AnimSetting.Deads[IDX].DeactiveTime);
		}
	}
	else return;
}
