// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/AI/BTServiece/BTService_PlayerSelectTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/AIController/PlayerAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Interface/RuntimeUnitDataInterface.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "UI/Battle/BattlePlayerController.h"

UBTService_PlayerSelectTarget::UBTService_PlayerSelectTarget()
{
	NodeName = TEXT("PlayerSelectTarget");
	bNotifyTick = true;
	bCreateNodeInstance = true;
	bCallTickOnSearchStart = true;
	Interval = 0.5f;
	RandomDeviation = 0.1f;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_PlayerSelectTarget, BlackboardKey), AActor::StaticClass());
}

void UBTService_PlayerSelectTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		CurrentDistance.ResolveSelectedKey(*BBAsset);
		IsMoveKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_PlayerSelectTarget::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
}

void UBTService_PlayerSelectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (!CachedPlayerCharacter.IsValid())
	{
		if (APlayerCharacter* TempPlayerCharacter = Cast<APlayerCharacter>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			CachedPlayerCharacter = TempPlayerCharacter;

			if (ABattlePlayerController* TempPlayerController = Cast<ABattlePlayerController>(CachedPlayerCharacter->CachedOwnerPC))
			{
				CachedPlayerController = TempPlayerController;
			}
		}
	}
	APlayerAIController* PlayerAIC = Cast<APlayerAIController>(OwnerComp.GetAIOwner());
	if (!PlayerAIC) return;
	UBehaviorTreeComponent* Behavior = &OwnerComp;
	if (!Behavior) return;

	APawn* MyPawn = PlayerAIC->GetPawn();
	if (!MyPawn) return;

	// 1. 현재 AI가 인지하고 있는 모든 액터 가져오기
	TArray<AActor*> PerceivedActors;
	PlayerAIC->GetAIPerceptionComp()->GetKnownPerceivedActors(nullptr, PerceivedActors);


	AActor* ClosestActor = nullptr;
	ClosestActor = TargetPriorityWeightData.CalculateWinActor(MyPawn, PerceivedActors);

	//float MinDistance = MAX_FLT; // 아주 큰 값으로 초기화


	//for (AActor* Actor : PerceivedActors)
	//{
	//	// 2. 유효성 및 팀 확인 (이전에 만든 인터페이스 활용)
	//	if (!Actor || Actor == MyPawn) continue;

	//	// 적대적 관계인지 확인
	//	if (PlayerAIC->GetTeamAttitudeTowards(*Actor) != ETeamAttitude::Hostile) continue;

	//	// 죽었는지 확인 (인터페이스 활용)
	//	if (Actor->Implements<URuntimeUnitDataInterface>())
	//	{
	//		if (IRuntimeUnitDataInterface* Interface = Cast<IRuntimeUnitDataInterface>(Actor))
	//		{
	//			if (Interface->GetIsDead()) continue;
	//		}
	//	}

	//	// 3. 거리 계산 및 최솟값 갱신
	//	float Distance = FVector::DistSquared(MyPawn->GetActorLocation(), Actor->GetActorLocation());
	//	if (Distance < MinDistance)
	//	{
	//		MinDistance = Distance;
	//		ClosestActor = Actor;
	//	}
	//}

	AActor* TargetActor = Cast<AActor>(Behavior->GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!ClosestActor)
	{
		UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		if (BattleSubsystem)
		{
			Behavior->GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), BattleSubsystem->GetEnemyBasement());
			float Distance = (BattleSubsystem->GetEnemyBasement()->GetActorLocation() - MyPawn->GetActorLocation()).Length();
			Behavior->GetBlackboardComponent()->SetValueAsFloat(CurrentDistance.SelectedKeyName, Distance);
		}
	}
	else if (ClosestActor && TargetActor != ClosestActor)
	{
		UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		if (BattleSubsystem)
		{
			Behavior->GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ClosestActor);
			float Distance = (BattleSubsystem->GetEnemyBasement()->GetActorLocation() - MyPawn->GetActorLocation()).Length();
			Behavior->GetBlackboardComponent()->SetValueAsFloat(CurrentDistance.SelectedKeyName, Distance);
		}
		UE_LOG(LogTemp, Log, TEXT("가장 가까운 적 발견: %s"), *ClosestActor->GetName());
	}

	//UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	//if (BattleSubsystem && MyPawn)
	//{
	//	AActor* Target = Cast<AActor>(Behavior->GetBlackboardComponent()->GetValueAsObject(BlackboardKey.SelectedKeyName));
	//	if (Target)
	//	{
	//		float Distance = (Target->GetActorLocation() - MyPawn->GetActorLocation()).Length();
	//		Behavior->GetBlackboardComponent()->SetValueAsFloat(CurrentDistance.SelectedKeyName, Distance);
	//	}
	//}




	//움직일 수 있는 상태인가
	if (CachedPlayerCharacter.IsValid())
	{
		bool bIsMove = !(CachedPlayerCharacter->bHasNextComboInput	||
			CachedPlayerCharacter->bIsBasicAttackMontagePlaying		||
			CachedPlayerCharacter->bIsCommonSkillMontagePlaying		||
			CachedPlayerCharacter->bIsUltimateReleaseMontagePlaying ||
			CachedPlayerController->bIsUltimateAiming);

		Behavior->GetBlackboardComponent()->SetValueAsBool(IsMoveKey.SelectedKeyName, bIsMove);
	}

}
