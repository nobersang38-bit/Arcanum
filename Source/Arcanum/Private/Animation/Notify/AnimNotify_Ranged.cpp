#include "Animation/Notify/AnimNotify_Ranged.h"
#include "Component/SkillComponent.h"

void UAnimNotify_Ranged::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
					// 원거리 기본공격 실행
					// 현재 테스트 단계에서는 즉시 처리, 나중엔 프로젝타일 발사로 교체
					skillComponent->ActivateSkill(basicAttackSkillTag);
				}
			}
		}
	}
}
