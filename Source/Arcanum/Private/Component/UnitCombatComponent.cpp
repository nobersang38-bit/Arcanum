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
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UUnitCombatComponent::UUnitCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UUnitCombatComponent::SendDamage(float InDamage)
{
	if (TargetCharacter.IsValid())
	{
		UGameplayStatics::ApplyDamage(TargetCharacter.Get(), InDamage, nullptr, GetOwner(), nullptr);
	}
}

// Called when the game starts
void UUnitCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UUnitCombatComponent::DeferredBeginPlay);

	//Todo KDH : Cast없애야함
	// Dead바인딩
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
		//OwnerCapsuleComponent->OnComponentHit.AddDynamic(this, &UUnitCombatComponent::Onhit);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("부모가 ACharacter가 아닙니다"));
		return;
	}

	// 감지 컴포넌트가 없다면 부모액터에 넣기
	if (!DetectComponent.IsValid()) 
	{
		if (GetOwner())
		{
			DetectComponent = NewObject<USphereComponent>(GetOwner(), USphereComponent::StaticClass());
		}

		if (DetectComponent.IsValid())
		{
			DetectComponent->RegisterComponent();
			DetectComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			DetectComponent->bFillCollisionUnderneathForNavmesh = false;
			DetectComponent->SetCanEverAffectNavigation(false);

			DetectComponent->OnComponentBeginOverlap.AddDynamic(this, &UUnitCombatComponent::OnBeginDetected);
			DetectComponent->OnComponentEndOverlap.AddDynamic(this, &UUnitCombatComponent::OnEndDetected);
		}
	}

	// AI 세팅 가져오기
	if (GetOwner()->GetClass()->ImplementsInterface(UUnitDataInterface::StaticClass()))
	{
		auto Interface = Cast<IUnitDataInterface>(GetOwner());
		UnitAISetting = Interface->GetUnitData().Info.AISetting;
		UnitData = Interface->GetUnitData();
		if (UDATargetPriorityWeight* TempTargetPriorityWeight = UnitAISetting.TargetPriorityWeight.LoadSynchronous())
		{
			TargetPriorityWeight = TempTargetPriorityWeight;
		}
		AIInitialize();
	}

	float InitialDelay = FMath::FRandRange(UnitAISetting.AITickParams.InitialDelayRange.X, UnitAISetting.AITickParams.InitialDelayRange.Y);
	float UpdateInterval = FMath::FRandRange(UnitAISetting.AITickParams.UpdateIntervalRange.X, UnitAISetting.AITickParams.UpdateIntervalRange.Y);
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	// 틱 재생
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UUnitCombatComponent::TickUpdate, UpdateInterval, true, InitialDelay);
}

void UUnitCombatComponent::TickUpdate()
{
	if (!bDebug_StopAI)
	{
		if (bDebug_DrawMoveTargeLine && TargetCharacter.IsValid())
		{
			DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), TargetCharacter->GetActorLocation(), FColor::Red, false, 2.0f);
		}
		switch (CurrentState)
		{
		case EUnitState::Idle:
			UE_LOG(LogTemp, Warning, TEXT("IDLE : %s"), *GetOwner()->GetName());
			Idle();
			break;
		case EUnitState::Move:
			UE_LOG(LogTemp, Warning, TEXT("Move : %s"), *GetOwner()->GetName());
			Move();
			break;
		case EUnitState::Attack:
			UE_LOG(LogTemp, Warning, TEXT("Attack : %s"), *GetOwner()->GetName());
			Attack();
			break;
		case EUnitState::ActionRestricted: // Todo : 태그로 CC기 종류 감지해야함
			UE_LOG(LogTemp, Warning, TEXT("ActionRestricted : %s"), *GetOwner()->GetName());
			break;
		case EUnitState::Death:
			UE_LOG(LogTemp, Warning, TEXT("Death : %s"), *GetOwner()->GetName());
			break;
		default:
			break;

		}
	}
	
}

void UUnitCombatComponent::AIInitialize()
{
	if (!OwnerAIC.IsValid()) return;

	if (!UnitAISetting.BehaviorTree.IsNull())
	{
		if (UBehaviorTree* BehaviorTree = UnitAISetting.BehaviorTree.LoadSynchronous())
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

	if (!UnitAISetting.TargetPriorityWeight.IsNull())
	{
		if (UDATargetPriorityWeight* TempTargetPriorityWeight = UnitAISetting.TargetPriorityWeight.LoadSynchronous())
		{
			DetectComponent->SetSphereRadius(TempTargetPriorityWeight->GetDetectDistance());
		}
	}

	if (UBattlefieldManagerSubsystem* BattlefieldManager = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		if (BattlefieldManager->GetAllyNexus() && BattlefieldManager->GetEnemyNexus() && GetOwner()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
		{
			auto Interface = Cast<ITeamInterface>(GetOwner());
			FGameplayTag MyTeamID = Interface->GetTeamTag();

			if (MyTeamID.MatchesTag(Arcanum::Unit::Faction::Ally::Root))
			{
				TargetNexus = BattlefieldManager->GetEnemyNexus();
			}
			else if (MyTeamID.MatchesTag(Arcanum::Unit::Faction::Enemy::Root))
			{
				TargetNexus = BattlefieldManager->GetAllyNexus();
			}

			if (TargetNexus.IsValid())
			{
				TargetAssigned(TargetNexus.Get());
			}
		}
	}
}

void UUnitCombatComponent::Onhit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ACharacter* HitCharacter = Cast<ACharacter>(OtherActor))
	{
		FVector Direction = HitCharacter->GetActorLocation() - GetOwner()->GetActorLocation();
		Direction = Direction.GetSafeNormal();
		HitCharacter->LaunchCharacter(Direction * 500.0f, true, true);
	}
}

