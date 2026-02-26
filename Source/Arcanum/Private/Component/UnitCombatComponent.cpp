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
	if (TargetActor.IsValid())
	{
		UGameplayStatics::ApplyDamage(TargetActor.Get(), InDamage, nullptr, GetOwner(), nullptr);
	}
}

// Called when the game starts
void UUnitCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UUnitCombatComponent::DeferredBeginPlay);

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

	// AI 세팅 가져오기
	if (GetOwner()->GetClass()->ImplementsInterface(UUnitDataInterface::StaticClass()))
	{
		auto Interface = Cast<IUnitDataInterface>(GetOwner());
		UnitAISetting = Interface->GetUnitData().Info.AISetting;
		UnitData = Interface->GetUnitData();
		TargetPriorityWeight = UnitAISetting.TargetPriorityWeightData;
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
		if (bDebug_DrawMoveTargeLine && TargetActor.IsValid())
		{
			DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), TargetActor->GetActorLocation(), FColor::Red, false, 2.0f);
		}
		switch (CurrentState)
		{
		case EUnitState::Idle:
			//UE_LOG(LogTemp, Warning, TEXT("IDLE : %s"), *GetOwner()->GetName());
			Idle();
			break;
		case EUnitState::Move:
			//UE_LOG(LogTemp, Warning, TEXT("Move : %s"), *GetOwner()->GetName());
			Move();
			break;
		case EUnitState::Attack:
			//UE_LOG(LogTemp, Warning, TEXT("Attack : %s"), *GetOwner()->GetName());
			Attack();
			break;
		case EUnitState::ActionRestricted: // Todo : 태그로 CC기 종류 감지해야함
			//UE_LOG(LogTemp, Warning, TEXT("ActionRestricted : %s"), *GetOwner()->GetName());
			break;
		case EUnitState::Death:
			//UE_LOG(LogTemp, Warning, TEXT("Death : %s"), *GetOwner()->GetName());
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

void UUnitCombatComponent::Onhit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ACharacter* HitCharacter = Cast<ACharacter>(OtherActor))
	{
		FVector Direction = HitCharacter->GetActorLocation() - GetOwner()->GetActorLocation();
		Direction = Direction.GetSafeNormal();
		HitCharacter->LaunchCharacter(Direction * 500.0f, true, true);
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
			if (OwnerAIC.IsValid())
			{
				OwnerAIC->StopMovement();
			}
			BBComp->SetValue<UBlackboardKeyType_Object>(TargetActorKey, TargetActor.Get());
			CurrentState = EUnitState::Move;
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

// 애매한 상황이 발생하면 여기로 오게됨
void UUnitCombatComponent::Idle()
{
	StateReset();
	TargetAssigned(TargetBasement.Get());
	CurrentState = EUnitState::Move;
}

void UUnitCombatComponent::Move()
{
	StateReset();
	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape MySphere = FCollisionShape::MakeSphere(TargetPriorityWeight.GetDetectDistance());
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
		OnBeginDetected(nullptr, OutOverlaps[i].GetActor(), nullptr, 0, false, FHitResult());
	}
	if (DetectedActors.Num() > 0)
	{
		SelectBestTarget(DetectedActors);
	}
	else
	{
		TargetAssigned(TargetBasement.Get());
	}

	if (TargetActor.IsValid())
	{
		float Distance = (TargetActor->GetActorLocation() - GetOwner()->GetActorLocation()).SquaredLength();
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
			if (TargetActor.IsValid() && OwnerCharacter.IsValid() && UnitData.Info.AnimSetting.Attacks.Num() > 0)
			{
				int32 IDX = FMath::RandRange(0, (UnitData.Info.AnimSetting.Attacks.Num() - 1));
				UAnimMontage* AttackMontage = UnitData.Info.AnimSetting.Attacks[IDX];
				OwnerCharacter->PlayAnimMontage(AttackMontage);

				RotateDelegate.Unbind();
				RotateDelegate.BindWeakLambda(this, [this]()
					{
						if (TargetActor.IsValid() && OwnerCharacter.IsValid())
						{
							FVector Direction = TargetActor->GetActorLocation() - GetOwner()->GetActorLocation();
							Direction = FVector(Direction.X, Direction.Y, 0);
							GetOwner()->SetActorRotation(FMath::RInterpConstantTo(GetOwner()->GetActorRotation(), Direction.Rotation(), RotateInterval, RotateSpeed));
						}
						else
						{
							GetWorld()->GetTimerManager().ClearTimer(RotateTimerHandle);
						}
					});

				GetWorld()->GetTimerManager().ClearTimer(RotateTimerHandle);
				GetWorld()->GetTimerManager().SetTimer(RotateTimerHandle, RotateDelegate, RotateInterval, true, 0.0f);
			}
			else
			{
				GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
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

	for (auto It = DetectedActors.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
	
}
