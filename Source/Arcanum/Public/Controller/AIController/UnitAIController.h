// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/TeamInterface.h"
//#include "DetourCrowdAIController.h"
#include "UnitAIController.generated.h"

// 김도현
// 아군 유닛, 적군 유닛 AI컨트롤러
UCLASS()
class ARCANUM_API AUnitAIController : public AAIController
{
	GENERATED_BODY()
public:
	AUnitAIController();
};
