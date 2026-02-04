// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatStageData.generated.h"

// 김도현
// 스테이지 데이터
USTRUCT(BlueprintType)
struct FCombatStageData
{
	GENERATED_BODY()
protected:

	// Todo : 이 부분에 몬스터 웨이브 구조체나 데이터 에셋 같은거 들어가야 합니다

	// 스테이지명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Info")
	FText StageName = FText::FromString(TEXT("스테이지 명"));

	// 스테이지 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Health")
	FText StageDescription = FText::FromString(TEXT("이 스테이지는 엄청난 보스가 기다리고 있다 방심하지 말아야한다"));

	// 아군 건물 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Health")
	float AllyNexusHealth = 100.0f;

	// 적 건물 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Health")
	float EnemyNexusHealth = 100.0f;

	// Todo 부모 데이터 에셋 만들어서 변경해야 합니다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Wave")
	TSoftObjectPtr<UDataAsset> WaveData = nullptr;
};
