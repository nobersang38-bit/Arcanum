// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Basement/Basement.h"

// Sets default values
ABasement::ABasement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasement::BeginPlay()
{
	Super::BeginPlay();
	
}
void ABasement::EnemySpawnTick()
{
}
void ABasement::TrySpawnEnemyUnit()
{
}
void ABasement::TakeDamageToBasement(float Damage)
{
}
bool ABasement::SpawnUnit(int32 Index)
{
	return false;
}
void ABasement::AddResource(int32 Amount)
{
}
bool ABasement::ConsumeResource(int32 Amount)
{
	return false;
}
