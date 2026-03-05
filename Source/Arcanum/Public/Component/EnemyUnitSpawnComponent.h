// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DataAssets/EnemyWaveData.h"
#include "GameplayTags/ArcanumTags.h"
#include "Data/Types/UnitData.h"
#include "EnemyUnitSpawnComponent.generated.h"

UENUM()
enum class EUnitSpawnCalc : uint8
{
	StartSpawnTime,
};

USTRUCT()
struct FUnitSpawnCalcData
{
	GENERATED_BODY()
public:
	TMap<EUnitSpawnCalc, float> UnitSpawnCalcData;
};

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
	UFUNCTION()
	void WaveStart();

	FGameplayTag CalculateEnemyUnitStartSpawnTime(const FEnemyUnitStartSpawnTimeData& InEnemyUnitStartSpawnTimeData, float InTime, float& OutPassedTime);

#pragma region 디버그
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseDebugStageInfoTag = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag DebugStageInfoTag = Arcanum::BattleStage::Normal::Stage1;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<UDataTable> UnitInfoDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<UDataTable> StageInfoDataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTag StageInfoTag = Arcanum::DataTable::StageInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTag EnemyUnitInfoTag = Arcanum::DataTable::EnemyUnitInfo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TSubclassOf<class ABaseUnitCharacter> EnemyUnitClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (MakeEditWidget = true))
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (MakeEditWidget = true))
	FRotator SpawnRotator = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float TickInterval = 0.1f;
	// 적 웨이브 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FEnemyWaveDataInfo EnemyWaveData;

private:
	FTimerHandle WaveTimer;

	float InternalTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FUnitData> UsingUnits;

	// 유닛 태그, 유닛 스폰 계산 타입, 지난시간
	UPROPERTY()
	TMap<FGameplayTag, FUnitSpawnCalcData> UnitTimes;
};
