// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/StatusAction/StatusAction_MoveSpeed.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UStatusAction_MoveSpeed::UStatusAction_MoveSpeed()
{
	EnableTagMergeType = EStatusActionTagType::OnlyStatusActionCompTags;
}

void UStatusAction_MoveSpeed::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	Super::StartAction_Implementation(RegenStat, NonRegenStat);
	ACharacter* Character = Cast<ACharacter>(GetOuter());
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = NonRegenStat.GetTotalValue();
	}
}
