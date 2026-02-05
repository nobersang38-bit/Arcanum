// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SkillRow.generated.h"

/**
 * 추영호
 * 스킬 설계용 DataTable Row
 */
USTRUCT(BlueprintType)
struct FSkillRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 스킬 ID (Arcanum.Ability.Skill.*) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FGameplayTag SkillID;

	/* 쿨타임 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cooldown")
	float Cooldown = 0.0f;

	/* 마나 소모량 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Cost")
	float ManaCost = 0.0f;

	/* 스킬 데미지 계수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Damage")
	float DamageMultiplier = 1.0f;

	/* 투사체 사거리 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Projectile")
	float Range = 0.0f;

	/* 투사체 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Projectile")
	float ProjectileSpeed = 0.0f;

	/* 범위 반경 (0이면 단일) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Area")
	float Radius = 0.0f;

	/* 부채꼴 각도 (0이면 미사용) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill|Area")
	float ConeAngle = 0.0f;
};