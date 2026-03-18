#include "Object/Skills/SkillType/ProjectileBase.h"
#include "Object/Skills/SkillBase.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/StatModifierInterface.h"
#include "Interface/StatInterface.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision");
    RootComponent = CollisionComponent;

    CollisionComponent->InitSphereRadius(20.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionComponent->SetCollisionProfileName("BlockAll");

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);

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
void AProjectileBase::Tick(float Deltatime)
{
    Super::Tick(Deltatime);

    if (TargetActor.IsValid())
    {
        FVector HormingDirection = TargetActor->GetActorLocation() - GetActorLocation();
        FVector ResultDirection = FMath::VInterpConstantTo(GetActorForwardVector(), HormingDirection, Deltatime, HormingSpeed);
        MovementComponent->Velocity = ResultDirection.GetSafeNormal() * InitialSpeed;

    }
    else
    {
        FVector HormingDirection = TargetLocation - GetActorLocation();
        FVector ResultDirection = FMath::VInterpConstantTo(GetActorForwardVector(), HormingDirection, Deltatime, HormingSpeed);
        MovementComponent->Velocity = ResultDirection.GetSafeNormal() * InitialSpeed;
    }
    
}
void AProjectileBase::ActivateSkillActor(USkillBase* InSkill, AActor* InOwner, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    Super::ActivateSkillActor(InSkill, InOwner, SpawnLocation, SpawnRotation);

    CollisionComponent->OnComponentHit.RemoveDynamic(this, &AProjectileBase::OnHit);
    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

    CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AProjectileBase::OnOverlap);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);

    if (!bUseHorming)
    {
        MovementComponent->Velocity = SpawnRotation.Vector() * InitialSpeed;
    }
    else
    {
        MovementComponent->Velocity = InstigatorActor->GetActorForwardVector()* InitialSpeed;
    }
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    MovementComponent->bRotationFollowsVelocity = !bUseHorming;



    GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &AProjectileBase::DeactivateSkillActor, LifeTime, false);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bIsActive) return;
    if (!OtherActor || OtherActor == InstigatorActor.Get()) return;

    if (OwnerSkill)
    {
        if (const FLevelModifierEntry* LevelModifierEntry = OwnerSkill->GetCurrentLevelEntry())
        {
            if (InstigatorActor->GetClass()->ImplementsInterface(UStatModifierInterface::StaticClass()))
            {
                auto Interface = Cast<IStatModifierInterface>(InstigatorActor);
                Interface->AddLevelModifierEntry(*LevelModifierEntry);
            }

            if (OtherActor->GetClass()->ImplementsInterface(UStatModifierInterface::StaticClass()))
            {
                auto Interface = Cast<IStatModifierInterface>(OtherActor);
                if (LevelModifierEntry->OtherCharacterModifiers.Num() > LevelModifierEntry->Level - 1)
                {
                    // 계산 부분
                    FDerivedStatModifier StatModifier = LevelModifierEntry->OtherCharacterModifiers[LevelModifierEntry->Level - 1];

                    if (StatModifier.Duration <= 0.0f && !StatModifier.bIsPermanent) // 체인지 스탯함수 실행
                    {
                        Interface->ChangeStat(StatModifier.StatTag, StatModifier.Value.Flat * StatModifier.Value.Mul);
                    }
                    else // 모디파이어 추가
                    {
                        Interface->AddDerivedStatModifier(StatModifier);
                    }
                }
            }
        }
    }

    DeactivateSkillActor();
}

void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsActive) return;
    if (!OtherActor || OtherActor == InstigatorActor.Get()) return;

    if (OwnerSkill)
    {
        if (const FLevelModifierEntry* LevelModifierEntry = OwnerSkill->GetCurrentLevelEntry())
        {
            if (InstigatorActor->GetClass()->ImplementsInterface(UStatModifierInterface::StaticClass()))
            {
                auto Interface = Cast<IStatModifierInterface>(InstigatorActor);
                Interface->AddLevelModifierEntry(*LevelModifierEntry);
            }

            if (OtherActor->GetClass()->ImplementsInterface(UStatModifierInterface::StaticClass()))
            {
                auto Interface = Cast<IStatModifierInterface>(OtherActor);
                if (LevelModifierEntry->OtherCharacterModifiers.Num() > LevelModifierEntry->Level - 1)
                {
                    // 계산 부분
                    FDerivedStatModifier StatModifier = LevelModifierEntry->OtherCharacterModifiers[LevelModifierEntry->Level - 1];

                    if (StatModifier.Duration <= 0.0f && !StatModifier.bIsPermanent) // 체인지 스탯함수 실행
                    {
                        Interface->ChangeStat(StatModifier.StatTag, StatModifier.Value.Flat * StatModifier.Value.Mul);
                    }
                    else // 모디파이어 추가
                    {
                        Interface->AddDerivedStatModifier(StatModifier);
                    }
                }
            }
        }
    }

    DeactivateSkillActor();
}
