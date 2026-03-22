// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/AI/BTTask/BTTask_PlayerSkill.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/Types/BTPlayerStruct.h"
#include "UI/Battle/BattlePlayerController.h"

UBTTask_PlayerSkill::UBTTask_PlayerSkill()
{
	NodeName = TEXT("PlayerSkill");
	bNotifyTick = false;
	bCreateNodeInstance = true;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_PlayerSkill, BlackboardKey), UBTPlayerDataObject::StaticClass());
}

EBTNodeResult::Type UBTTask_PlayerSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBTPlayerDataObject* PlayerDataObject = nullptr;
	if (UBehaviorTreeComponent* Behavior = &OwnerComp)
	{
		PlayerDataObject = Cast<UBTPlayerDataObject>(Behavior->GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	}

	if (AISkillType == EAISkillType::WeaponSkill)
	{
		if (PlayerDataObject)
		{
			AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorName));
			if (TargetActor)
			{
				PlayerDataObject->SetTargetActor(TargetActor);
			}
			else
			{
				return EBTNodeResult::Failed;
			}

			bool Result = PlayerDataObject->UseSkill();
			if (Result)
			{
				return EBTNodeResult::Succeeded;
			}
		}
	}
	else if (AISkillType == EAISkillType::UnitSpawn)
	{
		PlayerDataObject->SpawnUnit();
		return EBTNodeResult::Succeeded;
	}
	

	return EBTNodeResult::Failed;
}
