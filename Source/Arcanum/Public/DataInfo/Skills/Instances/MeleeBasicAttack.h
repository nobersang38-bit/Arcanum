#pragma once

#include "CoreMinimal.h"
#include "DataInfo/Skills/Instances/USkillBase.h"
#include "MeleeBasicAttack.generated.h"

/*
 * 추영호
 * 스킬베이스 - 근거리 기본 공격
 */
UCLASS()
class ARCANUM_API UMeleeBasicAttack : public USkillBase
{
	GENERATED_BODY()

public:
	virtual void ActivateSkill(AActor* Instigator) override;

private:
	/* 사거리 내 가장 가까운 대상 1명 찾기 */
	AActor* FindNearestTarget(AActor* InInstigator, float InRange) const;

	/* 기본공격 데미지 적용 */
	bool ApplyBasicAttackDamage(AActor* InInstigator, AActor* InTarget, const FLevelModifierEntry& InEntry);

	/* 적중 알림 -> SkillComponent 게이지 처리 */
	void NotifyBasicAttackHitToSkillComponent(AActor* InInstigator, AActor* InTarget);
};
