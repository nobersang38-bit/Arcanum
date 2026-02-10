// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DATargetPriorityWeight.generated.h"

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
	FORCEINLINE bool GetUseRandomTargetWeight() { return bUseRandomTargetWeight; };
	FORCEINLINE float GetRandomTargetWeight() const { return RandomTargetWeight; };

	// 해당 적을 공격하고 있는 아군이 적을수록 점수 올라감
	FORCEINLINE bool GetUseAttackingTargetNumWeight() { return bUseAttackingTargetNumWeight; };
	FORCEINLINE float GetAttackingTargetNumWeight() const { return AttackingTargetNumWeight; };

	// 거리가 가까울 수록 점수가 높음
	FORCEINLINE bool GetUseDistanceWeight() { return bUseDistanceWeight; };
	FORCEINLINE float GetDistanceWeight() const { return DistanceWeight; };

	// 나를 공격하는 대상에게 점수를 줌
	FORCEINLINE bool GetUseAggroWeight() { return bUseAggroWeight; };
	FORCEINLINE float GetAggroWeight() const { return AggroWeight; };

	// 엘리트일수록 점수가 높음
	FORCEINLINE bool GetUseEliteWeight() { return bUseEliteWeight; };
	FORCEINLINE float GetEliteWeight() const { return EliteWeight; };

protected:
	// 플레이어 캐릭터 인가
	// 플레이어 캐릭터가 맞다면 스킬 사거리 닿는가
	// 플레이어 체력이 높으면 호전성 올라감

	//감지 거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DetectDistance = 1000.0f;

	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseRandomTargetWeight = true;
	// 랜덤하게 지정할 점수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RandomTargetWeight = 0.5f;

	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseAttackingTargetNumWeight = true;
	// 해당 적을 공격하고 있는 아군이 적을수록 점수 올라감
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackingTargetNumWeight = 0.5f;

	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseDistanceWeight = true;
	// 거리가 가까울 수록 점수가 높음
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DistanceWeight = 0.5f;

	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseAggroWeight = true;
	// 나를 공격하는 대상에게 점수를 줌
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AggroWeight = 0.5f;

	
	// 이걸 사용할건가
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseEliteWeight = true;
	// 엘리트일수록 점수가 높음
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EliteWeight = 0.5f;
};
