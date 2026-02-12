// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LoadoutData.generated.h"

/*
 * 추영호
 * 캐릭터 출전 세팅 데이터
 * - 캐릭터 선택/정보창에서 무기 3개(슬롯1/슬롯2/전설) + 방어구 4부위를 선택해 저장
 * - 인게임 시작 시 이 데이터를 기준으로 실제 장착 상태를 확정한다(현재 장착 무기는 슬롯1)
 * - 로비에서는 무기/방어구 외형을 표시하지 않으며(캐릭터 메시만 변경), 인게임에서 무기 외형만 부착
 */

UENUM(BlueprintType)
enum class EStartWeaponSlot : uint8
{
	Slot1 UMETA(DisplayName = "Slot1"),
	Slot2 UMETA(DisplayName = "Slot2"),
};

USTRUCT(BlueprintType)
struct FLoadoutData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponSlot1Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponSlot2Tag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag LegendaryWeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag HelmetTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ArmorTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag GloveTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BootTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStartWeaponSlot StartWeaponSlot = EStartWeaponSlot::Slot1;
};
