#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingStarActor.generated.h"

class UNiagaraComponent;
class AASplinePathActor;
class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootingStarFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootingStarShrink);

UCLASS()
class ARCANUM_API AShootingStarActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable) FOnShootingStarFinished OnShootingStarFinished;
	UPROPERTY(BlueprintAssignable) FOnShootingStarShrink OnShootingStarShrink;

public:
	AShootingStarActor();
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SetSplineActor(AASplinePathActor* InSpline);

	void StartShootingStar();

	void ForceFinish();

	virtual void Tick(float DeltaTime) override;



private:
	void MoveShootingStar();
	void EndShootingStar();
	void FinishSequenceAndBroadcast();

	// ========================================================
	// Components
	// ========================================================
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StarMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> IntroSparkleComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> TrailNiagaraComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> ImpactExplosionComp;

	// ========================================================
	// Spline
	// ========================================================
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<AASplinePathActor> SplineActor;

private:
	UPROPERTY()
	TObjectPtr<USplineComponent> Spline;

	// ========================================================
	// Settings
	// ========================================================
protected:
	UPROPERTY(EditAnywhere) float MoveTimerDuration = 1.f;

	UPROPERTY(EditAnywhere) float MoveSpeed = 2000.f;

	UPROPERTY(EditAnywhere) float ShrinkSpeed = 5.0f;

	UPROPERTY(EditAnywhere) float ExplosionDelay = 2.0f;

	UPROPERTY(EditAnywhere) float BrakingZone = 2000.0f;

	// ========================================================
	// Runtime
	// ========================================================
private:
	float CurrentDistance = 0.f;
	bool bIsShrinking = false;

	FTimerHandle LaunchTimerHandle;
	FTimerHandle ExplosionTimerHandle;
};
