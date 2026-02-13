// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/AnimNotify_Attack.h"
#include "Interface/CombatInterface.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		if (MeshComp->GetOwner()->GetClass()->ImplementsInterface(UCombatInterface::StaticClass()))
		{
			auto Interface = Cast<ICombatInterface>(MeshComp->GetOwner());
			Interface->OnAttackNotifyTriggered();
		}
	}
}
