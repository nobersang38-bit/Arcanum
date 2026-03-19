// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NativeGameplayTags.h"
#include "SpawnCheckDecal.generated.h"
//김도현
UCLASS()
class ARCANUM_API ASpawnCheckDecal : public AActor
{
	GENERATED_BODY()
#pragma region 언리얼 엔진 기본 생성
public:
	// Sets default values for this actor's properties
	ASpawnCheckDecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
	UFUNCTION()
	void SetSpawnDecalActive(bool bIsOn);

	UFUNCTION()
	FVector CalculateMinDistanceLocation(const FVector& InCheckLocation, bool& OutIsInSide);

protected:
	void SetSpawnPossibleSize(FVector2D InSize);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UDecalComponent> DecalComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UBoxComponent> BoxComponent = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<UDataTable> StageInfoDataTable = nullptr;

#pragma region 디버그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bUseDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	FGameplayTag DebugStageTag;
#pragma endregion

private:
	FGameplayTag CurrentStageTag;
};
