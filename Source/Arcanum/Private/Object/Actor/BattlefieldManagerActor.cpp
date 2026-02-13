// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Actor/BattlefieldManagerActor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Data/Rows/AllyUnitsDataRow.h"
#include "Data/Rows/EnemyUnitsDataRow.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameplayTags/ArcanumTags.h"

// Sets default values
ABattlefieldManagerActor::ABattlefieldManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABattlefieldManagerActor::BeginPlay()
{
	Super::BeginPlay();
	DataSet();
	UBattlefieldManagerSubsystem* BattlefieldManagerSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if(BattlefieldManagerSubsystem)
	{
		BattlefieldManagerSubsystem->SetABattlefieldManagerActor(this);
	}
}

#if WITH_EDITOR
void ABattlefieldManagerActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ABattlefieldManagerActor, AllyUnitData) || 
		MemberPropertyName == GET_MEMBER_NAME_CHECKED(ABattlefieldManagerActor, EnemyUnitData))
	{
		//DataSet();
	}
}
#endif

// Called every frame
void ABattlefieldManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABattlefieldManagerActor::SpawnStaticMesh(FGameplayTag InTag, AActor* GetOwner)
{
	if (UHierarchicalInstancedStaticMeshComponent* HISM = *HISMs.Find(InTag))
	{
		FTransform Transform;
		if (GetOwner)
		{
			Transform.SetLocation(GetOwner->GetActorLocation());
			Transform.SetRotation(GetOwner->GetActorQuat());
		}
		HISM->AddInstance(Transform);
	}
}

void ABattlefieldManagerActor::SpawnStaticMesh(FGameplayTag InTag, const FTransform& InSpawnWorldLocation)
{
	if (UHierarchicalInstancedStaticMeshComponent* HISM = *HISMs.Find(InTag))
	{
		HISM->AddInstance(InSpawnWorldLocation);
	}
}

#pragma region Debug
void ABattlefieldManagerActor::Debug_StaticMeshSpawn()
{
	UE_LOG(LogTemp, Warning, TEXT("무야호"));
	FTransform Transform;
	for (int i = 0; i < 200; i++)
	{
		SpawnStaticMesh(Arcanum::Unit::Faction::Ally::Army::Root, Transform);
	}
}
#pragma endregion



void ABattlefieldManagerActor::DataSet()
{
	for (auto HISM : HISMs)
	{
		HISM.Value->UnregisterComponent();
		HISM.Value->DestroyComponent();
	}
	HISMs.Empty();

	if (AllyUnitData)
	{
		TArray<FAllyUnitsDataRow*> OutAllyUnits;
		AllyUnitData->GetAllRows<FAllyUnitsDataRow>(TEXT(""), OutAllyUnits);

		for (int i = 0; i < OutAllyUnits.Num(); i++)
		{
			if (OutAllyUnits[i])
			{
				FGameplayTag UnitTag = OutAllyUnits[i]->UnitData.Info.InfoSetting.Tag;
				FName CompName = FName(FString::Printf(TEXT("%s"), *OutAllyUnits[i]->UnitData.Info.InfoSetting.Name.ToString()));
				UHierarchicalInstancedStaticMeshComponent* HISM = nullptr;
				HISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, UHierarchicalInstancedStaticMeshComponent::StaticClass());

				if (HISM)
				{
					HISM->RegisterComponent();
					HISM->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}

				if (HISM)
				{
					HISM->SetStaticMesh(OutAllyUnits[i]->UnitData.Info.AnimSetting.StaticMesh);
					HISMs.Add(UnitTag, HISM);
				}
			}
		}
	}

	if (EnemyUnitData)
	{
		TArray<FEnemyUnitsDataRow*> OutEnemyUnits;
		EnemyUnitData->GetAllRows<FEnemyUnitsDataRow>(TEXT(""), OutEnemyUnits);

		for (int i = 0; i < OutEnemyUnits.Num(); i++)
		{
			if (OutEnemyUnits[i])
			{
				FGameplayTag UnitTag = OutEnemyUnits[i]->UnitData.Info.InfoSetting.Tag;
				FName CompName = FName(FString::Printf(TEXT("%s"), *OutEnemyUnits[i]->UnitData.Info.InfoSetting.Name.ToString()));

				UHierarchicalInstancedStaticMeshComponent* HISM = nullptr;
				HISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this, UHierarchicalInstancedStaticMeshComponent::StaticClass());

				if (HISM)
				{
					HISM->RegisterComponent();
					HISM->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
				if (HISM)
				{
					HISM->SetStaticMesh(OutEnemyUnits[i]->UnitData.Info.AnimSetting.StaticMesh);
					HISMs.Add(UnitTag, HISM);
				}
			}
		}
	}
}

