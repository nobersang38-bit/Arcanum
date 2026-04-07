#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Object/Skills/SkillActor.h"
#include "FBattleWeaponSkillData.generated.h"

/**
 * 추영호 
 * 스킬 캐시 데이터
 */

USTRUCT(BlueprintType)
struct FBattleSkillData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SkillTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SkillLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CastTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Cooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ManaCost = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> SkillIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> CastMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> PressMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ReleaseMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> ComboMontages;

	/* 스킬 실행 클래스 캐시 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASkillActor> SkillClass;
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

USTRUCT(BlueprintType)
struct FBattlePotionRuntimeSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag PotionTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCooldown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentCooldown = 0.0f;
};