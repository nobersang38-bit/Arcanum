#include "Animation/Notify/AnimNotify_BasicAttackHit.h"
#include "Character/PlayerCharacter.h"
#include "UI/Battle/BattlePlayerController.h"

void UAnimNotify_BasicAttackHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp)
	{
		if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			if (ABattlePlayerController* battlePlayerController = playerCharacter->GetController<ABattlePlayerController>())
			{
				battlePlayerController->TriggerBasicAttackHit();
			}
		}
	}
}
