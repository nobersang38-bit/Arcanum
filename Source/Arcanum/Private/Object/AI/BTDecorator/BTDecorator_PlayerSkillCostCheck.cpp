#include "Object/AI/BTDecorator/BTDecorator_PlayerSkillCostCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Data/Types/BTPlayerStruct.h"

UBTDecorator_PlayerRangeCheck::UBTDecorator_PlayerRangeCheck()
{
	NodeName = TEXT("PlayerSkillRangeCheck");
	bNotifyTick = false;
	bCreateNodeInstance = true;
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecorator_PlayerRangeCheck, BlackboardKey), UBTPlayerDataObject::StaticClass());
}

bool UBTDecorator_PlayerRangeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBTPlayerDataObject* PlayerDataObject = nullptr;
	if (UBehaviorTreeComponent* Behavior = &OwnerComp)
	{
		PlayerDataObject = Cast<UBTPlayerDataObject>(Behavior->GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	}

	if (PlayerDataObject)
	{
		return PlayerDataObject->RangeCheck(SkillType);
	}

	return false;
}
