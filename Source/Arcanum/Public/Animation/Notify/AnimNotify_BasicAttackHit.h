#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BasicAttackHit.generated.h"

/**
 * 기본공격 스킬 호출
 */
UCLASS()
class ARCANUM_API UAnimNotify_BasicAttackHit : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
