// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/DataAssets/EnemyWaveData.h"
#include "GameplayTags/ArcanumTags.h"
#include "Data/Types/UnitData.h"
#include "DataInfo/StageData/EnemyData/Data/FEnemyData.h"
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

protected:
	FUnitInfoSetting AllCalculate(FEnemyWaveDataInfo& InEnemyWaveData);

	// - 나오기 시작하는 시간 확인
	bool IsEnemyUnitStartSpawnTimeOver(const FEnemyUnitStartSpawnTimeData& InStartSpawnTimeData);

	// - 나오기 시작하고 나서 해당하는 적이 다시는 안나올때까지의 시간 확인
	bool IsEnemyUnitEndSpawnTimeNotOver(const FEnemyUnitEndSpawnTimeData& InEndSpawnTimeData);

	// - 나올 수 있는 시간인지 확인
	bool IsEnableEnemyUnitSpawnTime(const FEnemyUnitStartSpawnTimeData& InStartSpawnTimeData, const FEnemyUnitEndSpawnTimeData& InEndSpawnTimeData);

	// 사용한 유닛 주기 초기화
	void UseUnitData(FEnemyUnitSpawnType& InEnemyWaveData);

	// 유닛 타임 업데이트
	void UnitsTimeUpdate(FEnemyWaveDataInfo& InEnemyWaveData, float DeltaTime);

	// - 스폰 시간 범위내에 있는 유닛 태그 중 하나 골라서 리턴
	FGameplayTag SpawnTimeCalculate(FEnemyWaveDataInfo& InEnemyWaveData);




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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (MakeEditWidget = true))
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (MakeEditWidget = true))
	float SpawnRadius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (MakeEditWidget = true))
	FRotator SpawnRotator = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float TickInterval = 0.1f;
	// 적 웨이브 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	FEnemyWaveDataInfo EnemyWaveData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Stat")
	TArray<FGameplayTag> MultipleStatTags = { Arcanum::BattleStat::Character::Regen::Health::Root, Arcanum::BattleStat::Character::NonRegen::AttackPower::Root };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Stat")
	FGameplayTag RegenTag = Arcanum::BattleStat::Character::Regen::Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|Stat")
	FGameplayTag NonRegenTag = Arcanum::BattleStat::Character::NonRegen::Root;

private:
	FTimerHandle WaveTimer;

	float InternalTime = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FUnitInfoSetting> UsingUnits;

	// 유닛 태그, 유닛 스폰 계산 타입, 지난시간
	UPROPERTY()
	TMap<FGameplayTag, FUnitSpawnCalcData> UnitTimes;

	UPROPERTY()
	TMap<FGameplayTag, FEnemySpawnInfo> EnemyMultiers;
};
