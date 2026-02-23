// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Actor/BattlefieldManagerActor.h"
//#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Data/Rows/AllyUnitsDataRow.h"
#include "Data/Rows/EnemyUnitsDataRow.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameplayTags/ArcanumTags.h"

#pragma region FUnitISMData
FUnitISMData::FUnitISMData(AActor* InUnit, int32 InID, class UInstancedStaticMeshComponent* InComponent, FGameplayTag InComponentTag)
{
	Unit = InUnit;
	ID = InID;
	Component = InComponent;
	ComponentTag = InComponentTag;
}
#pragma endregion

// Sets default values
ABattlefieldManagerActor::ABattlefieldManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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

	//const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

	//if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ABattlefieldManagerActor, AllyUnitData) || 
	//	MemberPropertyName == GET_MEMBER_NAME_CHECKED(ABattlefieldManagerActor, EnemyUnitData))
	//{
	//	DataSet();
	//}
}
#endif

// Called every frame
void ABattlefieldManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TSet<UInstancedStaticMeshComponent*> UpdateComponents;

	TArray<AActor*> ActorsToRemove;

	for (const auto& UnitISMData : UnitISMDatas)
	{
		const FUnitISMData& Data = UnitISMData.Value;
		AActor* UnitActor = UnitISMData.Key;

		if (IsValid(UnitActor) && Data.ID != -1 && Data.Component.IsValid())
		{
			UpdateComponents.Add(Data.Component.Get());

			FTransform Transform;
			Transform = UnitActor->GetActorTransform();
			Transform.SetScale3D(FVector(Data.StaticData.MeshScale, Data.StaticData.MeshScale, Data.StaticData.MeshScale));
			Transform.SetLocation(UnitActor->GetActorLocation() + FVector(0, 0, Data.StaticData.AddHeight));

			Data.Component->UpdateInstanceTransform(Data.ID, Transform, true, false, true);
		}
		else
		{
			if (UnitISMData.Key)
			{
				ActorsToRemove.Add(UnitISMData.Key);
			}
		}
	}

	for (AActor* DeadActor : ActorsToRemove)
	{
		if (!UnitISMDatas.Contains(DeadActor)) continue;

		FUnitISMData& TargetData = UnitISMDatas[DeadActor];
		UInstancedStaticMeshComponent* Component = TargetData.Component.Get();

		if (!Component)
		{
			UnitISMDatas.Remove(DeadActor);
			continue;
		}

		int32 TargetIndex = TargetData.ID;
		int32 LastIndex = Component->GetInstanceCount() - 1;

		if (TargetIndex < 0 || TargetIndex > LastIndex)
		{
			UnitISMDatas.Remove(DeadActor);
			continue;
		}

		if (TargetIndex != LastIndex)
		{
			FTransform LastInstanceTransform;
			Component->GetInstanceTransform(LastIndex, LastInstanceTransform, true);

			Component->UpdateInstanceTransform(TargetIndex, LastInstanceTransform, true, false, true);

			for (int32 i = 0; i < Component->NumCustomDataFloats; ++i)
			{
				float LastValue = Component->PerInstanceSMCustomData[LastIndex * Component->NumCustomDataFloats + i];
				Component->SetCustomDataValue(TargetIndex, i, LastValue, false);
			}

			for (auto& Pair : UnitISMDatas)
			{
				if (Pair.Value.Component == Component && Pair.Value.ID == LastIndex)
				{
					Pair.Value.ID = TargetIndex;
					break;
				}
			}
		}

		Component->RemoveInstance(LastIndex);
		UpdateComponents.Add(Component);
		UnitISMDatas.Remove(DeadActor);
	}

	for (UInstancedStaticMeshComponent* ISM : UpdateComponents)
	{
		if (IsValid(ISM))
		{
			ISM->MarkRenderStateDirty();
			ISM->UpdateBounds();
		}
	}
}

void ABattlefieldManagerActor::SpawnStaticMesh(FGameplayTag InTag, AActor* InOwner)
{
	if (UnitISMDatas.Contains(InOwner)) return;
	if (!InOwner) return;

	if (UInstancedStaticMeshComponent* ISM = *ISMs.Find(InTag))
	{
		const FISMStaticlData* ISMStaticData = StaticDatas.Find(InTag);
		if (ISMStaticData)
		{
			FTransform Transform;
			if (InOwner)
			{
				Transform.SetLocation(InOwner->GetActorLocation() + FVector(0, 0, ISMStaticData->AddHeight));
				Transform.SetRotation(InOwner->GetActorQuat());
				Transform.SetScale3D(FVector(ISMStaticData->MeshScale, ISMStaticData->MeshScale, ISMStaticData->MeshScale));
			}
			int32 ID = ISM->AddInstance(Transform, true);
			FUnitISMData UnitISMData = FUnitISMData(InOwner, ID, ISM, InTag);
			UnitISMData.StaticData = *ISMStaticData;

			ISM->SetCustomDataValue(ID, ISMStaticData->StartFrameParameterIDX, ISMStaticData->StartFrameParameterDefaultValue, false);
			ISM->SetCustomDataValue(ID, ISMStaticData->EndFrameParameterIDX, ISMStaticData->EndFrameParameterDefaultValue, false);

			UnitISMDatas.Add(InOwner, UnitISMData);
		}
		
	}
}

