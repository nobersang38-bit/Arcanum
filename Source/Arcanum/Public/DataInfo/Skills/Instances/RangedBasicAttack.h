#pragma once

#include "CoreMinimal.h"
#include "DataInfo/Skills/Instances/USkillBase.h"
#include "RangedBasicAttack.generated.h"

/**
 * 추영호
 * 스킬베이스 - 원거리 기본 공격
 */
UCLASS()
class ARCANUM_API URangedBasicAttack : public USkillBase
{
	GENERATED_BODY()
	
public:
	/* 원거리 기본공격 실행 (최종: 프로젝타일 발사) */
	virtual void ActivateSkill(AActor* Instigator) override;

private:
	/* 사거리 내 가장 가까운 대상 1명 찾기 (현재 테스트용 타겟 선정) */
	AActor* FindNearestTarget(AActor* InInstigator, float InRange) const;

	/* 원거리 기본공격 데미지 적용 (현재 테스트용 즉시 적용) */
	bool ApplyRangedBasicAttackDamage(AActor* InInstigator, AActor* InTarget, const FLevelModifierEntry& InEntry);

	/* 기본공격 적중 알림 -> SkillComponent에서 게이지 증가 처리 */
	void NotifyBasicAttackHitToSkillComponent(AActor* InInstigator, AActor* InTarget);
};
