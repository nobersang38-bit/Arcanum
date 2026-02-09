// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Data/Types/CombatStageData.h"
#include "BattlefieldManagerSubsystem.generated.h"

// 김도현
// 전투 스테이지에 관련된 기능
// Todo : 나중에 스폰관련도 넣어야함
class ACharacter;
UCLASS()
class ARCANUM_API UBattlefieldManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
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

protected:
	FCombatStageData StageData;

	TWeakObjectPtr<ACharacter> AllyNexus = nullptr;
	TWeakObjectPtr<ACharacter> EnemyNexus = nullptr;
};
