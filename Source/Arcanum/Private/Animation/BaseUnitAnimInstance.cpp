// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BaseUnitAnimInstance.h"
#include "GameFramework/Character.h"

UBaseUnitAnimInstance::UBaseUnitAnimInstance()
{

}

void UBaseUnitAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (APawn* OwnerPawn = TryGetPawnOwner())
	{
		OwnerCharacter = Cast<ACharacter>(OwnerPawn);
	}

	Internal_UpdateInterval = UpdateInterval;
	Internal_UpdateInterval = FMath::FRandRange(0.0f, 0.1f);
	float StartDelay = FMath::FRandRange(0.0f, 0.25f);

	GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, this, &UBaseUnitAnimInstance::TickUpdate, Internal_UpdateInterval, true, StartDelay);
}

void UBaseUnitAnimInstance::TickUpdate()
{
	if (OwnerCharacter.IsValid())
	{
		Velocity = OwnerCharacter->GetVelocity().SquaredLength();
	}
}

