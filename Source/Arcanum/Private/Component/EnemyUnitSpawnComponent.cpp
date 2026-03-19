// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EnemyUnitSpawnComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"
#include "Data/Rows/DTBattleStageInfo.h"
#include "Data/Rows/UnitsDataRow.h"
#include "Character/BaseUnitCharacter.h"
#include "Core/SubSystem/PoolingSubsystem.h"
#include "NavigationSystem.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"

// Sets default values for this component's properties
UEnemyUnitSpawnComponent::UEnemyUnitSpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
			TArray<FUnitsDataRow*> EnemyUnitsDataRows;
			UnitInfoDataTable->GetAllRows<FUnitsDataRow>(TEXT(""), EnemyUnitsDataRows);

			for (int i = 0; i < EnemyUnitsDataRows.Num(); i++)
			{
				for (int j = 0; j < EnemyWaveData.EnemyUnitsSet.Num(); j++)
				{
					EnemyWaveData.EnemyUnitsSet[j].EnemyUnitSpawnIntervalData.Internal_SpawnInterval = 9999.0f;
					FGameplayTag EnemyWaveTag = EnemyWaveData.EnemyUnitsSet[j].EnemyUnitTag;
					FGameplayTag DataTableRowEnemyTag = EnemyUnitsDataRows[i]->UnitData.Tag;
					if (EnemyWaveTag.MatchesTag(DataTableRowEnemyTag) && EnemyWaveTag.IsValid() && DataTableRowEnemyTag.IsValid())
					{
						UsingUnits.Add(EnemyUnitsDataRows[i]->UnitData.Tag, EnemyUnitsDataRows[i]->UnitData);
						break;
					}
				}
			}
		}

		EnemyMultiers.Empty();
		for (int i = 0; i < BattleSubsystem->GetStageData().SpawnInfos.Num(); i++)
		{
			EnemyMultiers.Add(BattleSubsystem->GetStageData().SpawnInfos[i].EnemyTag, BattleSubsystem->GetStageData().SpawnInfos[i]);
		}
	}

	/*UnitSpawnTypes.Empty();
	for (int i = 0; i < EnemyWaveData.EnemyUnitsSet.Num(); i++)
	{
		FEnemyUnitSpawnType& SpawnType = EnemyWaveData.EnemyUnitsSet[i];
		UnitSpawnTypes.Add(SpawnType.EnemyUnitTag, SpawnType);
	}*/

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
	InternalTime += TickInterval;

	UnitsTimeUpdate(EnemyWaveData, TickInterval);
	FUnitInfoSetting UnitInfoSetting = AllCalculate(EnemyWaveData);

	// 스폰
	UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
	if (PoolingSubsystem)
	{
		// Todo KDH : 미리 로드해놓게 변경해야함
		if (UnitInfoSetting.Tag.IsValid())
		{
			FTransform Transform;
			FVector2D RandPoint = FMath::RandPointInCircle(SpawnRadius);
			FVector ResultSpawnLocation = GetOwner()->GetActorTransform().TransformPosition(SpawnLocation);

			UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
			FNavLocation NavLocation;

			NavSys->GetRandomReachablePointInRadius(ResultSpawnLocation, SpawnRadius, NavLocation);
			Transform.SetLocation(NavLocation);
			Transform.SetRotation(SpawnRotator.Quaternion());
			FVector Scale = FVector(1.0f, 1.0f, 1.0f);
			Transform.SetScale3D(Scale);

			AActor* EnemyUnitInstance = PoolingSubsystem->SpawnFromPool(UnitInfoSetting.UnitClass.LoadSynchronous(), Transform);
			ABaseUnitCharacter* EnemyUnitCharacter = Cast<ABaseUnitCharacter>(EnemyUnitInstance);
			if (EnemyUnitCharacter)
			{
				EnemyUnitCharacter->SetUnit(UnitInfoSetting);

				if (EnemyMultiers.Contains(UnitInfoSetting.Tag))
				{
					FEnemySpawnInfo* EnemySpawnInfo = EnemyMultiers.Find(UnitInfoSetting.Tag);
					
					TArray<FRegenStat> RegenStats;
					TArray<FNonRegenStat> NonRegenStats;
					for (int i = 0; i < MultipleStatTags.Num(); i++)
					{
						if (MultipleStatTags[i].MatchesTag(RegenTag))
						{
							const FRegenStat* RegenStat = EnemyUnitCharacter->GetCharacterBattleStatsComponent()->FindRegenStat(MultipleStatTags[i]);
							if (RegenStat)
							{
								FRegenStat Regen = *RegenStat;
								Regen.BaseMax *= (EnemySpawnInfo->SpawnMultiplier - 1.0f);
								Regen.Current *= Regen.BaseMax;
								RegenStats.Add(Regen);
							}
						}
						else if (MultipleStatTags[i].MatchesTag(NonRegenTag))
						{
							const FNonRegenStat* NonRegenStat = EnemyUnitCharacter->GetCharacterBattleStatsComponent()->FindNonRegenStat(MultipleStatTags[i]);
							if (NonRegenStat)
							{
								FNonRegenStat NonRegen = *NonRegenStat;
								NonRegen.BaseValue *= (EnemySpawnInfo->SpawnMultiplier - 1.0f);
								NonRegenStats.Add(NonRegen);
							}
						}
					}
					EnemyUnitCharacter->GetCharacterBattleStatsComponent()->AddRegenBonus(RegenStats);
					EnemyUnitCharacter->GetCharacterBattleStatsComponent()->AddNonRegenBonus(NonRegenStats);

					for (const auto& RegenStat : RegenStats)
					{
						EnemyUnitCharacter->GetCharacterBattleStatsComponent()->ChangeStatValue(RegenStat.ParentTag, RegenStat.BaseMax, nullptr);
					}

					/*for (const auto& NonRegenStat : NonRegenStats)
					{
						EnemyUnitCharacter->GetCharacterBattleStatsComponent()->ChangeStatValue(NonRegenStat.TagName, NonRegenStat.);
					}*/

				}
			}
		}
	}
}

