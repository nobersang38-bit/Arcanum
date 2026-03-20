// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectedArrow.generated.h"

UCLASS()
class ARCANUM_API ASelectedArrow : public AActor
{
	GENERATED_BODY()
	
#pragma region 언리얼 엔진 기본생성
public:
	// Sets default values for this actor's properties
	ASelectedArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
	UFUNCTION()
	void SetActive(bool IsActivate);

	UFUNCTION()
	void SetArrow(const FVector& StartLocation, const FVector& EndLocation);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> PlaneComponent = nullptr;


};
