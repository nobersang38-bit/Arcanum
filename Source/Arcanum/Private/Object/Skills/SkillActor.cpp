#include "Object/Skills/SkillActor.h"
#include "Object/Skills/SkillBase.h"

ASkillActor::ASkillActor()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
}

void ASkillActor::BeginPlay()
{
    Super::BeginPlay();
}

void ASkillActor::ActivateSkillActor(
    USkillBase* InSkill,
    AActor* InOwner,
    const FVector& SpawnLocation,
    const FRotator& SpawnRotation)
{
    OwnerSkill = InSkill;
    InstigatorActor = InOwner;

    SetActorLocation(SpawnLocation);
    SetActorRotation(SpawnRotation);

    bIsActive = true;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
}

void ASkillActor::DeactivateSkillActor()
{
    bIsActive = false;

    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);

    ReturnToPool();
}

void ASkillActor::ReturnToPool()
{
    // 여기서 PoolManager에 반환
}