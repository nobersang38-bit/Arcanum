// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/Actions/DAAction_MoveSpeed.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDAAction_MoveSpeed::StartAction(AActor* TargetActor, const FNonRegenStat& Value)
{
	Super::StartAction(TargetActor, Value);
	
	ACharacter* Character = Cast<ACharacter>(TargetActor);
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Value.GetTotalValue();
	}
}
