// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PlayerSkill.generated.h"

UENUM(BlueprintType)
enum class EAISkillType : uint8
{
	WeaponSkill		UMETA(DisplayName = "WeaponSkill"),
	UnitSpawn		UMETA(DisplayName = "UnitSpawn")
};

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UBTTask_PlayerSkill : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_PlayerSkill();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAISkillType AISkillType = EAISkillType::WeaponSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetActorName = FName("TargetActor");
};
