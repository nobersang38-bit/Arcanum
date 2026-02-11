#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Equipment/LoadoutData.h"
#include "GameplayTagContainer.h"
#include "EquipmentComponent.generated.h"

/*
 * 추영호
 * 장비 컴포넌트
 * - 로비(UI): FLoadoutData(출전 세팅/선택값) 관리 + 무기 슬롯 변경 알림
 * - 인게임: 현재 장착 상태 관리(무기 외형/스킬 분기용 "현재 무기"만 바뀜)
 * - 스탯은 무기 3개 + 방어구 4부위를 게임 시작 시점 합산 적용
 * - 인게임 무기 스왑/전설 발동은 스탯 재계산을 하지 않는다(스킬/기본공격 방식/무기 외형만 변경).
 * - 세트 판정/효과 적용은 여기서 하지 않는다. (Stats/Skill) 쪽에서 처리
 */

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Helmet UMETA(DisplayName = "Helmet"),
	Armor  UMETA(DisplayName = "Armor"),
	Glove UMETA(DisplayName = "Glove"),
	Boot  UMETA(DisplayName = "Boot"),
};

/* 캐릭터창 무기 선택 슬롯(슬롯1/슬롯2/전설) */
UENUM(BlueprintType)
enum class EWeaponPickSlot : uint8
{
	Slot1     UMETA(DisplayName = "Slot1"),
	Slot2     UMETA(DisplayName = "Slot2"),
	Legendary UMETA(DisplayName = "Legendary"),
};

/* 인게임 무기 모드 (연출/노티파이 흐름용) */
UENUM(BlueprintType)
enum class EWeaponSetMode : uint8
{
	Common    UMETA(DisplayName = "Common"),
	Legendary UMETA(DisplayName = "Legendary"),
};

/* 인게임 장착 변경 알림 (무기 외형/스킬 분기용: Weapon 슬롯 변경 포함) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipChanged, EEquipSlot, InSlot, FGameplayTag, InNewEquipTag);

/* 장비 전체 상태 변경 알림 (Stats/Skill 재계산 트리거)
 * - 합산형 기준: 게임 시작 또는 방어구 변경 같은 스탯이 바뀌는 타이밍에만 사용
 * - 무기 스왑/전설은 스탯 고정이므로 여기서 브로드캐스트하지 않는 것을 전제로 한다 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentStateChanged);

/* 캐릭터창 무기 슬롯(1/2/전설) 선택값 변경 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotChanged, EWeaponPickSlot, InSlot, FGameplayTag, InNewWeaponTag);


UCLASS(ClassGroup = (Arcanum), meta = (BlueprintSpawnableComponent))
class ARCANUM_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	/* ===============================
	 * 기본 장착/해제
	 * =============================== */

	/* 장비 장착 : InSlot에 InEquipTag를 맵에 저장하고 OnEquipChanged 브로드캐스트  */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment")
	bool EquipItem(EEquipSlot InSlot, const FGameplayTag& InEquipTag, bool bForce = false);

	/* 장비 해제 : InSlot의 태그를 None하고 OnEquipChanged 브로드캐스트 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment")
	bool UnEquipItem(EEquipSlot InSlot);

	/* ===============================
	 * 무기 로드아웃
	 * =============================== */

	 /* 일반 무기 2개 로드아웃 설정 (Index 0,1) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool SetCommonWeaponLoadoutData(const TArray<FGameplayTag>& InWeaponTags);

	/* 전설 무기 태그 설정 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool SetLegendaryWeaponData(const FGameplayTag& InWeaponTag);

	/* 일반 무기 스왑 (Index 0 <-> 1) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool SwapCommonWeapon();

	/* 전설 발동 : 무기만 전설로 교체 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool ActivateLegendaryMode();

	/* 전설 종료 : 원래 일반 무기로 복구 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool DeactivateLegendaryMode();

	/* ===============================
	 * Getters
	 * =============================== */

	/* 현재 선택된 일반 무기 인덱스 */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment|Weapon")
	int32 GetCurrentCommonWeaponIndex() { return CurrentCommonWeaponIndex; }

	/* 현재 전설 모드(변신) 중인가? */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment|Weapon")
	bool IsLegendaryActive() {return (CurrentSetMode == EWeaponSetMode::Legendary); }
	
	/* 현재 무기 태그만 조회 (스킬 컴포넌트 연동) */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment")
	FGameplayTag GetCurrentWeaponTag() { return GetEquippedTag(EEquipSlot::Weapon); }

	/* 현재 장착 태그 조회 */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment")
	FGameplayTag GetEquippedTag(EEquipSlot InSlot) { return EquipmentMap.FindRef(InSlot); }

	/* 방어구 4부위 현재 장착 태그 조회 (4셋 판정용) */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment")
	void GetEquippedState(FGameplayTag& OutHelmet, FGameplayTag& OutArmor, FGameplayTag& OutGlove, FGameplayTag& OutBoot) {
		OutHelmet = GetEquippedTag(EEquipSlot::Helmet);
		OutArmor = GetEquippedTag(EEquipSlot::Armor);
		OutGlove = GetEquippedTag(EEquipSlot::Glove);
		OutBoot = GetEquippedTag(EEquipSlot::Boot);
	}

protected:
	virtual void BeginPlay() override;

public:
	/* 장비 교체 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Equipment")
	FOnEquipChanged OnEquipChanged;

	/* 장비 전체 상태 변경 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Equipment")
	FOnEquipmentStateChanged OnEquipmentStateChanged;

private:
	/* 현재 장착 중인 모든 장비 상태 (TMap) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<EEquipSlot, FGameplayTag> EquipmentMap;

	/* 일반 무기 로드아웃 */
	UPROPERTY(EditAnywhere, Category = "Arcanum|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	TArray<FGameplayTag> CommonWeaponTags;

	/* 전설 무기 1개 */
	UPROPERTY(EditAnywhere, Category = "Arcanum|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	FGameplayTag LegendaryWeaponTag;

	/* 현재 활성화된 일반 무기 인덱스 */
	UPROPERTY(VisibleAnywhere, Category = "Arcanum|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	int32 CurrentCommonWeaponIndex = 0;

	/* 현재 모드 (일반 / 전설) */
	UPROPERTY(VisibleAnywhere, Category = "Arcanum|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	EWeaponSetMode CurrentSetMode = EWeaponSetMode::Common;
};