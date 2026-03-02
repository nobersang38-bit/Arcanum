// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyUnitSpawnComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"
#include "Data/Rows/DTBattleStageInfo.h"
#include "Data/Rows/EnemyUnitsDataRow.h"
#include "Character/BaseUnitCharacter.h"
#include "Core/SubSystem/PoolingSubsystem.h"

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
	
	//Todo : 데이터 통째로 가져오기
	if (UARGameInstance* GameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		UGameDataSubsystem* GameDataSubSystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
		if (GameDataSubSystem)
		{
			UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
			FString StageDifficult;
			FString StageName;
			FGameplayTag StageTag;
			if (bUseDebugStageInfoTag)
			{
				StageDifficult = TagManager.RequestGameplayTagDirectParent(DebugStageInfoTag).ToString();
				StageName = DebugStageInfoTag.ToString();
				StageTag = DebugStageInfoTag;

				int32 LastDot;
				if (StageDifficult.FindLastChar('.', LastDot))
				{
					// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
					StageDifficult = StageDifficult.RightChop(LastDot + 1);
				}

if (StageName.FindLastChar('.', LastDot))
{
	// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
	StageName = StageName.RightChop(LastDot + 1);
}

			}
			else
			{
				StageDifficult = TagManager.RequestGameplayTagDirectParent(GameInstance->CurrentStageTag).ToString();
				StageName = GameInstance->CurrentStageTag.ToString();
				StageTag = GameInstance->CurrentStageTag;

				int32 LastDot;
				if (StageDifficult.FindLastChar('.', LastDot))
				{
					// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
					StageDifficult = StageDifficult.RightChop(LastDot + 1);
				}

				if (StageName.FindLastChar('.', LastDot))
				{
					// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
					StageName = StageName.RightChop(LastDot + 1);
				}
			}
			FString ResultName = StageDifficult + StageName;

			TArray<FDTBattleStageInfo*> BattleStageInfos;
			if (StageInfoDataTable)
			{
				StageInfoDataTable->GetAllRows<FDTBattleStageInfo>(TEXT(""), BattleStageInfos);
				for (int i = 0; i < BattleStageInfos.Num(); i++)
				{
					if (StageTag.MatchesTag(BattleStageInfos[i]->StageTag) && StageTag.IsValid())
					{
						EnemyWaveData = BattleStageInfos[i]->BattleStageInfo.EnemyWaveDataInfo;
						break;
					}
				}
			}
		}
	}

	if (UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		if (UnitInfoDataTable)
		{
			TArray<FEnemyUnitsDataRow*> EnemyUnitsDataRows;
			UnitInfoDataTable->GetAllRows<FEnemyUnitsDataRow>(TEXT(""), EnemyUnitsDataRows);

			for (int i = 0; i < EnemyUnitsDataRows.Num(); i++)
			{
				for (int j = 0; j < EnemyWaveData.EnemyUnitsSet.Num(); j++)
				{
					FGameplayTag EnemyWaveTag = EnemyWaveData.EnemyUnitsSet[j].EnemyUnitTag;
					FGameplayTag DataTableRowEnemyTag = EnemyUnitsDataRows[i]->UnitData.Info.InfoSetting.Tag;
					if (EnemyWaveTag.MatchesTag(DataTableRowEnemyTag) && EnemyWaveTag.IsValid() && DataTableRowEnemyTag.IsValid())
					{
						UsingUnits.Add(EnemyUnitsDataRows[i]->UnitData.Info.InfoSetting.Tag, EnemyUnitsDataRows[i]->UnitData);
						break;
					}
				}
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(WaveTimer, this, &UEnemyUnitSpawnComponent::WaveStart, TickInterval, true);
}

#if WITH_EDITOR
void UEnemyUnitSpawnComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
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
	if (!EnemyUnitClass) return;

	for (int i = 0; i < EnemyWaveData.EnemyUnitsSet.Num(); i++)
	{
		FEnemyUnitStartSpawnTimeData EnemyUnitStartSpawnTimeData = EnemyWaveData.EnemyUnitsSet[i].EnemyUnitStartSpawnTimeData;
		if (!UnitTimes.Contains(EnemyWaveData.EnemyUnitsSet[i].EnemyUnitTag))
		{
			FUnitSpawnCalcData UnitSpawnCalcData;
			UnitTimes.Add(EnemyWaveData.EnemyUnitsSet[i].EnemyUnitTag, UnitSpawnCalcData);
		}
		if (!UnitTimes.Find(EnemyWaveData.EnemyUnitsSet[i].EnemyUnitTag)->UnitSpawnCalcData.Contains(EUnitSpawnCalc::StartSpawnTime))
		{
			
			UnitTimes.Find(EnemyWaveData.EnemyUnitsSet[i].EnemyUnitTag)->UnitSpawnCalcData.Add(EUnitSpawnCalc::StartSpawnTime, 0.0f);
		}

		float* Time = UnitTimes.Find(EnemyWaveData.EnemyUnitsSet[i].EnemyUnitTag)->UnitSpawnCalcData.Find(EUnitSpawnCalc::StartSpawnTime);
		if (EnemyUnitStartSpawnTimeData.bUseStartSpawnTime && EnemyUnitStartSpawnTimeData.StartSpawnTime <= *Time)
		{
			FGameplayTag WaveTag = EnemyWaveData.EnemyUnitsSet[i].EnemyUnitTag;
			UBattlefieldManagerSubsystem* BattlefieldManagerSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
			if(BattlefieldManagerSubsystem)
			{
				if (WaveTag.IsValid())
				{
					bool bIsDataSuccess = false;
					UE_LOG(LogTemp, Warning, TEXT("WaveTag : %s"), *WaveTag.ToString());
					FUnitData UnitData = BattlefieldManagerSubsystem->GetEnemyUnitData(WaveTag, bIsDataSuccess);
					if (!bIsDataSuccess) continue;

					FTransform Transform;
					FVector ResultSpawnLocation = GetOwner()->GetActorTransform().TransformPosition(SpawnLocation);
					Transform.SetLocation(ResultSpawnLocation);
					Transform.SetRotation(SpawnRotator.Quaternion());
					FVector Scale = FVector(1.0f, 1.0f, 1.0f);
					Transform.SetScale3D(Scale);

					// 스폰
					UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
					if (PoolingSubsystem)
					{
						if (EnemyUnitClass)
						{
							AActor* EnemyUnitInstance = PoolingSubsystem->SpawnFromPool(EnemyUnitClass, Transform);
							ABaseUnitCharacter* EnemyUnitCharacter = Cast<ABaseUnitCharacter>(EnemyUnitInstance);
							if (EnemyUnitCharacter)
							{
								EnemyUnitCharacter->SetUnit(UnitData);
								*Time = 0.0f;
								break;
							}
						}
						/*AActor* EnemyUnitInstance = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), EnemyUnitClass, Transform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
						ABaseUnitCharacter* EnemyUnitCharacter = Cast<ABaseUnitCharacter>(EnemyUnitInstance);
						if (EnemyUnitCharacter)
						{
							EnemyUnitCharacter->SetUnit(UnitData);

							EnemyUnitCharacter->FinishSpawning(Transform);
							*Time = 0.0f;
							break;
						}*/
					}
				}
			}
		}
		*Time += TickInterval;
	}
	InternalTime += TickInterval;
}


FGameplayTag UEnemyUnitSpawnComponent::CalculateEnemyUnitStartSpawnTime(const FEnemyUnitStartSpawnTimeData& InEnemyUnitStartSpawnTimeData, float InTime, float& OutPassedTime)
{
	if (!InEnemyUnitStartSpawnTimeData.bUseStartSpawnTime) return FGameplayTag::EmptyTag;

	return FGameplayTag();
}

