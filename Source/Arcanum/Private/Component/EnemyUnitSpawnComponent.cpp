// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyUnitSpawnComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"

// Sets default values for this component's properties
UEnemyUnitSpawnComponent::UEnemyUnitSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEnemyUnitSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	UBattlefieldManagerSubsystem* BattlefieldManagerSubSystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (BattlefieldManagerSubSystem)
	{
		EnemyWaveData = BattlefieldManagerSubSystem->GetInBattleData().EnemyWaveDataInfo;
	}
}

#if WITH_EDITOR
void UEnemyUnitSpawnComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	//const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

	//if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UEnemyUnitSpawnComponent, DTUnitDataRowHandle))
	//{
	//	if (DTUnitDataRowHandle.DataTable && !DTUnitDataRowHandle.RowName.IsNone())
	//	{
	//		if (const FAllyUnitsDataRow* AllyRow = DTUnitDataRowHandle.DataTable->FindRow<FAllyUnitsDataRow>(DTUnitDataRowHandle.RowName, TEXT("Load")))
	//		{
	//			if (AllyRow) UnitData = (*AllyRow).UnitData;
	//		}
	//		else if (const FEnemyUnitsDataRow* EnemyRow = DTUnitDataRowHandle.DataTable->FindRow<FEnemyUnitsDataRow>(DTUnitDataRowHandle.RowName, TEXT("Load")))
	//		{
	//			if (EnemyRow) UnitData = (*EnemyRow).UnitData;
	//		}
	//	}
	//}
}
#endif


// Called every frame
void UEnemyUnitSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyUnitSpawnComponent::WaveStart()
{
	for (int i = 0; i < EnemyWaveData.EnemyUnitsSet.Num(); i++)
	{

	}
}

