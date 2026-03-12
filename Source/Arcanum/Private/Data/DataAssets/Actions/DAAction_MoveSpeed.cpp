// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/Actions/DAAction_MoveSpeed.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDAAction_MoveSpeed::StartAction(const FNonRegenStat& Value)
{
	Super::StartAction(Value);
	
	ACharacter* Character = Cast<ACharacter>(GetOuter());
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Value.GetTotalValue();
	}
}
