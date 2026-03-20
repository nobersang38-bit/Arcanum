#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_MeleeAttack.generated.h"

/**
 * 추영호
 * 기본 공격 노티파이
 */

UCLASS()
class ARCANUM_API UAnimNotify_MeleeAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
