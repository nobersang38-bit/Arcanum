// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Curves/CurveFloat.h"
#include "EnemyWaveData.generated.h"

// 김도현
// 해당하는 적의 스폰 관련 정보
// Todo : 나중에 완성해야함
USTRUCT(BlueprintType, meta = (CollapseCategories))
struct FEnemyUnitSpawnType
{
	GENERATED_BODY()

	// 플레이어 쪽 데이터를 고려해야하나
	// 예를 들어 플레이어의 현재 코스트(마나, 고기)
	// 플레이어 캐릭터의 체력
	// 플레이어의 건물 체력

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|Unit")
	TSubclassOf<AActor> UnitClass = nullptr;

	// - 나오기 시작하는 시간을 사용할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|StartSpawn")
	bool bUseStartSpawnTime = true;
	// - 나오기 시작하는 시간이 있어야함
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|StartSpawn", meta = (EditCondition = "bUseStartSpawnTime", EditConditionHides))
	float StartSpawnTime = 0.0f;


	// - 나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간을 사용할 것인가(false이면 계속나옴)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EndSpawn")
	bool bUseEndSpawnTime = false;
	// - 나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간(float)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EndSpawn", meta = (EditCondition = "bUseEndSpawnTime", EditConditionHides))
	float EndSpawnTime = 0.0f;
	// - 나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 커브(FRuntimeFloatCurve)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EndSpawn", meta = (EditCondition = "bUseEndSpawnTime", EditConditionHides))
	FRuntimeFloatCurve SpawnWeightCurve;


	// - 적의 건물 체력이 특정 체력 이하이면 나오게 하는 것을 사용할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EnemyBaseHealth")
	bool bUseEnemyBaseHealthTrigger = false;
	// - 적의 건물 체력이 어느정도 이하이면 나올 것인가(일정이하이면 그때부터 스폰됨, 그리고 이것은 0에서1사이)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|EnemyBaseHealth", meta = (EditCondition = "bUseEnemyBaseHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
	float EnemyBaseHealthThreshold = 0.0f;


	// - 플레이어의 건물 체력이 특정 체력 이하이면 나오게 하는것을 사용할 건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|AllyBaseHealth")
	bool bUseAllyBaseHealthTrigger = false;
	// - 플레이어의 건물 체력이 특정 체력 이하이면 나올 것인가(일정이하이면 그때부터 스폰됨, 그리고 이것은 0에서1사이)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|AllyBaseHealth", meta = (EditCondition = "bUseAllyBaseHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
	float AllyBaseHealthThreshold = 0.0f;
	// - 플레이어의 건물 체력이 특정 체력 이하인데 이것을 지금 발동할까 말까 가중치(1이면 무조건 발동, 엘리트 몹이면 무조건 발동)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|AllyBaseHealth", meta = (EditCondition = "bUseAllyBaseHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
	float AllyBaseHealthTriggerProbability = 1.0f;

	// - 플레이어 캐릭터의 체력이 특정 체력 이하이면 나오게 하는것을 사용할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|PlayerHealth")
	bool bUsePlayerHealthTrigger = false;
	// - 플레이어 캐릭터의 체력이 어느정도 이하이면 나올 것인가(일정이하이면 그때부터 스폰됨, 그리고 이것은 0에서1사이)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|PlayerHealth", meta = (EditCondition = "bUsePlayerHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
	float PlayerHealthThreshold = 0.0f;
	// - 플레이어 캐릭터의 체력이 어느정도 이하인데 이것을 지금 발동할까 말까 가중치(1이면 무조건 발동, 엘리트 몹이면 무조건 발동)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|PlayerHealth", meta = (EditCondition = "bUsePlayerHealthTrigger", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
	float PlayerHealthTriggerProbability = 1.0f;


	// - 나오는 주기는 어느정도 인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|SpawnInterval")
	float SpawnInterval = 2.0f;
	// - 나오는 주기의 랜덤 오차 범위는 어디까지 인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|SpawnInterval")
	float SpawnIntervalDeviation = 1.0f;


	// - 나오는 주기의 주기를 사용할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|DynamicInterval")
	bool bUseDynamicInterval = false;
	// - 나오는 주기의 주기는 어느정도 인가(이것은 0에서 1사이로 지정해야 할 듯)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|DynamicInterval", meta = (EditCondition = "bUseDynamicInterval", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
	float DynamicIntervalRatio = 0.5f;
	// - 나오는 주기의 주기의 랜덤 오차 범위는 어디까지 인가(이건 주기의 주기의 값을 백분율?함)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|DynamicInterval", meta = (EditCondition = "bUseDynamicInterval", EditConditionHides, ClampMin = "0.0", ClampMax = "1.0"))
	float DynamicIntervalDeviation = 0.25f;


	// - 엘리트 몹인가(엘리트 몹이라면 현재 스테이지에 하나만 있어야하기 때문에 필요함)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSet|Elite")
	bool bIsEliteUnit = false;

	/*float SpawnStartTime = 0.0f
	FRuntimeFloatCurve */
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
	TArray<FEnemyUnitSpawnType> EnemyUnitsSet;
};
