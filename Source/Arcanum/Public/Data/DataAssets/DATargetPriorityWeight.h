// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/Types/BaseUnitData.h"
#include "Data/Types/UnitData.h"
#include "DATargetPriorityWeight.generated.h"

// 김도현
// AI 우선순위 하위 구조체
USTRUCT(BlueprintType)
struct FTargetPriorityWeightSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseWeight = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float TargetWeight = 0.5f;
};

// 김도현
// AI 우선순위 데이터
UCLASS()
class ARCANUM_API UDATargetPriorityWeight : public UDataAsset
{
	GENERATED_BODY()
public:
	//감지 거리
	FORCEINLINE float GetDetectDistance() const { return DetectDistance; };

	// 랜덤하게 지정할 점수
	FORCEINLINE const FTargetPriorityWeightSetting& GetRandomTargetWeightData() const { return RandomTargetWeightSet; };

	// 해당 적을 공격하고 있는 아군이 적을수록 점수 올라감
	FORCEINLINE const FTargetPriorityWeightSetting& GetAttackingTargetNumWeightData() const { return AttackingTargetNumWeightSet; };

	// 나와 적의 거리가 가까울 수록 점수가 높음
	FORCEINLINE const FTargetPriorityWeightSetting& GetDistanceWeightData() const { return DistanceWeightSet; };

	// 나를 공격하는 대상에게 점수를 줌
	FORCEINLINE const FTargetPriorityWeightSetting& GetAggroWeightData() const { return AggroWeightSet; };

	// 해당 적의 체력이 적으면 점수가 높음(백분율)
	FORCEINLINE const FTargetPriorityWeightSetting& GetLowHealthWeightData() const { return LowHealthWeightSet; };

	// 엘리트일수록 점수가 높음
	FORCEINLINE const FTargetPriorityWeightSetting& GetEliteWeightData() const { return EliteWeightSet; };


	//점수 계산 함수
	float CalculateScore(ACharacter* MyCharacter, ACharacter* TargetCharacter) const;

protected:
	// 플레이어 캐릭터 인가
	// 플레이어 캐릭터가 맞다면 스킬 사거리 닿는가
	// 플레이어 체력이 높으면 호전성 올라감

	//감지 거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectDistance = 1000.0f;

	// 랜덤하게 지정할 점수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTargetPriorityWeightSetting RandomTargetWeightSet;

	// 해당 적을 공격하고 있는 아군이 적을수록 점수 올라감
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTargetPriorityWeightSetting AttackingTargetNumWeightSet;

	// 나와 적의 거리가 가까울 수록 점수가 높음
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTargetPriorityWeightSetting DistanceWeightSet;

	// 나를 공격하는 대상에게 점수를 줌
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTargetPriorityWeightSetting AggroWeightSet;

	// 해당 적의 체력이 적으면 점수가 높음(백분율)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTargetPriorityWeightSetting LowHealthWeightSet;
	
	// 엘리트일수록 점수가 높음
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTargetPriorityWeightSetting EliteWeightSet;

private:
	// 랜덤하게 지정할 점수
	float CalculateScoreRandomTargetWeight() const;

	// 해당 적을 공격하고 있는 아군이 적을수록 점수 올라감
	float CalculateScoreAttackingTargetNumWeight(const FUnitRuntimeData& TargetUnitRuntimeData) const;

	// 나와 적의 거리가 가까울 수록 점수가 높음
	float CalculateScoreDistanceWeight(ACharacter* MyCharacter, ACharacter* TargetCharacter) const;

	// 나를 공격하는 대상에게 점수를 줌
	float CalculateScoreAggroWeight(const FUnitRuntimeData& MyUnitRuntimeData, ACharacter* TargetCharacter) const;

	// 해당 적의 체력이 적으면 점수가 높음(백분율)
	float CalculateScoreLowHealthWeight() const;

	// 엘리트일수록 점수가 높음
	float CalculateScoreEliteWeight(const FUnitData& TargetUnitData) const;
};
