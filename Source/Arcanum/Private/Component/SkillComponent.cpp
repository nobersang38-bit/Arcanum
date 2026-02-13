#include "Component/SkillComponent.h"
#include "Component/EquipmentComponent.h"
#include "GameplayTags/ArcanumTags.h"
#include "GameplayTagsManager.h"
#include "DataInfo/Skills/Instances/USkillBase.h"

USkillComponent::USkillComponent()
{
}

void USkillComponent::BeginPlay()
{
}

bool USkillComponent::AcquireSkill(const FGameplayTag& SkillIDTag)
{
	return false;
}

bool USkillComponent::RemoveSkill(const FGameplayTag& SkillIDTag)
{
	return false;
}

USkillBase* USkillComponent::GetSkill(const FGameplayTag& SkillIDTag) const
{
	return nullptr;
}

bool USkillComponent::ActivateSkill(const FGameplayTag& SkillIDTag)
{
	return false;
}

FTransform USkillComponent::GetSkillSpawnTransform(const FSkillInfo& SkillData) const
{
	return FTransform();
}

void USkillComponent::BindEquipmentEvents()
{
}

void USkillComponent::RefreshCurrentSkillTags()
{
}
