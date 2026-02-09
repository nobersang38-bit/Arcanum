// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/UnitAIController.h"
#include "Components/SphereComponent.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameplayTags/ArcanumTags.h"

AUnitAIController::AUnitAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AUnitAIController::BeginPlay()
{
	Super::BeginPlay();
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
	// Todo KDH : 테스트용 나중에 타이머 빼야함
	FTimerDelegate AIInitializeTimerDelegate;
	AIInitializeTimerDelegate.BindUObject(this, &AUnitAIController::AIInitialize, InPawn);
	GetWorld()->GetTimerManager().SetTimerForNextTick(AIInitializeTimerDelegate);
}

void AUnitAIController::TargetAssigned(ACharacter* Target)
{
	if (!Target) return;

	TargetCharacter = Target;

	if (UBlackboardComponent* BBComp = GetBlackboardComponent())
	{
		BBComp->SetValue<UBlackboardKeyType_Object>(TargetActorKey, TargetCharacter.Get());
	}
}

void AUnitAIController::AIInitialize(APawn* InPawn)
{
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		if (UBlackboardComponent* BBComp = GetBlackboardComponent())
		{
			TargetActorKey = BBComp->GetKeyID(BBTargetActorName);
		}
	}

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

			if (UBattlefieldManagerSubsystem* BattlefieldManager = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
			{
				if (BattlefieldManager->GetAllyNexus() && BattlefieldManager->GetEnemyNexus())
				{
					FGameplayTag MyTeamID = GetTeamTag_Implementation();

					if (MyTeamID == Arcanum::TeamID::Friendly::Value)
					{
						TargetAssigned(BattlefieldManager->GetEnemyNexus());
					}
					else if (MyTeamID == Arcanum::TeamID::Enemy::Value)
					{
						TargetAssigned(BattlefieldManager->GetAllyNexus());
					}
				}
			}
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
	if (!OtherActor->IsA<ACharacter>()) return;
	if (!OtherActor->Implements<UTeamInterface>()) return;	// 팀 아이디 인터페이스가 없으면 제외
	if (ITeamInterface::Execute_GetTeamTag(OtherActor) == ITeamInterface::Execute_GetTeamTag(this)) return; // 같은 팀이면 제외

	ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);
	if (DetectedCharacter)
	{
		DetectedCharacters.Add(DetectedCharacter);
	}
}

void AUnitAIController::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor))
	{
		DetectedCharacters.Remove(DetectedCharacter);
	}
}
