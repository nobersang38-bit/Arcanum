// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Actor/SpawnCheckDecal.h"
#include "Components/DecalComponent.h"
#include "Data/Rows/DTBattleStageInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnCheckDecal::ASpawnCheckDecal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);

	DecalComponent->SetRelativeLocation(FVector(0.0f, 1500.0f, 0.0f));
	DecalComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	DecalComponent->DecalSize = FVector(256.0f, 1500.0f, 900.0f);
	DecalComponent->DecalColor = FLinearColor(0.0f, 10.0f, 0.0f, 0.011f);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(DecalComponent);

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

// Called when the game starts or when spawned
void ASpawnCheckDecal::BeginPlay()
{
	Super::BeginPlay();
	TArray<FDTBattleStageInfo*> BattleStageInfos;
	if (UARGameInstance* GameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (bUseDebug)
		{
			CurrentStageTag = DebugStageTag;
		}
		else
		{
			CurrentStageTag = GameInstance->CurrentStageTag;
		}

		if (StageInfoDataTable)
		{
			StageInfoDataTable->GetAllRows<FDTBattleStageInfo>(TEXT(""), BattleStageInfos);
			for (int i = 0; i < BattleStageInfos.Num(); i++)
			{
				if (CurrentStageTag.MatchesTag(BattleStageInfos[i]->StageTag) && CurrentStageTag.IsValid())
				{
					SetSpawnPossibleSize(BattleStageInfos[i]->BattleStageInfo.SpawnPossibleSize);
					break;
				}
			}
		}
	}

	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (BattleSubsystem)
	{
		BattleSubsystem->SetSpawnCheckDecal(this);
	}
	SetSpawnDecalActive(false);
}

// Called every frame
void ASpawnCheckDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnCheckDecal::SetSpawnDecalActive(bool bIsOn)
{
	SetActorHiddenInGame(!bIsOn);
}

FVector ASpawnCheckDecal::CalculateMinDistanceLocation(const FVector& InCheckLocation, bool& OutIsInSide)
{
	FVector ResultLocation;
	BoxComponent->GetClosestPointOnCollision(InCheckLocation, ResultLocation);
	OutIsInSide = BoxComponent->Bounds.GetBox().IsInside(InCheckLocation);
	return ResultLocation;
}

void ASpawnCheckDecal::SetSpawnPossibleSize(FVector2D InSize)
{
	DecalComponent->DecalSize = FVector(256.0f, InSize.X, InSize.Y);
	DecalComponent->SetRelativeLocation(FVector(0.0f, InSize.X, 0.0f));
	BoxComponent->SetBoxExtent(DecalComponent->DecalSize);
}

