// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BasementTeamComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"

// Sets default values for this component's properties
UBasementTeamComponent::UBasementTeamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBasementTeamComponent::BeginPlay()
{
	Super::BeginPlay();
	UBattlefieldManagerSubsystem* BattlefieldManagerSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (BattlefieldManagerSubsystem)
	{
		BattlefieldManagerSubsystem->AddBasement(GetOwner(), TeamTag);
	}
}


// Called every frame
void UBasementTeamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

