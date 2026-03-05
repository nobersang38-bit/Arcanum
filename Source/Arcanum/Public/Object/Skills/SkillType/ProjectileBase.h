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

    /** 충돌 */
    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
    /** 충돌용 콜리전 */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> CollisionComponent;

    /** 이동 컴포넌트 */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UProjectileMovementComponent> MovementComponent;

    /** 기본 속도 */
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float InitialSpeed = 1200.f;

    /** 수명 */
    UPROPERTY(EditDefaultsOnly, Category = "Projectile")
    float LifeTime = 5.f;

    FTimerHandle LifeTimerHandle;
};