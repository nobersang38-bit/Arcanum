#include "Component/EquipmentComponent.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

/* 컴포 시작 시 장착 맵 키 초기화 */
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	EquipmentMap.FindOrAdd(EEquipSlot::Weapon);
	EquipmentMap.FindOrAdd(EEquipSlot::Helmet);
	EquipmentMap.FindOrAdd(EEquipSlot::Armor);
	EquipmentMap.FindOrAdd(EEquipSlot::Glove);
	EquipmentMap.FindOrAdd(EEquipSlot::Boot);
}

/* SaveGame 등에서 로드아웃을 통째로 반영(로비 UI 갱신용) */
bool UEquipmentComponent::SetLoadoutData(const FLoadoutData& InLoadoutData)
{
	LoadoutData = InLoadoutData;

	if (LoadoutData.StartWeaponIndex != 0 && LoadoutData.StartWeaponIndex != 1)
	{
		LoadoutData.StartWeaponIndex = 0;
	}

	OnWeaponSlotChanged.Broadcast(EWeaponPickSlot::Slot1, LoadoutData.WeaponSlot1Tag);
	OnWeaponSlotChanged.Broadcast(EWeaponPickSlot::Slot2, LoadoutData.WeaponSlot2Tag);
	OnWeaponSlotChanged.Broadcast(EWeaponPickSlot::Legendary, LoadoutData.LegendaryWeaponTag);

	return true;
}

/* 로비에서 무기 슬롯(1/2/전설) 선택값 1개만 교체 */
bool UEquipmentComponent::SetWeaponPick(EWeaponPickSlot InSlot, const FGameplayTag& InWeaponTag)
{
	if (InWeaponTag.IsValid())
	{
		if (InSlot == EWeaponPickSlot::Slot1)
		{
			LoadoutData.WeaponSlot1Tag = InWeaponTag;
			OnWeaponSlotChanged.Broadcast(InSlot, InWeaponTag);
			return true;
		}
		else if (InSlot == EWeaponPickSlot::Slot2)
		{
			LoadoutData.WeaponSlot2Tag = InWeaponTag;
			OnWeaponSlotChanged.Broadcast(InSlot, InWeaponTag);
			return true;
		}
		else if (InSlot == EWeaponPickSlot::Legendary)
		{
			LoadoutData.LegendaryWeaponTag = InWeaponTag;
			OnWeaponSlotChanged.Broadcast(InSlot, InWeaponTag);
			return true;
		}
	}

	return false;
}

/* 로비에서 방어구 4부위 선택값 교체(Weapon 슬롯은 금지) */
bool UEquipmentComponent::SetArmorPick(EEquipSlot InSlot, const FGameplayTag& InArmorTag)
{
	if (InSlot == EEquipSlot::Weapon)
	{
		return false;
	}

	if (InSlot == EEquipSlot::Helmet)
	{
		LoadoutData.HelmetTag = InArmorTag;
		return true;
	}
	else if (InSlot == EEquipSlot::Armor)
	{
		LoadoutData.ArmorTag = InArmorTag;
		return true;
	}
	else if (InSlot == EEquipSlot::Glove)
	{
		LoadoutData.GloveTag = InArmorTag;
		return true;
	}
	else if (InSlot == EEquipSlot::Boot)
	{
		LoadoutData.BootTag = InArmorTag;
		return true;
	}

	return false;
}

/* 로드아웃을 인게임 장착 상태로 확정(스탯 합산 트리거는 여기서만) */
bool UEquipmentComponent::CommitLoadoutToEquipped()
{
	CurrentSetMode = EWeaponSetMode::Common;
	bHasWeaponBackup = false;
	CachedWeaponTagBeforeLegendary = FGameplayTag();

	if (LoadoutData.StartWeaponIndex == 0 || LoadoutData.StartWeaponIndex == 1)
	{
		CurrentCommonWeaponIndex = LoadoutData.StartWeaponIndex;
	}
	else
	{
		CurrentCommonWeaponIndex = 0;
		LoadoutData.StartWeaponIndex = 0;
	}

	FGameplayTag startWeaponTag;

	if (CurrentCommonWeaponIndex == 0)
	{
		startWeaponTag = LoadoutData.WeaponSlot1Tag;
	}
	else
	{
		startWeaponTag = LoadoutData.WeaponSlot2Tag;
	}

	if (startWeaponTag.IsValid())
	{
		SetEquippedTag(EEquipSlot::Helmet, LoadoutData.HelmetTag, true);
		SetEquippedTag(EEquipSlot::Armor, LoadoutData.ArmorTag, true);
		SetEquippedTag(EEquipSlot::Glove, LoadoutData.GloveTag, true);
		SetEquippedTag(EEquipSlot::Boot, LoadoutData.BootTag, true);
		SetEquippedTag(EEquipSlot::Weapon, startWeaponTag, true);

		OnEquipmentStateChanged.Broadcast();
		return true;
	}

	return false;
}

