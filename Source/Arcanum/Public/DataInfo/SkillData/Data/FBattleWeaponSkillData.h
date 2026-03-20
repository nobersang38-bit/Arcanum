#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "FBattleWeaponSkillData.generated.h"

/**
 * 추영호 
 * 전투용 무기 스킬 캐시 데이터
 */

USTRUCT(BlueprintType)
struct FBattleSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SkillTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SkillLevel = 0;

	/* 궁극기 캐스트 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CastTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillIcon = nullptr;
};

USTRUCT(BlueprintType)
struct FBattleWeaponSkillData
{
	GENERATED_BODY()

	/* 일반 무기 슬롯 1 기본 공격 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBattleSkillData WeaponSlot1BasicAttackSkill;

	/* 일반 무기 슬롯 1 기본 스킬 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBattleSkillData WeaponSlot1BasicSkill;

	/* 일반 무기 슬롯 2 기본 공격 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBattleSkillData WeaponSlot2BasicAttackSkill;

	/* 일반 무기 슬롯 2 기본 스킬 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBattleSkillData WeaponSlot2BasicSkill;

	/* 전설 무기 궁극기 스킬 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBattleSkillData LegendaryUltimateSkill;

	/* 현재 활성 일반 무기 슬롯 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag CurrentWeaponSlotTag;
};