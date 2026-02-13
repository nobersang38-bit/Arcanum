// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/Types/BaseUnitData.h"
#include "RuntimeUnitDataInterface.generated.h"

// 김도현
// 주변의 적의 정보
UINTERFACE(MinimalAPI)
class URuntimeUnitDataInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANUM_API IRuntimeUnitDataInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FUnitRuntimeData& GetUnitRuntimeData() = 0;
};
