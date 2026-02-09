#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WeaponSkillMapRow.generated.h"

/**
 * 추영호
 * 무기 태그 → 스킬ID 매핑용 Row.
 * 장비(무기) 교체 시 현재 무기 태그로 이 Row를 조회해서
 * 일반 무기 스킬(CommonSkillID)과 전설/궁극기 스킬(LegendarySkillID)을 결정한다.
 * 실제 스킬 수치/효과는 FSkillRow가 담당하고, 이 Row는 '연결(매핑)'만 담당한다.
 */
USTRUCT(BlueprintType)
struct FWeaponSkillMapRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 무기 태그 (Arcanum.Item.Equipment.Weapon.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSkillMap")
	FGameplayTag WeaponTag;

	/* 일반 무기 스킬 ID (Arcanum.Ability.Skill.Weapon.Common.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSkillMap")
	FGameplayTag CommonSkillID;

	/* 전설 무기 스킬 ID (Arcanum.Ability.Skill.Weapon.Legendary.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSkillMap")
	FGameplayTag LegendarySkillID;
};

