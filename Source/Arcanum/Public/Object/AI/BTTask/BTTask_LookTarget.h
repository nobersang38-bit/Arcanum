// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_LookTarget.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UBTTask_LookTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_LookTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float MarginYaw = 10.0f;

protected:
	UPROPERTY()
	TWeakObjectPtr<class APlayerCharacter> CachedPlayerCharacter = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> CachedTargetActor = nullptr;
};
