#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GachaCamera.generated.h"

class UCameraComponent;
class UCameraShakeBase;
class UActiveCameraShakeInfo;

UCLASS()
class ARCANUM_API AGachaCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AGachaCamera();
    virtual void Tick(float DeltaTime) override;
    /** 따라갈 액터 지정 함수 */
    void SetTargetActor(AActor* InTarget) { TargetActor = InTarget; }
protected:
	virtual void BeginPlay() override;

public:
    /** 따라가는 카메라 속도*/
    UPROPERTY(EditAnywhere, Category = "00-Global")
    float FollowSpeed = 10.0f;
    UPROPERTY(EditAnywhere, Category = "00-Global")
    float RotationSpeed = 5.0f;
    UPROPERTY(EditAnywhere, Category = "00-Global")
    FVector RelativeOffset = FVector(-800.0f, 0.0f, 200.0f);

#pragma region 카메라 진동
public:
    void PlayFlyingShake();
    void StopFlyingShake();
    void PlayImpactShake();
protected:
    UPROPERTY(EditAnywhere, Category = "00-Global") TSubclassOf<UCameraShakeBase> FlyingShakeClass;
    UPROPERTY(EditAnywhere, Category = "00-Global") TSubclassOf<UCameraShakeBase> ImpactShakeClass;
private:
    UPROPERTY() TObjectPtr<UCameraShakeBase> FlyingShakeInstance;
#pragma endregion


private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UCameraComponent> CameraComp;
    UPROPERTY()
    TObjectPtr<AActor> TargetActor;
    FVector LastTargetLocation;
};
