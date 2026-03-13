// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/Actions/DAAction_MoveSpeed.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDAAction_MoveSpeed::StartAction(const FNonRegenStat& Value)
{
	if (!IsEnable(Value)) return;

	UE_LOG(LogTemp, Error, TEXT("달리기 속도!!! %s"), *GetOuter()->GetName());
	ACharacter* Character = Cast<ACharacter>(GetOuter());
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Value.GetTotalValue();
	}
}
