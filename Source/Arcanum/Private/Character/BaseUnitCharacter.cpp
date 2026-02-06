// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseUnitCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABaseUnitCharacter::ABaseUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(0.0f);
}

// Called when the game starts or when spawned
void ABaseUnitCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseUnitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseUnitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

