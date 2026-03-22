// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/AI/BTDecorator/BTDecorator_PlayerSkillCostCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/Types/BTPlayerStruct.h"

UBTDecorator_PlayerSkillCostCheck::UBTDecorator_PlayerSkillCostCheck()
{
	NodeName = TEXT("PlayerSkillCostCheck");
	bNotifyTick = false;
	bCreateNodeInstance = true;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_PlayerSkillCostCheck, BlackboardKey), UBTPlayerDataObject::StaticClass());
}

bool UBTDecorator_PlayerSkillCostCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBTPlayerDataObject* PlayerDataObject = nullptr;
	if (UBehaviorTreeComponent* Behavior = &OwnerComp)
	{
		PlayerDataObject = Cast<UBTPlayerDataObject>(Behavior->GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	}
	
	if (PlayerDataObject)
	{
		return PlayerDataObject->CostCheck();
	}

	return false;
}
