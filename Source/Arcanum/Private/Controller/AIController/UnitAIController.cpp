// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/UnitAIController.h"
#include "Components/SphereComponent.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"

AUnitAIController::AUnitAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUnitAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

FGameplayTag AUnitAIController::GetTeamTag_Implementation()
{
	if (GetPawn() && GetPawn()->Implements<UTeamInterface>())
	{
		return ITeamInterface::Execute_GetTeamTag(GetPawn());
	}
	else if (TeamID.IsValid())
	{
		return TeamID;
	}

	return FGameplayTag::EmptyTag;
};

void AUnitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!DetectComponent)
	{
		DetectComponent = NewObject<USphereComponent>(InPawn, USphereComponent::StaticClass());
		if (DetectComponent)
		{
			DetectComponent->RegisterComponent();
			DetectComponent->AttachToComponent(InPawn->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			if (TargetPriorityWeight)
			{
				DetectComponent->SetSphereRadius(TargetPriorityWeight->GetDetectDistance());
			}

			DetectComponent->OnComponentBeginOverlap.AddDynamic(this, &AUnitAIController::BeginOverlap);
			DetectComponent->OnComponentEndOverlap.AddDynamic(this, &AUnitAIController::EndOverlap);
		}
	}
}

AActor* AUnitAIController::TargetCalcTest()
{

	return nullptr;
}

void AUnitAIController::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetPawn()) return;					// 자신이면 제외
	if (!OtherActor->Implements<UTeamInterface>()) return;	// 팀 아이디 인터페이스가 없으면 제외
	//if ()

}

void AUnitAIController::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
