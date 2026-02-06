// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DATargetPriorityWeight.generated.h"

/**
 * 
 */
UCLASS()
class ARCANUM_API UDATargetPriorityWeight : public UDataAsset
{
	GENERATED_BODY()
public:
	FORCEINLINE float GetDetectDistance() const { return DetectDistance; };
	FORCEINLINE bool GetUseAttackingTargetNumWeight() const { return bUseAttackingTargetNumWeight; };
	FORCEINLINE float GetAttackingTargetNumWeight() const { return AttackingTargetNumWeight; };

protected:
	// 플레이어 캐릭터 인가
	// 플레이어 캐릭터가 맞다면 스킬 사거리 닿는가
	// 플레이어 체력이 높으면 호전성 올라감

	//감지 거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectDistance = 1000.0f;

	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseAttackingTargetNumWeight = true;
	// 공격하고 있는 대상이 적을수록 점수 올라감
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackingTargetNumWeight = 0.5f;

	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseEnemyDensityWeight = true;
	// 주변 적의 밀도가 낮을 수록 호전성 올라감
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EnemyDensityWeight = 0.5f;

	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseDistanceWeight = true;
	// 거리가 가까울 수록 호전성이 올라감 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DistanceWeight = 0.5f;
	
	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseEliteWeight = true;
	// 엘리트일수록 호전성이 올라감
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EliteWeight = 0.5f;
};
