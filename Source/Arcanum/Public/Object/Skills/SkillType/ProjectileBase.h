#pragma once

#include "CoreMinimal.h"
#include "Object/Skills/SkillActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "ProjectileBase.generated.h"

UCLASS(Abstract)
class ARCANUM_API AProjectileBase : public ASkillActor
{
    GENERATED_BODY()

    public:
    AProjectileBase();

    virtual void ActivateSkillActor(USkillBase* InSkill, AActor* InOwner, const FVector& SpawnLocation, const FRotator& SpawnRotation) override;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float Deltatime) override;

    /** 충돌 */
    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** 오버랩 */
    UFUNCTION()
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
    /** 충돌용 콜리전 */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> CollisionComponent;

    /** 이동 컴포넌트 */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UProjectileMovementComponent> MovementComponent;

    /** 기본 속도 */
    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe")
    float InitialSpeed = 1200.f;

    /** 수명 */
    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe")
    float LifeTime = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Horming")
    bool bUseHorming = false;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Horming")
    float HormingSpeed = 50.0f;

    FTimerHandle LifeTimerHandle;
};