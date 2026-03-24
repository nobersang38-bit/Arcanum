// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Actor/SkillRangeDecal.h"
#include "Components/DecalComponent.h"

// Sets default values
ASkillRangeDecal::ASkillRangeDecal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);

	CursorDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("CursorDecalComponent"));
	CursorDecalComponent->SetupAttachment(RootComponent);

	CursorDecalComponent->DecalSize = FVector(256.0f, 100.0f, 100.0f);
}

// Called when the game starts or when spawned
void ASkillRangeDecal::BeginPlay()
{
	Super::BeginPlay();
	SkillRangeDecalOff();
}

// Called every frame
void ASkillRangeDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOn && TargetActor.IsValid())
	{
		SetActorLocation(TargetActor->GetActorLocation());
	}
}

void ASkillRangeDecal::SkillRangeDecalOn(float InRadius, AActor* InTargetActor, const FVector& InCursorLocation)
{
	bIsOn = true;
	MainRadius = InRadius;
	DecalComponent->DecalSize = FVector(DecalComponent->DecalSize.X, MainRadius, MainRadius);
	TargetActor = InTargetActor;
	SetCursorLocation(InCursorLocation);
	SetActorHiddenInGame(false);
}

void ASkillRangeDecal::SkillRangeDecalOff()
{
	bIsOn = false;
	SetActorHiddenInGame(true);
}

FVector ASkillRangeDecal::SetCursorLocation(const FVector& InCursorLocation)
{
	FVector2D CursorLocation2D = FVector2D(InCursorLocation);
	FVector2D ActorLocation2D = FVector2D(GetActorLocation());

	FVector ResultLocation = FVector::ZeroVector;
	if ((CursorLocation2D - ActorLocation2D).Length() > MainRadius)
	{
		FVector2D Temp = ((CursorLocation2D - ActorLocation2D).GetSafeNormal() * MainRadius);
		FVector Temp2 = GetActorLocation() + FVector(Temp.X, Temp.Y, GetActorLocation().Z + 200);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Temp2, Temp2 + (FVector::UpVector * -9999.0f), ECollisionChannel::ECC_WorldStatic);
		ResultLocation = HitResult.ImpactPoint;
	}
	else
	{
		ResultLocation = InCursorLocation;
	}

	CursorDecalComponent->SetWorldLocation(ResultLocation);

	return ResultLocation;
}

