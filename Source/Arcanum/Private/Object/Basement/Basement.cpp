// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Basement/Basement.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

// Sets default values
ABasement::ABasement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//CurrentBasementHealth = MaxBasementHealth;

	// 캡슐 말고 스태틱 메시의 콜리전 사용
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
}

// Called when the game starts or when spawned
void ABasement::BeginPlay()
{
	Super::BeginPlay();

	//if (CommonEnemyClass)
	//{
	//	GetWorld()->GetTimerManager().SetTimer(
	//		SpawnTimerHandle,
	//		this,
	//		&ABasement::EnemySpawnTick,
	//		SpawnCommonInterval,
	//		true
	//	);
	//}
}

//void ABasement::EnemySpawnTick()
//{
//	 if (SpawnCommonCount >= MaxSpawnCommonCount)
//    {
//        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
//        return;
//    }
//
//	 SpawnCommonCount++;
//
//  //  GetWorld()->SpawnActor<ACharacter>(
//		//CommonEnemyClass,
//		//SpawnLocation,
//		//FRotator::ZeroRotator
//  //  );
//	 ACharacter* SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(
//		 CommonEnemyClass,
//		 SpawnLocation,
//		 FRotator::ZeroRotator
//	 );
//}
//
//void ABasement::TrySpawnEnemyUnit()
//{
//}
//
//void ABasement::TakeDamageToBasement(float Damage)
//{
//	CurrentBasementHealth = CurrentBasementHealth - Damage;
//
//	if (OnBasementHealthChanged.IsBound())
//	{
//		OnBasementHealthChanged.Broadcast(CurrentBasementHealth, MaxBasementHealth);
//	}
//}
//
//bool ABasement::SpawnUnit(int32 Index)
//{
//	return false;
//}
//
//void ABasement::AddResource(int32 Amount)
//{
//}
//
//bool ABasement::ConsumeResource(int32 Amount)
//{
//	return false;
//}
//
//bool ABasement::IsBaseDestroyed() const
//{
//	return false;
//}