void UUnitCombatComponent::TargetAssigned(ACharacter* Target)
{
	if (!Target) return;

	TargetCharacter = Target;

	if (OwnerAIC.IsValid())
	{
		if (UBlackboardComponent* BBComp = OwnerAIC->GetBlackboardComponent())
		{
			if (OwnerAIC.IsValid())
			{
				OwnerAIC->StopMovement();
			}
			BBComp->SetValue<UBlackboardKeyType_Object>(TargetActorKey, TargetCharacter.Get());
			CurrentState = EUnitState::Move;
		}
	}
}

void UUnitCombatComponent::OnBeginDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner()) return;					// 자신이면 제외
	if (!OtherActor->IsA<ACharacter>()) return;
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
	ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);
	if (DetectedCharacter && DetectedCharacters.Num() < UnitAISetting.MaxTargetCount)
	{
		if (DetectedCharacter->GetClass()->ImplementsInterface(UUnitDataInterface::StaticClass()))
		{
			auto Interface = Cast<IUnitDataInterface>(DetectedCharacter);
			if (Interface->GetUnitData().Info.AISetting.bIsElite)
			{
				UnitRuntimeData.Elites.Add(DetectedCharacter);
			}
		}
		DetectedCharacters.Add(DetectedCharacter);
	}
}

void UUnitCombatComponent::OnEndDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor))
	{
		if (DetectedCharacter->GetClass()->ImplementsInterface(UUnitDataInterface::StaticClass()))
		{
			auto Interface = Cast<IUnitDataInterface>(DetectedCharacter);
			if (Interface->GetUnitData().Info.AISetting.bIsElite)
			{
				UnitRuntimeData.Elites.Remove(DetectedCharacter);
			}
		}
		if (DetectedCharacters.Contains(DetectedCharacter) && DetectedCharacter == TargetCharacter)
		{
			if (TargetNexus.IsValid())
			{
				TargetAssigned(TargetNexus.Get());
			}
		}
		DetectedCharacters.Remove(DetectedCharacter);
	}
}

// 타겟 찾는 함수
void UUnitCombatComponent::SelectBestTarget(const TSet<TWeakObjectPtr<ACharacter>>& InDetectedCharacters)
{
	ACharacter* ResultTarget = nullptr;

	int32 WinScore = 0;

	for (const auto DetectedCharacter : InDetectedCharacters)
	{
		ResultTarget = GetHigherPriorityTarget(DetectedCharacter.Get(), ResultTarget, WinScore);
	}

	if (ResultTarget &&
		ResultTarget->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
	{
		if (TargetCharacter.IsValid() &&
			TargetCharacter->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
		{
			auto RevertInterface = Cast<IRuntimeUnitDataInterface>(TargetCharacter.Get());
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
		TargetCharacter = ResultTarget;
		TargetAssigned(TargetCharacter.Get());
	}
}

ACharacter* UUnitCombatComponent::GetHigherPriorityTarget(ACharacter* CurrentTarget, ACharacter* WinTarget, int32& WinScore)
{
	if (!TargetPriorityWeight) return nullptr;

	ACharacter* ResultCharacter = nullptr;
	float CurrentTargetScore = 0;

	if (OwnerCharacter.IsValid())
	{
		CurrentTargetScore = TargetPriorityWeight->CalculateScore(OwnerCharacter.Get(), CurrentTarget);
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

// 애매한 상황이 발생하면 여기로 오게됨
void UUnitCombatComponent::Idle()
{
	StateReset();
	TargetAssigned(TargetNexus.Get());
	CurrentState = EUnitState::Move;
}

void UUnitCombatComponent::Move()
{
	StateReset();
	SelectBestTarget(DetectedCharacters);

	if (TargetCharacter.IsValid())
	{
		float Distance = (TargetCharacter->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
		if (Distance <= UnitAISetting.AttackRange)
		{
			CurrentState = EUnitState::Attack;
		}
	}
}

void UUnitCombatComponent::Attack()
{
	FTimerDelegate AttackDelegate;
	AttackDelegate.BindWeakLambda(this, [this]()
		{
			if (OwnerCharacter.IsValid() && UnitData.Info.AnimSetting.Attacks.Num() > 0)
			{
				int32 IDX = FMath::RandRange(0, (UnitData.Info.AnimSetting.Attacks.Num() - 1) );
				UAnimMontage* AttackMontage = UnitData.Info.AnimSetting.Attacks[IDX];
				OwnerCharacter->PlayAnimMontage(AttackMontage);
			}
			else
			{
				CurrentState = EUnitState::Idle;
			}
		}); 
	if (!AttackTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, AttackDelegate, UnitAISetting.AttackRate, true, 0.0f);
	}
}

void UUnitCombatComponent::ActionRestricted(FGameplayTag InActionRestrictedTag)
{

}

void UUnitCombatComponent::Death(const FRegenStat& InData)
{
	if (!bIsDead && InData.ParentTag == Arcanum::BattleStat::Character::Regen::Health::Root && InData.Current <= 0.0f)
	{
		bIsDead = true;
		CurrentState = EUnitState::Death;
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

	for (auto It = DetectedCharacters.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
	
}
