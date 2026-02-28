// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Curves/CurveFloat.h"
#include "NativeGameplayTags.h"
#include "EnemyWaveData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyUnitStartSpawnTimeData
{
	GENERATED_BODY()
public:
	// - 나오기 시작하는 시간을 사용할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|StartSpawn", meta = (DisplayName = "나오기 시작하는 시간을 사용할 것인가"))
	bool bUseStartSpawnTime = true;
	// - 나오기 시작하는 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|StartSpawn", meta = (EditCondition = "bUseStartSpawnTime", EditConditionHides, DisplayName = "나오기 시작하는 시간"))
	float StartSpawnTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FEnemyUnitEndSpawnTimeData
{
	GENERATED_BODY()
public:
	// - 나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간을 사용할 것인가(false이면 계속나옴)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EndSpawn", meta = (DisplayName = "나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간을 사용할 것인가(false이면 계속나옴)"))
	bool bUseEndSpawnTime = false;
	// - 나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간(float)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EndSpawn", meta = (EditCondition = "bUseEndSpawnTime", EditConditionHides, DisplayName = "나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간"))
	float EndSpawnTime = 0.0f;
	// - 나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 커브(FRuntimeFloatCurve)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EndSpawn", meta = (EditCondition = "bUseEndSpawnTime", EditConditionHides, DisplayName = "나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 커브"))
	FRuntimeFloatCurve SpawnWeightCurve;
};

USTRUCT(BlueprintType)
struct FEnemyUnitEnemyBaseHealthTriggerData
{
	GENERATED_BODY()
public:
	// - 적의 건물 체력이 특정 체력 이하이면 나오게 하는 것을 사용할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EnemyBaseHealth", meta = (DisplayName = "적의 건물 체력이 특정 체력 이하이면 나오게 하는 것을 사용할 것인가"))
	bool bUseEnemyBaseHealthTrigger = false;
	// - 적의 건물 체력이 어느정도 이하이면 나올 것인가(일정이하이면 그때부터 스폰됨, 그리고 이것은 0에서1사이)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EnemyBaseHealth", meta = (EditCondition = "bUseEnemyBaseHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "적의 건물 체력이 어느정도 이하이면 나올 것인가"))
	float EnemyBaseHealthThreshold = 0.0f;
};

USTRUCT(BlueprintType)
struct FEnemyUnitAllyBaseHealthTriggerData
{
	GENERATED_BODY()
public:
	// - 플레이어의 건물 체력이 특정 체력 이하이면 나오게 하는것을 사용할 건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|AllyBaseHealth", meta = (DisplayName = "플레이어의 건물 체력이 특정 체력 이하이면 나오게 하는것을 사용할 건가"))
	bool bUseAllyBaseHealthTrigger = false;
	// - 플레이어의 건물 체력이 특정 체력 이하이면 나올 것인가(일정이하이면 그때부터 스폰됨, 그리고 이것은 0에서1사이)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|AllyBaseHealth", meta = (EditCondition = "bUseAllyBaseHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "플레이어의 건물 체력이 특정 체력 이하이면 나올 것인가"))
	float AllyBaseHealthThreshold = 0.0f;
	// - 플레이어의 건물 체력이 특정 체력 이하인데 이것을 지금 발동할까 말까 가중치(1이면 무조건 발동, 엘리트 몹이면 무조건 발동)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|AllyBaseHealth", meta = (EditCondition = "bUseAllyBaseHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "플레이어의 건물 체력이 특정 체력 이하인데 이것을 지금 발동할까 말까 가중치"))
	float AllyBaseHealthTriggerProbability = 1.0f;
};

USTRUCT(BlueprintType)
struct FEnemyUnitPlayerHealthTriggerData
{
	GENERATED_BODY()
public:
	// - 플레이어 캐릭터의 체력이 특정 체력 이하이면 나오게 하는것을 사용할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|PlayerHealth", meta = (DisplayName = "플레이어 캐릭터의 체력이 특정 체력 이하이면 나오게 하는것을 사용할 것인가"))
	bool bUsePlayerHealthTrigger = false;
	// - 플레이어 캐릭터의 체력이 어느정도 이하이면 나올 것인가(일정이하이면 그때부터 스폰됨, 그리고 이것은 0에서1사이)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|PlayerHealth", meta = (EditCondition = "bUsePlayerHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "플레이어 캐릭터의 체력이 어느정도 이하이면 나올 것인가"))
	float PlayerHealthThreshold = 0.0f;
	// - 플레이어 캐릭터의 체력이 어느정도 이하인데 이것을 지금 발동할까 말까 가중치(1이면 무조건 발동, 엘리트 몹이면 무조건 발동)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|PlayerHealth", meta = (EditCondition = "bUsePlayerHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "플레이어 캐릭터의 체력이 어느정도 이하인데 이것을 지금 발동할까 말까 가중치"))
	float PlayerHealthTriggerProbability = 1.0f;
};

USTRUCT(BlueprintType)
struct FEnemyUnitSpawnIntervalData
{
	GENERATED_BODY()
public:
	// - 나오는 주기는 어느정도 인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|SpawnInterval", meta = (DisplayName = "나오는 주기는 어느정도 인가"))
	float SpawnInterval = 2.0f;
	// - 나오는 주기의 랜덤 오차 범위는 어디까지 인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|SpawnInterval", meta = (DisplayName = "나오는 주기의 랜덤 오차 범위는 어디까지 인가"))
	float SpawnIntervalDeviation = 1.0f;
};

USTRUCT(BlueprintType)
struct FEnemyUnitDynamicIntervalData
{
	GENERATED_BODY()
public:
	// - 나오는 주기의 주기를 사용할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|DynamicInterval", meta = (DisplayName = "나오는 주기의 주기를 사용할 것인가"))
	bool bUseDynamicInterval = false;
	// - 나오는 주기의 주기는 어느정도 인가(이것은 0에서 1사이로 지정해야 할 듯)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|DynamicInterval", meta = (EditCondition = "bUseDynamicInterval", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "나오는 주기의 주기는 어느정도 인가"))
	float DynamicIntervalRatio = 0.5f;
	// - 나오는 주기의 주기의 랜덤 오차 범위는 어디까지 인가(이건 주기의 주기의 값을 백분율?함)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|DynamicInterval", meta = (EditCondition = "bUseDynamicInterval", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0", DisplayName = "나오는 주기의 주기의 랜덤 오차 범위는 어디까지 인가"))
	float DynamicIntervalDeviation = 0.25f;
};

// 김도현
// 해당하는 적의 스폰 관련 정보
USTRUCT(BlueprintType)
struct FEnemyUnitSpawnType
{
	GENERATED_BODY()

	// 플레이어 쪽 데이터를 고려해야하나
	// 예를 들어 플레이어의 현재 코스트(마나, 고기)
	// 플레이어 캐릭터의 체력
	// 플레이어의 건물 체력

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|Unit")
	FGameplayTag EnemyUnitTag;

	// - 나오기 시작하는 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|StartSpawn", meta = (DisplayName = "나오기 시작하는 시간"))
	FEnemyUnitStartSpawnTimeData EnemyUnitStartSpawnTimeData;

	// - 나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간(false이면 계속나옴)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EndSpawn", meta = (DisplayName = "나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간"))
	FEnemyUnitEndSpawnTimeData EnemyUnitEndSpawnTimeData;

	// - 적의 건물 체력이 특정 체력 이하이면 나오게 함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EnemyBaseHealth", meta = (DisplayName = "적의 건물 체력이 특정 체력 이하이면 나오게 함"))
	FEnemyUnitEnemyBaseHealthTriggerData EnemyUnitEnemyBaseHealthTriggerData;

	// - 플레이어의 건물 체력이 특정 체력 이하이면 나오게 함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|AllyBaseHealth", meta = (DisplayName = "플레이어의 건물 체력이 특정 체력 이하이면 나오게 함"))
	FEnemyUnitAllyBaseHealthTriggerData EnemyUnitAllyBaseHealthTriggerData;

	// - 플레이어 캐릭터의 체력이 특정 체력 이하이면 나오게 함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|PlayerHealth", meta = (DisplayName = "플레이어 캐릭터의 체력이 특정 체력 이하이면 나오게 함"))
	FEnemyUnitPlayerHealthTriggerData EnemyUnitPlayerHealthTriggerData;

	// - 나오는 주기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|SpawnInterval", meta = (DisplayName = "나오는 주기"))
	FEnemyUnitSpawnIntervalData EnemyUnitSpawnIntervalData;


	// - 나오는 주기의 주기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|DynamicInterval", meta = (DisplayName = "나오는 주기의 주기"))
	FEnemyUnitDynamicIntervalData EnemyUnitDynamicIntervalData;
};

USTRUCT(BlueprintType)
struct FEnemyWaveDataInfo
{
	GENERATED_BODY()
public:
	// 사용할 적 유닛들
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnitSet")
	TArray<FEnemyUnitSpawnType> EnemyUnitsSet;
};

// 김도현
// 스테이지 적 웨이브 기능
UCLASS()
class ARCANUM_API UEnemyWaveData : public UDataAsset
{
	GENERATED_BODY()
protected:
	// 사용할 적 유닛들
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnitSet")
	FEnemyWaveDataInfo EnemyUnitsSet;
};
