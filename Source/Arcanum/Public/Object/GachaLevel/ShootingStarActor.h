#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShootingStarActor.generated.h"

class UNiagaraComponent;
class AASplinePathActor;
class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootingStarFinished);

UCLASS()
class ARCANUM_API AShootingStarActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable) FOnShootingStarFinished OnShootingStarFinished;

	AShootingStarActor();
	UFUNCTION() void SetSplineActor(AASplinePathActor* InSpline);
	virtual void Tick(float DeltaTime) override;
	void StartShootingStar();
private:
	void MoveShootingStar();
	void EndShootingStar();
	void FinishSequenceAndBroadcast();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StarMesh;
	/** 처음에 빛 번쩍 나이아가라*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> IntroSparkleComp;
	/** 움직이면서 꼬리(리본 나이아가라)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> TrailNiagaraComp;
	/** 초신성 폭발 같은 나이아가라*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> ImpactExplosionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AASplinePathActor> SplineActor;

	/** 별동별 움직이는 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 2000.f;
	/** 액터가 줄어드는 크기 */
	UPROPERTY(EditAnywhere, Category = "00-Global")
	float ShrinkSpeed = 5.0f;
	/** 브로드캐스트까지 시간 */
	UPROPERTY(EditAnywhere, Category = "00-Global")
	float ExplosionDelay = 2.0f;
	/** 감속 위치*/
	UPROPERTY(EditAnywhere, Category = "00-Global")
	float BrakingZone = 2000.0f;

private:
	float CurrentDistance = 0.f;
	bool bIsShrinking = false;
	USplineComponent* Spline;
};