FUnitInfoSetting UEnemyUnitSpawnComponent::AllCalculate(FEnemyWaveDataInfo& InEnemyWaveData)
{
	FGameplayTag ResultTag;
	ResultTag = SpawnTimeCalculate(InEnemyWaveData);

	if (ResultTag.IsValid())
	{
		if (FUnitInfoSetting* UnitInfo = UsingUnits.Find(ResultTag))
		{
			for (int i = 0; i < EnemyWaveData.EnemyUnitsSet.Num(); i++)
			{
				if (EnemyWaveData.EnemyUnitsSet[i].EnemyUnitTag == UnitInfo->Tag)
				{
					FEnemyUnitSpawnType& SpawnType = EnemyWaveData.EnemyUnitsSet[i];
					if (SpawnType.EnemyUnitTag.IsValid())
					{
						UseUnitData(SpawnType);
						return *UnitInfo;
					}
					break;
				}
			}
		}
	}
	return FUnitInfoSetting();
}

bool UEnemyUnitSpawnComponent::IsEnemyUnitStartSpawnTimeOver(const FEnemyUnitStartSpawnTimeData& InStartSpawnTimeData)
{
	if (!InStartSpawnTimeData.bUseStartSpawnTime) return true;

	if (InStartSpawnTimeData.StartSpawnTime <= InternalTime)
	{
		return true;
	}
	return false;
}

bool UEnemyUnitSpawnComponent::IsEnemyUnitEndSpawnTimeNotOver(const FEnemyUnitEndSpawnTimeData& InEndSpawnTimeData)
{
	if (!InEndSpawnTimeData.bUseEndSpawnTime) return true;

	if (InEndSpawnTimeData.EndSpawnTime > InternalTime)
	{
		return true;
	}
	return false;
}

bool UEnemyUnitSpawnComponent::IsEnableEnemyUnitSpawnTime(const FEnemyUnitStartSpawnTimeData& InStartSpawnTimeData, const FEnemyUnitEndSpawnTimeData& InEndSpawnTimeData)
{
	return IsEnemyUnitStartSpawnTimeOver(InStartSpawnTimeData) && IsEnemyUnitEndSpawnTimeNotOver(InEndSpawnTimeData);
}

void UEnemyUnitSpawnComponent::UseUnitData(FEnemyUnitSpawnType& InEnemyWaveData)
{
	InEnemyWaveData.EnemyUnitSpawnIntervalData.Internal_SpawnInterval = 0.0f;
	if (InEnemyWaveData.EnemyUnitDynamicIntervalData.bUseDynamicInterval)
	{
		InEnemyWaveData.EnemyUnitDynamicIntervalData.Internal_DynamicIntervalTime = 0.0f;
	}
}

void UEnemyUnitSpawnComponent::UnitsTimeUpdate(FEnemyWaveDataInfo& InEnemyWaveData, float DeltaTime)
{
	for (int i = 0; i < InEnemyWaveData.EnemyUnitsSet.Num(); i++)
	{
		FEnemyUnitSpawnType& UnitSpawnType = InEnemyWaveData.EnemyUnitsSet[i];

		UnitSpawnType.EnemyUnitSpawnIntervalData.Internal_SpawnInterval += DeltaTime;

		if (UnitSpawnType.EnemyUnitDynamicIntervalData.bUseDynamicInterval)
		{
			UnitSpawnType.EnemyUnitDynamicIntervalData.Internal_DynamicIntervalTime += DeltaTime;
		}
	}
}

FGameplayTag UEnemyUnitSpawnComponent::SpawnTimeCalculate(FEnemyWaveDataInfo& InEnemyWaveData)
{
	TArray<FGameplayTag> Tags;
	for (int i = 0; i < InEnemyWaveData.EnemyUnitsSet.Num(); i++)
	{
		FEnemyUnitSpawnType& SpawnType = InEnemyWaveData.EnemyUnitsSet[i];
		if (IsEnableEnemyUnitSpawnTime(SpawnType.EnemyUnitStartSpawnTimeData, SpawnType.EnemyUnitEndSpawnTimeData))
		{
			if (SpawnType.UseOnceSpawn)
			{
				if (!SpawnType.Internal_UsedOnce)
				{
					SpawnType.Internal_UsedOnce = true;
					Tags.Add(SpawnType.EnemyUnitTag);
				}
				continue;
			}
			// 나오는 주기 체크
			float SpawnTime = (SpawnType.EnemyUnitSpawnIntervalData.SpawnInterval - (SpawnType.EnemyUnitSpawnIntervalData.SpawnIntervalDeviation * 0.5f));

			SpawnTime = FMath::FRandRange(0.0f, SpawnType.EnemyUnitSpawnIntervalData.SpawnIntervalDeviation) + SpawnTime;
			if (SpawnTime < SpawnType.EnemyUnitSpawnIntervalData.Internal_SpawnInterval)
			{
				Tags.Add(SpawnType.EnemyUnitTag);
			}
		}
	}
	if (!Tags.IsEmpty())
	{
		int32 SelectIDX = FMath::RandRange(0, Tags.Num() - 1);
		return Tags[SelectIDX];
	}
	return FGameplayTag::EmptyTag;
}

