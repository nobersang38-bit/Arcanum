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
 * - 인게임 무기 스왑/전설 발동은 스탯 재계산 않함(스킬/기본공격 방식/무기 외형만 변경)
 * - 세트 판정/효과 적용은 여기서 하지 않음 (Stats/Skill) 쪽에서 처리
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

/* 캐릭터창 무기 슬롯(1/2/전설) 선택값 변경 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotChanged, EWeaponPickSlot, InSlot, FGameplayTag, InNewWeaponTag);

/* 장비 전체 상태 변경 알림 (스탯이 바뀌는 타이밍에만 사용) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentStateChanged);


UCLASS(ClassGroup = (Arcanum), meta = (BlueprintSpawnableComponent))
class ARCANUM_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	/* ===============================
	 * 로비(UI): 출전 세팅(선택값)
	 * =============================== */

	 /* SaveGame에서 불러온 로드아웃을 적용할 때
		로비 들어오자마자 “마지막 출전 세팅”을 그대로 UI에 반영 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Loadout")
	bool SetLoadoutData(const FLoadoutData& InLoadoutData);

	/* 현재 로비 출전 세팅 반환 */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment|Loadout")
	const FLoadoutData& GetLoadoutData() const { return LoadoutData; }
	
	/* UI에서 슬롯 클릭 후 무기 선택 시 호출 (로비 상태값) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Loadout")
	bool SetWeaponPick(EWeaponPickSlot InSlot, const FGameplayTag& InWeaponTag);

	/* 방어구 로비 상태값 (Weapon이면 실패 처리) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Loadout")
	bool SetArmorPick(EEquipSlot InSlot, const FGameplayTag& InArmorTag);

	/*
	 * 게임 시작(또는 로비 확정): 선택값을 인게임 장착 상태로 확정
	 * - 이때만 Stats/Skill 재계산 트리거(OnEquipmentStateChanged)를 쏜다
	 * - StartWeaponIndex(0=Slot1, 1=Slot2) 기준으로 현재 일반무기 세팅
	 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Loadout")
	bool CommitLoadoutToEquipped();

	/* ===============================
	 * 인게임: 무기 스왑/전설
	 * =============================== */

    /* 슬롯1 <-> 슬롯2 스왑 : 현재 무기/외형/스킬 분기만 바뀐다(스탯 재계산 X) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool SwapCommonWeapon();

	/* 전설 모드 요청(쿨타임 관리는 Skill/Combat에서) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool ActivateLegendaryMode();

	/* 전설 모드 해제 요청 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool DeactivateLegendaryMode();

	/* 전설 무기 적용/복구 타이밍에서 호출 (노티파이) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool ApplyLegendaryWeapon();

	/* 전설 종료 후 원래 일반 무기로 복구 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Weapon")
	bool RestoreCommonWeapon();

	/* ===============================
	 * Getters
	 * =============================== */

	/* 현재 선택된 일반 무기 인덱스(0=슬롯1, 1=슬롯2) */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment|Weapon")
	int32 GetCurrentCommonWeaponIndex() const { return CurrentCommonWeaponIndex; }

	/* 전설 모드 활성 여부(전설 버튼/쿨 UI 연동, 입력 게이트 등에 사용) */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment|Weapon")
	bool IsLegendaryActive() const { return (CurrentSetMode == EWeaponSetMode::Legendary); }

	/* 현재 무기 모드(Common/Legendary) */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment|Weapon")
	EWeaponSetMode GetCurrentWeaponMode() const { return CurrentSetMode; }

	/* 현재 장착 무기 태그(무기 외형/스킬 분기/기본공격 분기용) */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment")
	FGameplayTag GetCurrentWeaponTag() const { return GetEquippedTag(EEquipSlot::Weapon); }

	/* 특정 슬롯의 현재 장착 태그를 반환 */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment")
	FGameplayTag GetEquippedTag(EEquipSlot InSlot) const { return EquipmentMap.FindRef(InSlot); }

	/* 방어구 4부위의 현재 장착 태그를 한번에 반환 */
	UFUNCTION(BlueprintPure, Category = "Arcanum|Equipment")
	void GetEquippedArmorState(FGameplayTag& OutHelmet, FGameplayTag& OutArmor, FGameplayTag& OutGlove, FGameplayTag& OutBoot) const
	{
		OutHelmet = GetEquippedTag(EEquipSlot::Helmet);
		OutArmor = GetEquippedTag(EEquipSlot::Armor);
		OutGlove = GetEquippedTag(EEquipSlot::Glove);
		OutBoot = GetEquippedTag(EEquipSlot::Boot);
	}

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Equipment")
	FOnEquipChanged OnEquipChanged;

	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Equipment")
	FOnEquipmentStateChanged OnEquipmentStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Equipment|Loadout")
	FOnWeaponSlotChanged OnWeaponSlotChanged;

private:
	/* ===============================
	 * 내부 헬퍼(인게임 실제 장착값 세팅)
	 * - 외부에서 임의로 방어구를 바꾸지 않게, Commit/Swap/Legendary 흐름으로만 세팅하기 위한 용도
	 * =============================== */
	bool SetEquippedTag(EEquipSlot InSlot, const FGameplayTag& InEquipTag, bool bForce);

	/* 인게임: 현재 장착 상태(무기 외형/스킬 분기용) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment", meta = (AllowPrivateAccess = "true"))
	TMap<EEquipSlot, FGameplayTag> EquipmentMap;

	/* 로비(UI): 출전 세팅(선택값) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment|Loadout", meta = (AllowPrivateAccess = "true"))
	FLoadoutData LoadoutData;

	/* 인게임: 일반 무기 인덱스(0=슬롯1, 1=슬롯2) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	int32 CurrentCommonWeaponIndex = 0;

	/* 인게임: 무기 모드 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	EWeaponSetMode CurrentSetMode = EWeaponSetMode::Common;

	/* 전설 복구용 캐시(전설 적용 직전 무기) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	FGameplayTag CachedWeaponTagBeforeLegendary;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment|Weapon", meta = (AllowPrivateAccess = "true"))
	bool bHasWeaponBackup = false;
};