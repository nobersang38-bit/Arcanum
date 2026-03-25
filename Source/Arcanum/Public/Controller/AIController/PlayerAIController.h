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
	const class UAIPerceptionComponent* GetAIPerceptionComp() const { return AIPerceptionComp; }
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void BeginPlay() override;

	/*UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);*/

	/*UFUNCTION()
	void UpdateTarget();*/

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComp = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<AActor> CachedEnemyBasement = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<AActor> TargetActor = nullptr;

	FTimerHandle TimerHandle;
};
