// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Curves/CurveFloat.h"
#include "EnemyWaveData.generated.h"

// 김도현
// 해당하는 적의 스폰 관련 정보
// Todo : 나중에 완성해야함
USTRUCT(BlueprintType)
struct FEnemyUnitSpawnType
{
	GENERATED_BODY()

	/*
		- 나오기 시작하는 시간을 사용할 것인가
		- 나오기 시작하는 시간이 있어야함

		- 적의 건물 체력이 특정 체력 이하이면 나오게 하는 것을 사용할 것인가
		- 적의 건물 체력이 어느정도 이하이면 나올 것인가(일정이하이면 그때부터 스폰됨, 그리고 이것은 백분율로 합시다 0에서1사이)

		- 나오는 주기는 어느정도 인가
		- 나오는 주기의 랜덤 오차 범위는 어디까지 인가

		- 엘리트 몹인가(엘리트 몹이라면 현재 스테이지에 하나만 있어야하기 때문에 필요함)
	*/

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSet<TSubclassOf<AActor>> UsingEnemyUnits;
	
	
};
