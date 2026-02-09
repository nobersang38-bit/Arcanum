// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseUnitCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameplayTags/ArcanumTags.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Component/UnitCombatComponent.h"

// Sets default values
ABaseUnitCharacter::ABaseUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DetectComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectComponent"));
	UnitCombatComponent = CreateDefaultSubobject<UUnitCombatComponent>(TEXT("UnitCombatComponent"));

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(0.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

FGameplayTag ABaseUnitCharacter::GetTeamTag_Implementation()
{
	if (UnitData.Stat.CommonStat.TeamID.IsValid())
	{
		return UnitData.Stat.CommonStat.TeamID;
	}

	return FGameplayTag::EmptyTag;
};

void ABaseUnitCharacter::BeginPlay()
{
	if (!UnitData.Info.AISetting.TargetPriorityWeight.IsNull())
	{
		if (UDATargetPriorityWeight* TargetPriorityWeight = UnitData.Info.AISetting.TargetPriorityWeight.LoadSynchronous())
		{
			UnitCombatComponent->SetTargetPriorityWeight(TargetPriorityWeight);
		}
	}
	Super::BeginPlay();
}

void ABaseUnitCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIController = AIC;
	}

	// Todo KDH : 테스트용 나중에 타이머 빼야함
	FTimerDelegate AIInitializeTimerDelegate;
	AIInitializeTimerDelegate.BindUObject(this, &ABaseUnitCharacter::AIInitialize);
	GetWorld()->GetTimerManager().SetTimerForNextTick(AIInitializeTimerDelegate);

	DataInitialize();
}

void ABaseUnitCharacter::TargetAssigned(ACharacter* Target)
{
	if (!Target) return;

	TargetCharacter = Target;

	if (AIController.IsValid())
	{
		if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
		{
			BBComp->SetValue<UBlackboardKeyType_Object>(TargetActorKey, TargetCharacter.Get());
		}
	}
	
}

void ABaseUnitCharacter::AIInitialize()
{
	if (!UnitData.Info.AISetting.BehaviorTree.IsNull())
	{
		if (UBehaviorTree* BehaviorTree = UnitData.Info.AISetting.BehaviorTree.LoadSynchronous())
		{
			AIController->RunBehaviorTree(BehaviorTree);
			if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
			{
				if (!UnitData.Info.AISetting.BBTargetActorName.IsNone())
				{
					TargetActorKey = BBComp->GetKeyID(UnitData.Info.AISetting.BBTargetActorName);
				}
			}
		}
	}
	
	if (!UnitData.Info.AISetting.TargetPriorityWeight.IsNull())
	{
		if (UDATargetPriorityWeight* TargetPriorityWeight = UnitData.Info.AISetting.TargetPriorityWeight.LoadSynchronous())
		{
			DetectComp->SetSphereRadius(TargetPriorityWeight->GetDetectDistance());
		}
	}

	if (UBattlefieldManagerSubsystem* BattlefieldManager = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		if (BattlefieldManager->GetAllyNexus() && BattlefieldManager->GetEnemyNexus())
		{
			FGameplayTag MyTeamID = GetTeamTag_Implementation();

			if (MyTeamID == Arcanum::Unit::Faction::Ally)
			{
				TargetAssigned(BattlefieldManager->GetEnemyNexus());

			}
			else if (MyTeamID == Arcanum::Unit::Faction::Enemy)
			{
				TargetAssigned(BattlefieldManager->GetAllyNexus());
			}
		}
	}
}

void ABaseUnitCharacter::DataInitialize()
{
	FGameplayTag MyTeamID = GetTeamTag_Implementation();

	if (!UnitData.Info.BodySetting.SkeletalMesh.IsNull())// 스켈레탈 메시 설정
	{
		if (USkeletalMesh* SkeletalMesh = UnitData.Info.BodySetting.SkeletalMesh.LoadSynchronous())
		{
			GetMesh()->SetSkeletalMesh(SkeletalMesh);
		}
	}

	if (UnitData.Info.AnimSetting.AnimInstance) // 애님 인스턴스 설정
	{
		GetMesh()->SetAnimInstanceClass(UnitData.Info.AnimSetting.AnimInstance);
	}
}

AActor* ABaseUnitCharacter::TargetCalcTest()
{

	return nullptr;
}

void ABaseUnitCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;					// 자신이면 제외
	if (!OtherActor->IsA<ACharacter>()) return;
	if (!OtherActor->Implements<UTeamInterface>()) return;	// 팀 아이디 인터페이스가 없으면 제외
	if (ITeamInterface::Execute_GetTeamTag(OtherActor) == ITeamInterface::Execute_GetTeamTag(this)) return; // 같은 팀이면 제외

	ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor);
	if (DetectedCharacter)
	{
		DetectedCharacters.Add(DetectedCharacter);
	}
}

void ABaseUnitCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter* DetectedCharacter = Cast<ACharacter>(OtherActor))
	{
		DetectedCharacters.Remove(DetectedCharacter);
	}
}

