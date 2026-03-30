#pragma once

#include "CoreMinimal.h"
#include "Object/Skills/SkillActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Curves/CurveFloat.h"
#include "GameplayTags/ArcanumTags.h"
#include "ProjectileBase.generated.h"

UENUM()
enum class ECollisionMode
{
    Immediately             UMETA(DisplayName = "Immediately"), // 첫번째 충돌후 비활성화
    ActivateOnCollision     UMETA(DisplayName = "ActivateOnCollision"), // 첫번째 충돌 후 범위 콜리전 활성화 후 비활성화
    Infinity                UMETA(DisplayName = "Infinity"), // 라이프타임이 다 되기 전까지 활성화 상태
};

UENUM()
enum class EProjectileMode : uint8
{
    Straight    UMETA(DisplayName = "Straight"),
    Horming     UMETA(DisplayName = "Horming"),
    Howitzer    UMETA(DisplayName = "Howitzer")
};

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
    virtual void DeactivateSkillActor() override;
    /** 충돌 */
    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** 오버랩 */
    UFUNCTION()
    virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    void CollisionProcess(AActor* OtherActor);
    void ActivateOnCollisionProcess(AActor* OtherActor);
    bool TargetfilterCheck(AActor* OtherActor);

    UFUNCTION()
    void DeactivateDelay(float InDelay);

protected:
    /** 충돌용 콜리전 */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> CollisionComponent;

    /** 이동 컴포넌트 */
    //UPROPERTY(VisibleAnywhere)
    //TObjectPtr<UProjectileMovementComponent> MovementComponent;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe")
    float ReOnCollisionTime = 1.0f;

    // 공격이라면 공격로직
    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe")
    bool bIsAttack = true;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe")
    bool bUseOwnerStat = false;

    // 사용할 태그 만약 공격력을 넣었다면 선택한 태그의 값을 스킬의 모디파이어에 스탯의 Value값에 더한다
    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe", meta = (EditCondition = "bUseOwnerStat", EditConditionHides))
    FGameplayTag UseStatTag;

    /** 기본 속도 */
    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe")
    float InitialSpeed = 1200.f;

    /** 수명 */
    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe")
    float LifeTime = 5.f;

    // 충돌이 끝나고 나서 비활성화될 시간
    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe", meta = (EditCondition = "CollisionMode != ECollisionMode::Infinity", EditConditionHides))
    float DeactivationDealay = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Collision")
    ECollisionMode CollisionMode = ECollisionMode::Immediately;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Collision", meta = (EditCondition = "CollisionMode == ECollisionMode::ActivateOnCollision", EditConditionHides))
    float SphereCollisionRadius = 200.0f;


    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Horming")
    EProjectileMode ProjectileMode = EProjectileMode::Straight;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Horming", meta = (EditCondition = "ProjectileMode == EProjectileMode::Horming", EditConditionHides))
    float HormingSpeed = 50.0f;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Howitzer", meta = (EditCondition = "ProjectileMode == EProjectileMode::Howitzer", EditConditionHides))
    FRuntimeFloatCurve HowitzerHeightCurve;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Howitzer", meta = (EditCondition = "ProjectileMode == EProjectileMode::Howitzer", EditConditionHides))
    FRuntimeFloatCurve HowitzerSpeedCurve;

    UPROPERTY(EditDefaultsOnly, Category = "ProjectiIe|Howitzer", meta = (EditCondition = "ProjectileMode == EProjectileMode::Howitzer", EditConditionHides))
    float HowitzerHeight;

    // 같은 팀이면 충돌 활성화
    UPROPERTY(VisibleAnywhere, Category = "TagSetting")
    FGameplayTag AllyTag = Arcanum::Skills::TargetFilter::Ally;

    // 다른 팀이면 충돌 활성화
    UPROPERTY(VisibleAnywhere, Category = "TagSetting")
    FGameplayTag EnemyTag = Arcanum::Skills::TargetFilter::Enemy;

    // 자기 자신일 경우만 충돌 활성화
    UPROPERTY(VisibleAnywhere, Category = "TagSetting")
    FGameplayTag SelfTag = Arcanum::Skills::TargetFilter::Self;

    UPROPERTY(VisibleAnywhere, Category = "TagSetting")
    FGameplayTag NoneTag = Arcanum::Skills::TargetFilter::None;

    FTimerHandle LifeTimerHandle;
    FTimerHandle DeactiveDelayTimerHandle;
    float HowitzerTime = 0.0f;
    FTransform HowitzerStartTransform;
    ECollisionEnabled::Type FirstCollisionEnabled;
    bool bIsFirstStart = true;

    UPROPERTY()
    TSet<AActor*> CachedActors;

    UPROPERTY()
    TMap<AActor*, float> ActorCollisionCoolTime;

private:
    bool bShowCollision = false;
};