// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerSelectTarget.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UBTService_PlayerSelectTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_PlayerSelectTarget();

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector CurrentDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector IsMoveKey;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class APlayerCharacter> CachedPlayerCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<class ABattlePlayerController> CachedPlayerController = nullptr;
};
