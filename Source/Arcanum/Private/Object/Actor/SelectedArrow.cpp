// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Actor/SelectedArrow.h"

// Sets default values
ASelectedArrow::ASelectedArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	PlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));
	PlaneComponent->SetupAttachment(RootComponent);
	PlaneComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ASelectedArrow::BeginPlay()
{
	Super::BeginPlay();
	PlaneComponent->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
}

// Called every frame
void ASelectedArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectedArrow::SetActive(bool IsActivate)
{
	if (IsActivate)
	{
		SetActorHiddenInGame(false);
	}
	else
	{
		SetActorHiddenInGame(true);
	}
}

void ASelectedArrow::SetArrow(const FVector& StartLocation, const FVector& EndLocation)
{
	FVector Direction = EndLocation - StartLocation;
	float Length = Direction.Length() - 100.0f;
	Direction = Direction.GetSafeNormal();

	SetActorLocation(StartLocation);
	SetActorRotation(Direction.Rotation());

	PlaneComponent->SetRelativeLocation(FVector(50.0f + Length * 0.5f, 0.0f, 0.0f));
	PlaneComponent->SetRelativeScale3D(FVector(1.0f + ((Length * 0.5f) / 50.0f), 1.0f, 1.0f));
}

