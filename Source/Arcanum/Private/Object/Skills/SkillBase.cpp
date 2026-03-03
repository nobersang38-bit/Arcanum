#include "Object/Skills/SkillBase.h"
#include "GameFramework/Actor.h"

USkillBase::USkillBase()
{
}

void USkillBase::Initialize(AActor* InOwner, const FSkillInfo* InSkillInfo, int32 InLevel)
{
    if (!InOwner || !InSkillInfo)
    {
        return;
    }

    OwnerActor = InOwner;
    SkillInfo = InSkillInfo;
    SkillState.Level = InLevel;
}

const FLevelModifierEntry* USkillBase::GetCurrentLevelEntry() const
{
    if (SkillState.Level <= 0)
    {
        return nullptr;
    }

    for (const FLevelModifierEntry& Entry : SkillInfo->LevelModifiers)
    {
        if (Entry.Level == SkillState.Level)
        {
            return &Entry;
        }
    }

    return nullptr;
}

void USkillBase::ActivateSkill(AActor* Instigator)
{
    //if (!SkillActorClass.IsNull())
    //{
    //    ASkillActor* Actor = PoolManager->Acquire(SkillActorClass);
    //    Actor->Initialize(this, OwnerActor);
    //}
}

void USkillBase::DeactivateSkill(AActor* Instigator)
{
   
}