// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HologramVisualizer.generated.h"

UCLASS()
class ARCANUM_API AHologramVisualizer : public AActor
{
	GENERATED_BODY()
#pragma region 언리얼 엔진 기본 생성
public:	
	// Sets default values for this actor's properties
	AHologramVisualizer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;
};
