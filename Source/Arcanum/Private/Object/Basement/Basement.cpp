// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Basement/Basement.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABasement::ABasement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentBasementHealth = MaxBasementHealth;

	//CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	//CapsuleComponent->InitCapsuleSize(42.f, 96.8f);
	//SetRootComponent(CapsuleComponent);

	//MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//MeshComponent->SetupAttachment(RootComponent);

	// 캡슐 말고 스태틱 메시의 콜리전 사용
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	//Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh"));
	//Mesh->SetupAttachment(RootComponent);
	//Mesh->SetCollisionProfileName(TEXT("BlockAll"));
}

// Called when the game starts or when spawned
void ABasement::BeginPlay()
{
	Super::BeginPlay();
	
	//if (bIsFriendly)
	//{
	//	MaxBasementHealth = FriendlyMaxHealth;
	//}
	//else
	//{
	//	MaxBasementHealth = EnemyMaxHealth;
	//}

	//CurrentBasementHealth = MaxBasementHealth;

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

bool ABasement::IsBaseDestroyed() const
{
	return false;
}


