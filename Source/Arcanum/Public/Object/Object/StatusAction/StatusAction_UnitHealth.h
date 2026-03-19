// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Object/Object/StatusAction/StatusAction.h"
#include "StatusAction_UnitHealth.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UStatusAction_UnitHealth : public UStatusAction
{
	GENERATED_BODY()
public:
	UStatusAction_UnitHealth();
	virtual void StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MaterialCurve")
	TObjectPtr<UCurveFloat> OutLineCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MaterialCurve")
	float OutLineTime = 0.3f;

private:
	float RevertHealth = 0.0f;
};
