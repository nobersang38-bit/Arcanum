#include "Object/Skills/SkillType/ProjectileBase.h"
#include "Object/Skills/SkillBase.h"
#include "TimerManager.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision");
    RootComponent = CollisionComponent;

    CollisionComponent->InitSphereRadius(20.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionComponent->SetCollisionProfileName("BlockAll");

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
    MovementComponent->InitialSpeed = InitialSpeed;
    MovementComponent->MaxSpeed = InitialSpeed;
    MovementComponent->bRotationFollowsVelocity = true;
    MovementComponent->bShouldBounce = false;
}
void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
}
void AProjectileBase::ActivateSkillActor(USkillBase* InSkill, AActor* InOwner, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    Super::ActivateSkillActor(InSkill, InOwner, SpawnLocation, SpawnRotation);

    MovementComponent->Velocity = SpawnRotation.Vector() * InitialSpeed;
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &AProjectileBase::DeactivateSkillActor, LifeTime, false);
}
void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bIsActive) return;
    if (!OtherActor || OtherActor == InstigatorActor.Get()) return;

    // 여기서 데미지/스탯 적용 (OwnerSkill 통해 접근)

    DeactivateSkillActor();
}