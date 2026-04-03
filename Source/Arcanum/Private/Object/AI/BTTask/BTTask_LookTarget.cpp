// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/AI/BTTask/BTTask_LookTarget.h"
#include "Character/PlayerCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LookTarget::UBTTask_LookTarget()
{
	NodeName = TEXT("LookTarget");
	bNotifyTick = true;
	bCreateNodeInstance = true;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_LookTarget, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTask_LookTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (!CachedPlayerCharacter.IsValid())
	{
		if (APlayerCharacter* TempPlayerCharacter = Cast<APlayerCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			CachedPlayerCharacter = TempPlayerCharacter;
		}
		else return EBTNodeResult::Failed;

		if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey())))
		{
			CachedTargetActor = TargetActor;
		}
		else return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_LookTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (CachedTargetActor.IsValid())
	{
		FVector PlayerLocation = CachedPlayerCharacter->GetActorLocation();
		PlayerLocation = FVector(PlayerLocation.X, PlayerLocation.Y, 0.0f);

		FVector TargetLocation = CachedTargetActor->GetActorLocation();
		TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, 0.0f);

		FRotator ResultRotator = (TargetLocation - PlayerLocation).Rotation();
		FRotator NextRotator = FMath::RInterpConstantTo(CachedPlayerCharacter->GetActorRotation(), ResultRotator, DeltaSeconds, RotationSpeed);

		CachedPlayerCharacter->SetActorRotation(NextRotator);
		//CachedPlayerCharacter->GetController()->SetControlRotation(NextRotator);

		if (FMath::Abs(ResultRotator.Yaw - NextRotator.Yaw) <= MarginYaw)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
