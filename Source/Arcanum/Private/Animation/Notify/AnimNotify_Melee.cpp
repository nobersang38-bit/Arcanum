#include "Animation/Notify/AnimNotify_Melee.h"
#include "Component/SkillComponent.h"

void UAnimNotify_Melee::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp)
	{
		AActor* owner = MeshComp->GetOwner();

		if (owner)
		{
			USkillComponent* skillComponent = owner->FindComponentByClass<USkillComponent>();

			if (skillComponent)
			{
				const FGameplayTag basicAttackSkillTag = skillComponent->GetBasicAttackSkillTag();

				if (basicAttackSkillTag.IsValid())
				{
					// 근거리 기본공격 실행
					skillComponent->ActivateSkill(basicAttackSkillTag);
				}
			}
		}
	}
}
