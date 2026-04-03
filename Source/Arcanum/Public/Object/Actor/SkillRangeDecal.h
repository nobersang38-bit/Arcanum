// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillRangeDecal.generated.h"

//김도현
UCLASS()
class ARCANUM_API ASkillRangeDecal : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASkillRangeDecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UDecalComponent> DecalComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<class UDecalComponent> CursorDecalComponent = nullptr;

public:
	void SkillRangeDecalOn(float InRadius, AActor* InTargetActor, const FVector& InCursorLocation);
	void SkillRangeDecalOn(float InRadius);
	void SkillRangeDecalOff();
	FVector SetCursorLocation(const FVector& InCursorLocation);

protected:
	float MainRadius = 0.0f;

	bool bIsOn = false;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor = nullptr;
};
