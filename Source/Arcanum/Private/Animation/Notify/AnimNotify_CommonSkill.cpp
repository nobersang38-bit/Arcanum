#include "Animation/Notify/AnimNotify_CommonSkill.h"
#include "Character/PlayerCharacter.h"
#include "UI/Battle/BattlePlayerController.h"

void UAnimNotify_CommonSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp)
	{
		if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (ABattlePlayerController* battlePlayerController = playerCharacter->GetBattleOwnerController())
			{
				battlePlayerController->TriggerSkill();
			}
		}
	}
}
