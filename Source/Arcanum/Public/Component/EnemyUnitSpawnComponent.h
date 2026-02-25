// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DataAssets/EnemyWaveData.h"
#include "EnemyUnitSpawnComponent.generated.h"

// 김도현
// 적 웨이브 생성 컴포넌트
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UEnemyUnitSpawnComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
public:
	// Sets default values for this component's properties
	UEnemyUnitSpawnComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion
	
protected:
	void WaveStart();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float EnemyWaveStartTime = 3.0f;

	// 적 웨이브 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FEnemyWaveDataInfo EnemyWaveData;
};
