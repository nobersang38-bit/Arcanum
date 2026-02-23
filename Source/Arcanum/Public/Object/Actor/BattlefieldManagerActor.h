// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "BattlefieldManagerActor.generated.h"

UCLASS()
class ARCANUM_API ABattlefieldManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattlefieldManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
#pragma region Debug
	UFUNCTION(CallInEditor)
	void Debug_StaticMeshSpawn();
#pragma endregion
	void SpawnStaticMesh(FGameplayTag InTag, AActor* GetOwner);
	void SpawnStaticMesh(FGameplayTag InTag, const FTransform& InSpawnWorldLocation);

protected:
	void DataSet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> AllyUnitData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> EnemyUnitData = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<class UHierarchicalInstancedStaticMeshComponent>> HISMs;

};
