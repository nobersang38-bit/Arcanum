// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "PlayerAIController.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API APlayerAIController : public AAIController
{
	GENERATED_BODY()
public:
	APlayerAIController();

protected:
	virtual void BeginPlay() override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComp = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<class ABasement> CachedEnemyBasement = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor = nullptr;
};
