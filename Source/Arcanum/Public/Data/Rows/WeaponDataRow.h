#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WeaponDataRow.generated.h"

/**
 * 추영호
 * - WeaponTag 하나로 무기의 외형, 소켓, 스킬을 모두 조회하기 위한 데이터
 * - 인게임에서 무기 교체/스왑/전설 발동 시 공통으로 사용되는 기준 테이블
 */
USTRUCT(BlueprintType)
struct FWeaponDataRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 무기 식별 태그 (Arcanum.Item.Equipment.Weapon.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FGameplayTag WeaponTag;

	/* 인게임에서 손 소켓에 부착될 무기 메시 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	/* 무기 부착 소켓 이름 (Hand_R) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName SocketName = FName("Hand_R");

	/* 일반 무기 스킬 ID (Arcanum.Ability.Skill.Weapon.Common.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FGameplayTag CommonSkillID;

	/* 전설 무기 스킬 ID (Arcanum.Ability.Skill.Weapon.Legendary.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FGameplayTag LegendarySkillID;
};

