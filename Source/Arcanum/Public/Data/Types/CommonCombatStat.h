// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "CommonCombatStat.generated.h"

// 김도현
// 전투할때 필요한 공용 스탯
// 플레이어 캐릭터, 아군 유닛, 저군 유닛
USTRUCT(BlueprintType)
struct FCommonCombatStat
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Arcanum.Unit.Faction"))
	FGameplayTag TeamID;

	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CommonStat")
	float Health = 100.0f;

	// 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CommonStat")
	float WalkSpeed = 450.0f;
};
