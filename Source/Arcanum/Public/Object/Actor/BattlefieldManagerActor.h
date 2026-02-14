// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "BattlefieldManagerActor.generated.h"

USTRUCT()
struct FISMStaticlData
{
	GENERATED_BODY()
public:
	int32 StartFrameParameterIDX = 0;
	float StartFrameParameterDefaultValue = 0.0f;

	int32 EndFrameParameterIDX = 0;
	float EndFrameParameterDefaultValue = 0.0f;

	float AddHeight = 0.0f;
	float MeshScale = 1.0f;
};

USTRUCT()
struct FUnitISMData
{
	GENERATED_BODY()
public:
	FUnitISMData() = default;
	FUnitISMData(AActor* InUnit, int32 InID, class UInstancedStaticMeshComponent* InComponent, FGameplayTag InComponentTag);

	UPROPERTY()
	TWeakObjectPtr<AActor> Unit = nullptr;

	int32 ID = -1;

	UPROPERTY()
	TWeakObjectPtr<class UInstancedStaticMeshComponent> Component = nullptr;

	FGameplayTag ComponentTag = FGameplayTag::EmptyTag;

	FISMStaticlData StaticData;
};

// 김도현
// 유닛 캐릭터 ISM관리 매니저 액터
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
	// 해당 함수로 스폰하면 관리 대상이 됩니다, 자동으로 트랜스폼 업데이트 됩니다
	UFUNCTION(BlueprintCallable)
	void SpawnStaticMesh(FGameplayTag InTag, AActor* InOwner);

	void SetChangeAnimation(AActor* InOwner, FVector2D FrameRange);

protected:
	void DataSet();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> AllyUnitData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> EnemyUnitData = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<class UInstancedStaticMeshComponent>> ISMs;

private:
	UPROPERTY()
	TMap<AActor*, FUnitISMData> UnitISMDatas;

	TMap<FGameplayTag, FISMStaticlData> StaticDatas;
};
