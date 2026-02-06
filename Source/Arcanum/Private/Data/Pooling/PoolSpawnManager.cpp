// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Pooling/PoolSpawnManager.h"
#include "Component/PoolingComponent.h"

UObject* UPoolSpawnManager::Spawn(const FTransform& InTransform)
{
	SpawnTransform = InTransform;

	switch (SpawnType)
	{
	case ESpawnType::None:
		UE_LOG(LogTemp, Error, TEXT("UPoolSpawnData::Spawnd의 SpawnType이 None 입니다"));
		break;
	case ESpawnType::Actor:
		return ActorSpawn();
		break;
	case ESpawnType::SceneComponent:
		return SceneComponentSpawn();
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("UPoolSpawnData::Spawnd의 스위치에서 default로 넘어옴"));
		break;
	}
	return nullptr;
}

AActor* UPoolSpawnManager::ActorSpawn()
{
	UE_LOG(LogTemp, Log, TEXT("ActorSpawn이 실행되었다"))
	return GetWorld()->SpawnActor(SpawnClass, &SpawnTransform);
}

USceneComponent* UPoolSpawnManager::SceneComponentSpawn()
{
	USceneComponent* SceneComp = nullptr;
	if (PoolingComponent)
	{
		SceneComp = NewObject<USceneComponent>(PoolingComponent, SpawnClass);
	}

	if (SceneComp)
	{
		SceneComp->RegisterComponent();
		SceneComp->AttachToComponent(PoolingComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	return SceneComp;
}
