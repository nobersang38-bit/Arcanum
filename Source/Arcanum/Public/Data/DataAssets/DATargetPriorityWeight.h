// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DATargetPriorityWeight.generated.h"

/**
 * 
 */
UCLASS()
class ARCANUM_API UDATargetPriorityWeight : public UDataAsset
{
	GENERATED_BODY()
protected:
	// 플레이어 캐릭터 인가
	// 플레이어 캐릭터가 맞다면 스킬 사거리 닿는가
	// 플레이어 체력이 높으면 호전성 올라감

	// 이걸 사용할건가
	// 주변 적의 밀도가 높을 수로 호전성 올라감

	// 이걸 사용할건가
	// 거리가 가까울 수록 호전성이 올라감 
	
	// 이걸 사용할건가
	// 엘리트일수록 호전성이 올라감
};