void ABattlefieldManagerActor::SetChangeAnimation(AActor* InOwner, FVector2D FrameRange)
{
	if (FUnitISMData* UnitISMData = UnitISMDatas.Find(InOwner))
	{
		const FISMStaticlData* ISMMaterialData = StaticDatas.Find(UnitISMData->ComponentTag);
		if (ISMMaterialData)
		{
			UnitISMData->Component->SetCustomDataValue(UnitISMData->ID, ISMMaterialData->StartFrameParameterIDX, FrameRange.X, false);
			UnitISMData->Component->SetCustomDataValue(UnitISMData->ID, ISMMaterialData->EndFrameParameterIDX, FrameRange.Y, false);
		}
	}
}

void ABattlefieldManagerActor::DataSet()
{
	for (const auto& ISM : ISMs)
	{
		ISM.Value->UnregisterComponent();
		ISM.Value->DestroyComponent();
	}
	ISMs.Empty();

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
				UInstancedStaticMeshComponent* ISM = nullptr;
				ISM = NewObject<UInstancedStaticMeshComponent>(this, UInstancedStaticMeshComponent::StaticClass());

				if (ISM)
				{
					ISM->RegisterComponent();
					ISM->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}

				//실제 세팅 부분
				if (ISM)
				{
					FISMStaticlData ISMMaterialData;
					ISMMaterialData.StartFrameParameterIDX = OutAllyUnits[i]->UnitData.Info.AnimSetting.StartFramePropertyIDX;
					ISMMaterialData.StartFrameParameterDefaultValue = OutAllyUnits[i]->UnitData.Info.AnimSetting.DefaultAnimFrameRange.X;
					ISMMaterialData.EndFrameParameterIDX = OutAllyUnits[i]->UnitData.Info.AnimSetting.EndFramePropertyIDX;
					ISMMaterialData.EndFrameParameterDefaultValue = OutAllyUnits[i]->UnitData.Info.AnimSetting.DefaultAnimFrameRange.Y;
					ISMMaterialData.AddHeight = OutAllyUnits[i]->UnitData.Info.AnimSetting.MeshAddHeight;
					ISMMaterialData.MeshScale = OutAllyUnits[i]->UnitData.Info.AnimSetting.MeshScale;

					StaticDatas.Add(UnitTag, ISMMaterialData);
					ISM->SetStaticMesh(OutAllyUnits[i]->UnitData.Info.AnimSetting.StaticMesh);
					ISM->SetNumCustomDataFloats(OutAllyUnits[i]->UnitData.Info.AnimSetting.MaterialNum);
					ISMs.Add(UnitTag, ISM);
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
			if (OutEnemyUnits[i]&& OutEnemyUnits[i]->UnitData.Info.AnimSetting.AnimMode == EAnimMode::AnimToTexture)
			{
				FGameplayTag UnitTag = OutEnemyUnits[i]->UnitData.Info.InfoSetting.Tag;
				FName CompName = FName(FString::Printf(TEXT("%s"), *OutEnemyUnits[i]->UnitData.Info.InfoSetting.Name.ToString()));

				UInstancedStaticMeshComponent* ISM = nullptr;
				ISM = NewObject<UInstancedStaticMeshComponent>(this, UInstancedStaticMeshComponent::StaticClass());

				if (ISM)
				{
					ISM->RegisterComponent();
					ISM->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
				if (ISM)
				{
					FISMStaticlData ISMMaterialData;
					ISMMaterialData.StartFrameParameterIDX = OutEnemyUnits[i]->UnitData.Info.AnimSetting.StartFramePropertyIDX;
					ISMMaterialData.StartFrameParameterDefaultValue = OutEnemyUnits[i]->UnitData.Info.AnimSetting.DefaultAnimFrameRange.X;
					ISMMaterialData.EndFrameParameterIDX = OutEnemyUnits[i]->UnitData.Info.AnimSetting.EndFramePropertyIDX;
					ISMMaterialData.EndFrameParameterDefaultValue = OutEnemyUnits[i]->UnitData.Info.AnimSetting.DefaultAnimFrameRange.Y;

					StaticDatas.Add(UnitTag, ISMMaterialData);
					ISM->SetStaticMesh(OutEnemyUnits[i]->UnitData.Info.AnimSetting.StaticMesh);
					ISM->SetNumCustomDataFloats(OutEnemyUnits[i]->UnitData.Info.AnimSetting.MaterialNum);
					ISMs.Add(UnitTag, ISM);
				}
			}
		}
	}
}