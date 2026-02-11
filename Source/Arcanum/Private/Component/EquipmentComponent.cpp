#include "Component/EquipmentComponent.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// 초기화 일반 무기 로드아웃 0번 자동 장착
	if (CommonWeaponTags.IsValidIndex(0) == true)
	{
		if (CommonWeaponTags[0].IsValid() == true)
		{
			EquipItem(EEquipSlot::Weapon, CommonWeaponTags[0]);
		}
	}
}

bool UEquipmentComponent::EquipItem(EEquipSlot InSlot, const FGameplayTag& InEquipTag, bool bForce)
{
	if (InEquipTag.IsValid())
	{
		// 전설 모드 중에는 무기 슬롯 교체 차단
		if (bForce || (CurrentSetMode != EWeaponSetMode::Legendary) || (InSlot != EEquipSlot::Weapon))
		{
			FGameplayTag* currentTag = EquipmentMap.Find(InSlot);

			if (currentTag == nullptr || *currentTag != InEquipTag)
			{
				EquipmentMap.Emplace(InSlot, InEquipTag);

				OnEquipChanged.Broadcast(InSlot, InEquipTag);
				OnEquipmentStateChanged.Broadcast();

				return true;
			}
		}
	}

	return false;
}

bool UEquipmentComponent::UnEquipItem(EEquipSlot InSlot)
{
	if (EquipmentMap.Contains(InSlot))
	{
		EquipmentMap.Remove(InSlot);

		OnEquipChanged.Broadcast(InSlot, FGameplayTag::EmptyTag);
		OnEquipmentStateChanged.Broadcast();

		return true;
	}

	return false;
}

bool UEquipmentComponent::SetCommonWeaponLoadoutData(const TArray<FGameplayTag>& InWeaponTags)
{
	if (InWeaponTags.Num() == 2)
	{
		CommonWeaponTags = InWeaponTags;

		// 인덱스 보정
		if (CurrentCommonWeaponIndex != 0 && CurrentCommonWeaponIndex != 1)
		{
			CurrentCommonWeaponIndex = 0;
		}

		return true;
	}

	return false;
}

bool UEquipmentComponent::SetLegendaryWeaponData(const FGameplayTag& InWeaponTag)
{
	if (InWeaponTag.IsValid())
	{
		LegendaryWeaponTag = InWeaponTag;

		return true;
	}

	return false;
}

bool UEquipmentComponent::SwapCommonWeapon()
{
	if (CurrentSetMode == EWeaponSetMode::Common)
	{
		// 0 <-> 1 토글
		if (CurrentCommonWeaponIndex == 0)
		{
			CurrentCommonWeaponIndex = 1;
		}
		else
		{
			CurrentCommonWeaponIndex = 0;
		}

		if (CommonWeaponTags.IsValidIndex(CurrentCommonWeaponIndex) == true)
		{
			const FGameplayTag& targetWeaponTag = CommonWeaponTags[CurrentCommonWeaponIndex];

			if (targetWeaponTag.IsValid())
			{
				EquipItem(EEquipSlot::Weapon, targetWeaponTag);
				return true;
			}
		}
	}

	return false;
}

bool UEquipmentComponent::ActivateLegendaryMode()
{
	if (CurrentSetMode == EWeaponSetMode::Common)
	{
		if (LegendaryWeaponTag.IsValid())
		{
			CurrentSetMode = EWeaponSetMode::Legendary;

			EquipItem(EEquipSlot::Weapon, LegendaryWeaponTag, true);

			return true;
		}
	}

	return false;
}

bool UEquipmentComponent::DeactivateLegendaryMode()
{
	if (CurrentSetMode == EWeaponSetMode::Legendary)
	{
		CurrentSetMode = EWeaponSetMode::Common;

		if (CommonWeaponTags.IsValidIndex(CurrentCommonWeaponIndex) == true)
		{
			const FGameplayTag& restoreWeaponTag = CommonWeaponTags[CurrentCommonWeaponIndex];

			if (restoreWeaponTag.IsValid())
			{
				EquipItem(EEquipSlot::Weapon, restoreWeaponTag, true);
				return true;
			}
		}
	}

	return false;
}