/* 인게임 스왑: 슬롯1 <-> 슬롯2 (전설 중에는 금지, 실패 시 인덱스/장착 상태 변경 없음) */
bool UEquipmentComponent::SwapCommonWeapon()
{
	if (CurrentSetMode == EWeaponSetMode::Common)
	{
		int32 nextIndex = 0;

		if (CurrentCommonWeaponIndex == 0)
		{
			nextIndex = 1;
		}
		else
		{
			nextIndex = 0;
		}

		FGameplayTag nextWeaponTag;

		if (nextIndex == 0)
		{
			nextWeaponTag = LoadoutData.WeaponSlot1Tag;
		}
		else
		{
			nextWeaponTag = LoadoutData.WeaponSlot2Tag;
		}

		if (nextWeaponTag.IsValid())
		{
			CurrentCommonWeaponIndex = nextIndex;
			SetEquippedTag(EEquipSlot::Weapon, nextWeaponTag, true);
			return true;
		}
	}

	return false;
}


/* 전설 모드 시작 요청(실제 적용은 ApplyLegendaryWeapon에서) */
bool UEquipmentComponent::ActivateLegendaryMode()
{
	if (CurrentSetMode == EWeaponSetMode::Common)
	{
		if (LoadoutData.LegendaryWeaponTag.IsValid())
		{
			CurrentSetMode = EWeaponSetMode::Legendary;
			return true;
		}
	}

	return false;
}

/* 전설 모드 종료 요청(전설 무기가 적용돼있으면 복구 포함) */
bool UEquipmentComponent::DeactivateLegendaryMode()
{
	if (CurrentSetMode == EWeaponSetMode::Legendary)
	{
		if (bHasWeaponBackup)
		{
			RestoreCommonWeapon();
		}

		CurrentSetMode = EWeaponSetMode::Common;
		return true;
	}

	return false;
}

/* 노티파이 타이밍: 전설 무기를 지금 무기 슬롯에 적용(원복용 백업 포함) */
bool UEquipmentComponent::ApplyLegendaryWeapon()
{
	if (CurrentSetMode == EWeaponSetMode::Legendary)
	{
		if (bHasWeaponBackup)
		{
			return false;
		}
		else
		{
			if (LoadoutData.LegendaryWeaponTag.IsValid())
			{
				CachedWeaponTagBeforeLegendary = GetEquippedTag(EEquipSlot::Weapon);
				bHasWeaponBackup = true;

				SetEquippedTag(EEquipSlot::Weapon, LoadoutData.LegendaryWeaponTag, true);
				return true;
			}
		}
	}

	return false;
}

/* 전설 종료 타이밍: 백업해둔 일반 무기로 복구 */
bool UEquipmentComponent::RestoreCommonWeapon()
{
	if (bHasWeaponBackup)
	{
		SetEquippedTag(EEquipSlot::Weapon, CachedWeaponTagBeforeLegendary, true);

		bHasWeaponBackup = false;
		CachedWeaponTagBeforeLegendary = FGameplayTag();
		CurrentSetMode = EWeaponSetMode::Common;

		return true;
	}

	return false;
}

/* 내부 공용: 장착 태그 세팅 + OnEquipChanged 브로드캐스트 */
bool UEquipmentComponent::SetEquippedTag(EEquipSlot InSlot, const FGameplayTag& InEquipTag, bool bForce)
{
	FGameplayTag& currentTag = EquipmentMap.FindOrAdd(InSlot);

	if (bForce)
	{
		currentTag = InEquipTag;
		OnEquipChanged.Broadcast(InSlot, InEquipTag);
		return true;
	}
	else
	{
		if (currentTag == InEquipTag)
		{
			return true;
		}
		else
		{
			currentTag = InEquipTag;
			OnEquipChanged.Broadcast(InSlot, InEquipTag);
			return true;
		}
	}
}