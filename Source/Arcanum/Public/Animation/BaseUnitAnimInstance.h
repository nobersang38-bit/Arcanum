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

public:
	FORCEINLINE void SetAnimations(UAnimSequenceBase* InAnimIdle, UAnimSequenceBase* InAnimRun)
	{
		Anim_Idle = InAnimIdle;
		Anim_Run = InAnimRun;
	}

protected:
	UFUNCTION()
	void TickUpdate();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpdateInterval = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Refrence")
	float Velocity = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Refrence")
	float VelocityRatio = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	float VelocityRatioFactor = 0.01f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimSequence")
	TObjectPtr<UAnimSequenceBase> Anim_Idle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimSequence")
	TObjectPtr<UAnimSequenceBase> Anim_Run = nullptr;

private:
	float Internal_UpdateInterval = 0.0f;

	FTimerHandle UpdateTimerHandle;

	UPROPERTY()
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;
};
