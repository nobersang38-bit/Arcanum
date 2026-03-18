#include "Object/Skills/SkillActor.h"
#include "Object/Skills/SkillBase.h"
#include "Core/SubSystem/PoolingSubsystem.h"

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
void ASkillActor::ActivateSkillActor(USkillBase* InSkill, AActor* InOwner, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    OwnerSkill = InSkill;
    InstigatorActor = InOwner;

    SetActorLocation(SpawnLocation);
    SetActorRotation(SpawnRotation);

    // 0312 풀링 활성화함수 실행 추가
    UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
    if (PoolingSubsystem)
    {
        PoolingSubsystem->ActivateItem(this);
    }
    // 0312 해당 위치에 있던 Activate로직 ActivateItem으로 옮김
}
void ASkillActor::DeactivateSkillActor()
{
    // 0312 해당 위치에 있던 Deactive로직 DeactiveItem으로 옮김

    ReturnToPool();
}
void ASkillActor::ReturnToPool()
{
    // 여기서 PoolManager에 반환
    // 0312 풀링으로 반환 추가
    UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
    if (PoolingSubsystem)
    {
        PoolingSubsystem->DeactiveItem(this);
    }
}

void ASkillActor::ActivateItem()
{
    bIsActive = true;
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);
}

void ASkillActor::DeactiveItem()
{
    // 0312 풀링 서브시스템에서 실행할 Deactive로직 추가
    bIsActive = false;

    SetActorEnableCollision(false);
    SetActorHiddenInGame(true);
}
