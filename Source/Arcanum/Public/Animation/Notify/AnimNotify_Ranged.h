// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Ranged.generated.h"

/**
 * 추영호
 * 노티파이 기본공격 원거리
 */
UCLASS()
class ARCANUM_API UAnimNotify_Ranged : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
