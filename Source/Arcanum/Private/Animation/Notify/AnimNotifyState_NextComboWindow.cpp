#include "Animation/Notify/AnimNotifyState_NextComboWindow.h"
#include "Character/PlayerCharacter.h"

void UAnimNotifyState_NextComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp)
	{
		if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			playerCharacter->EnableNextComboInput();
		}
	}
}

void UAnimNotifyState_NextComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (MeshComp)
	{
		if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		{
			playerCharacter->DisableNextComboInput();
		}
	}
}