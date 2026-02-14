// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/Types/CombatStageData.h"
#include "Data/Types/MatchData.h"
#include "BattlefieldManagerSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchEnded, const FMatchData&)

// 김도현
// 전투 스테이지에 관련된 기능
// Todo : 나중에 스폰관련도 넣어야함
class ACharacter;
class ABattlefieldManagerActor;
UCLASS()
class ARCANUM_API UBattlefieldManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

public:
	// 전투 스테이지가 종료되면 호출, 결과 정보 = FMatchResultData
	FOnMatchEnded OnMatchEnded;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetStageData(const FCombatStageData& InData) { StageData = InData; }

	UFUNCTION(BlueprintCallable)
	ACharacter* GetAllyNexus() const;

	UFUNCTION(BlueprintCallable)
	ACharacter* GetEnemyNexus() const;

	UFUNCTION(BlueprintCallable)
	void SetAllyNexus(ACharacter* InNexus);

	UFUNCTION(BlueprintCallable)
	void SetEnemyNexus(ACharacter* InNexus);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ABattlefieldManagerActor* GetBattlefieldManagerActor() { return BattlefieldManagerActor.Get(); }

	UFUNCTION()
	void SetABattlefieldManagerActor(ABattlefieldManagerActor* InBattlefieldManagerActor);

	// 현재 스테이지 플레이 정보(진행중인지, 종료했는지 종료됐다면 승리했는지)
	FORCEINLINE const FMatchData& GetCurrentMatchData() { return CurrentMatchData; }

protected:
	UFUNCTION()
	void SetCurrentMatchData(const FMatchData& InData);

protected:
	FCombatStageData StageData;

	UPROPERTY()
	TWeakObjectPtr<ACharacter> AllyNexus = nullptr;
	UPROPERTY()
	TWeakObjectPtr<ACharacter> EnemyNexus = nullptr;

	FMatchData CurrentMatchData;

	UPROPERTY()
	TWeakObjectPtr<ABattlefieldManagerActor> BattlefieldManagerActor = nullptr;
};
