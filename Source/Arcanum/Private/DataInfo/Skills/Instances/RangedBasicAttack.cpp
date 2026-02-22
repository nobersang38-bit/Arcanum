#include "DataInfo/Skills/Instances/RangedBasicAttack.h"

void URangedBasicAttack::ActivateSkill(AActor* Instigator)
{

}

AActor* URangedBasicAttack::FindNearestTarget(AActor* InInstigator, float InRange) const
{
	return nullptr;
}

bool URangedBasicAttack::ApplyRangedBasicAttackDamage(AActor* InInstigator, AActor* InTarget, const FLevelModifierEntry& InEntry)
{
	return false;
}

void URangedBasicAttack::NotifyBasicAttackHitToSkillComponent(AActor* InInstigator, AActor* InTarget)
{

}
