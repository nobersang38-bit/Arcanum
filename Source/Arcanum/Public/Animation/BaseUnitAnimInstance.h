// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseUnitAnimInstance.generated.h"

// 김도현
// 유닛 베이스 애님인스턴스
UCLASS()
class ARCANUM_API UBaseUnitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBaseUnitAnimInstance();

	virtual void NativeInitializeAnimation() override;

protected:
	UFUNCTION()
	void TickUpdate();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpdateInterval = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Refrence")
	float Velocity = 0.0f;

private:
	float Internal_UpdateInterval = 0.0f;

	FTimerHandle UpdateTimerHandle;

	UPROPERTY()
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;
};
