// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Actor/HologramVisualizer.h"

// Sets default values
AHologramVisualizer::AHologramVisualizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletaMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHologramVisualizer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHologramVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

