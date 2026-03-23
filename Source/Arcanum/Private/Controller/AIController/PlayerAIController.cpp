// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/PlayerAIController.h"
#include "Interface/TeamInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Interface/RuntimeUnitDataInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

APlayerAIController::APlayerAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));
}

void APlayerAIController::BeginPlay()
{
	Super::BeginPlay();
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (BattleSubsystem)
	{
		CachedEnemyBasement = BattleSubsystem->GetEnemyBasement();
	}

	/*if (AIPerceptionComp)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &APlayerAIController::OnTargetPerceptionUpdated);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerAIController::UpdateTarget, 0.5f, true, 0.5f);*/
}

ETeamAttitude::Type APlayerAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const AActor* OtherActor = &Other;
	auto MyInterface = Cast<ITeamInterface>(GetPawn());
	auto OtherInterface = Cast<ITeamInterface>(OtherActor);
	if (MyInterface && OtherInterface)
	{
		FGameplayTag MyTag = MyInterface->GetTeamTag();
		FGameplayTag OtherTag = OtherInterface->GetTeamTag();

		if (MyTag != OtherTag)
		{
			return ETeamAttitude::Hostile;
		}
		else
		{
			return ETeamAttitude::Friendly;
		}
	}
	return ETeamAttitude::Neutral;
}

//void APlayerAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	//if (Stimulus.WasSuccessfullySensed() && Actor)// 플레이어 감지를 성공하면 TargetEnemy에 할당
//	//{
//	//	bool bIsDead = false;
//	//	if (TargetActor.IsValid() && TargetActor->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
//	//	{
//	//		auto Interface = Cast<IRuntimeUnitDataInterface>(TargetActor);
//	//		bIsDead = Interface->GetIsDead();
//	//	}
//	//	if (!TargetActor.IsValid() || bIsDead)
//	//	{
//	//		UE_LOG(LogTemp, Log, TEXT("%s : 적을 감지했습니다."), *Actor->GetName());
//	//		TargetActor = Actor;
//	//	}
//	//}
//}

//void APlayerAIController::UpdateTarget()
//{
//	APawn* MyPawn = GetPawn();
//	if (!MyPawn) return;
//
//	// 1. 현재 AI가 인지하고 있는 모든 액터 가져오기
//	TArray<AActor*> PerceivedActors;
//	AIPerceptionComp->GetKnownPerceivedActors(nullptr, PerceivedActors);
//
//	AActor* ClosestActor = nullptr;
//	float MinDistance = MAX_FLT; // 아주 큰 값으로 초기화
//
//	for (AActor* Actor : PerceivedActors)
//	{
//		// 2. 유효성 및 팀 확인 (이전에 만든 인터페이스 활용)
//		if (!Actor || Actor == MyPawn) continue;
//
//		// 적대적 관계인지 확인
//		if (GetTeamAttitudeTowards(*Actor) != ETeamAttitude::Hostile) continue;
//
//		// 죽었는지 확인 (인터페이스 활용)
//		if (Actor->Implements<URuntimeUnitDataInterface>())
//		{
//			if (IRuntimeUnitDataInterface* Interface = Cast<IRuntimeUnitDataInterface>(Actor))
//			{
//				if (Interface->GetIsDead()) continue;
//			}
//		}
//
//		// 3. 거리 계산 및 최솟값 갱신
//		float Distance = FVector::DistSquared(MyPawn->GetActorLocation(), Actor->GetActorLocation());
//		if (Distance < MinDistance)
//		{
//			MinDistance = Distance;
//			ClosestActor = Actor;
//		}
//	}
//
//	if (!ClosestActor)
//	{
//		TargetActor = CachedEnemyBasement;
//		/*if (UBlackboardComponent* BB = GetBlackboardComponent())
//		{
//			BB->SetValueAsObject(TEXT("TargetActor"), TargetActor.Get());
//		}*/
//	}
//	else if (ClosestActor && TargetActor != ClosestActor)
//	{
//		TargetActor = ClosestActor;
//		UE_LOG(LogTemp, Log, TEXT("가장 가까운 적 발견: %s"), *ClosestActor->GetName());
//
//		//// 블랙보드 업데이트
//		//if (UBlackboardComponent* BB = GetBlackboardComponent())
//		//{
//		//	BB->SetValueAsObject(TEXT("TargetActor"), TargetActor.Get());
//		//}
//	}
//}

//if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->TargetActor->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
//{
//	auto Interface = Cast<IRuntimeUnitDataInterface>(Internal_UnitCombatComponent->TargetActor);
//	if (Interface && Interface->GetIsDead())
//	{
//		// 상태 변경
//		Internal_UnitCombatComponent->StateChange(EUnitState::Move);
//	}
//}