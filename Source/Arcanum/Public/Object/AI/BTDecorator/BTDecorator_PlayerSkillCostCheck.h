// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "Data/Types/BTPlayerStruct.h"
#include "BTDecorator_PlayerSkillCostCheck.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UBTDecorator_PlayerRangeCheck : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_PlayerRangeCheck();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EBSkillType SkillType;
};
