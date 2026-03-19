// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/PlayerAIController.h"
#include "Interface/TeamInterface.h"
#include "Perception/AIPerceptionComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"

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

	if (AIPerceptionComp)
	{
		AIPerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &APlayerAIController::OnTargetPerceptionUpdated);
	}
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

void APlayerAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)// 플레이어 감지를 성공하면 TargetEnemy에 할당
	{
		if (!TargetActor.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("%s : 적을 감지했습니다."), *Actor->GetName());
			TargetActor = Actor;
		}
	}
}

//if (Internal_UnitCombatComponent->TargetActor.IsValid() && Internal_UnitCombatComponent->TargetActor->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
//{
//	auto Interface = Cast<IRuntimeUnitDataInterface>(Internal_UnitCombatComponent->TargetActor);
//	if (Interface && Interface->GetIsDead())
//	{
//		// 상태 변경
//		Internal_UnitCombatComponent->StateChange(EUnitState::Move);
//	}
//}