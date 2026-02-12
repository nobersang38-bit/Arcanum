// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MatchData.generated.h"

UENUM(BlueprintType)
enum class EMatchState : uint8
{
	Playing		UMETA(DisplayName = "Playing"),
	Ended		UMETA(DisplayName = "Ended"),
};

// 김도현
// 게임이 종료되었을때 결과 값들
USTRUCT(BlueprintType)
struct FMatchData
{
	GENERATED_BODY()
public:
	// 현재 게임이 진행중인가, 아니면 종료된 상태인가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMatchState CurrentMatchState = EMatchState::Playing;

	// 해당 라운드를 플레이어가 승리하였는가
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsVictory = false;
};
