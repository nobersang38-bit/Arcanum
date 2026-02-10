#include "Component/EquipmentComponent.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UEquipmentComponent::EquipByTag(EEquipSlot InSlot, const FGameplayTag& InEquipTag)
{
	if (InEquipTag.IsValid())
	{
		FGameplayTag* slotTag = nullptr;

		switch (InSlot)
		{
		case EEquipSlot::Weapon: 
			slotTag = &WeaponTag; 
			break;
		case EEquipSlot::Helmet:
			slotTag = &HelmetTag; break;
		case EEquipSlot::Armor: 
			slotTag = &ArmorTag; break;
		case EEquipSlot::Gloves: 
			slotTag = &GlovesTag;
			break;
		case EEquipSlot::Shoes:
			slotTag = &ShoesTag; 
			break;
		default:
			break;
		}

		if (slotTag)
		{
			if (*slotTag != InEquipTag)
			{
				*slotTag = InEquipTag;
				OnEquipChanged.Broadcast(InSlot, *slotTag);

				return true;
			}
		}
	}

	return false;
}

bool UEquipmentComponent::UnEquip(EEquipSlot InSlot)
{
	FGameplayTag* slotTag = nullptr;

	switch (InSlot)
	{
	case EEquipSlot::Weapon: 
		slotTag = &WeaponTag;
		break;
	case EEquipSlot::Helmet: 
		slotTag = &HelmetTag;
		break;
	case EEquipSlot::Armor:  
		slotTag = &ArmorTag; 
		break;
	case EEquipSlot::Gloves:
		slotTag = &GlovesTag; 
		break;
	case EEquipSlot::Shoes: 
		slotTag = &ShoesTag; 
		break;
	default:
		break;
	}

	if (slotTag)
	{
		if (slotTag->IsValid())
		{
			*slotTag = FGameplayTag();
			OnEquipChanged.Broadcast(InSlot, *slotTag);

			return true;
		}
	}

	return false;
}

FGameplayTag UEquipmentComponent::GetEquippedTag(EEquipSlot InSlot) const
{
	FGameplayTag equippedTag;

	switch (InSlot)
	{
	case EEquipSlot::Weapon: 
		equippedTag = WeaponTag; break;
	case EEquipSlot::Helmet: 
		equippedTag = HelmetTag; break;
	case EEquipSlot::Armor:  
		equippedTag = ArmorTag;  break;
	case EEquipSlot::Gloves: 
		equippedTag = GlovesTag; break;
	case EEquipSlot::Shoes: 
		equippedTag = ShoesTag; 
		break;
	default: equippedTag = FGameplayTag();
		break;
	}

	return equippedTag;
}

void UEquipmentComponent::SetNormalWeaponHelmetSet(int32 InIndex, const FWeaponHelmetSet& InSet)
{
	if (InIndex < 0 || InIndex > 1) return;

	NormalSets[InIndex] = InSet;

	// 현재 일반 모드이고, 지금 사용 중인 세트를 갱신하는 경우 즉시 반영
	if (CurrentSetMode == EWeaponSetMode::Normal && CurrentNormalSetIndex == InIndex)
	{
		if (NormalSets[InIndex].WeaponTag.IsValid())
		{
			EquipByTag(EEquipSlot::Weapon, NormalSets[InIndex].WeaponTag);
		}

		if (NormalSets[InIndex].HelmetTag.IsValid())
		{
			EquipByTag(EEquipSlot::Helmet, NormalSets[InIndex].HelmetTag);
		}
	}
}

void UEquipmentComponent::SetLegendaryWeaponHelmetSet(const FWeaponHelmetSet& InSet)
{
	LegendarySet = InSet;

	// 전설 모드인 상태에서 세트를 바꾸면 즉시 반영
	if (CurrentSetMode == EWeaponSetMode::Legendary)
	{
		if (LegendarySet.WeaponTag.IsValid())
		{
			EquipByTag(EEquipSlot::Weapon, LegendarySet.WeaponTag);
		}

		if (LegendarySet.HelmetTag.IsValid())
		{
			EquipByTag(EEquipSlot::Helmet, LegendarySet.HelmetTag);
		}
	}
}

bool UEquipmentComponent::SwapNormalWeaponSet()
{
	if (CurrentSetMode == EWeaponSetMode::Legendary) { return false; }

	int32 nextIndex = (CurrentNormalSetIndex == 0) ? 1 : 0;
	FWeaponHelmetSet& nextSet = NormalSets[nextIndex];

	CurrentSetMode = EWeaponSetMode::Normal;
	CurrentNormalSetIndex = nextIndex;

	bool bWeaponChanged = false;
	bool bHelmetChanged = false;

	if (nextSet.WeaponTag.IsValid())
	{
		bWeaponChanged = EquipByTag(EEquipSlot::Weapon, nextSet.WeaponTag);
	}

	if (nextSet.HelmetTag.IsValid())
	{
		bHelmetChanged = EquipByTag(EEquipSlot::Helmet, nextSet.HelmetTag);
	}

	return (bWeaponChanged || bHelmetChanged);
}

bool UEquipmentComponent::ActivateLegendaryWeaponSet()
{
	if (CurrentSetMode == EWeaponSetMode::Legendary) { return false; }

	bool bWeaponChanged = false;
	bool bHelmetChanged = false;

	if (LegendarySet.WeaponTag.IsValid())
	{
		bWeaponChanged = EquipByTag(EEquipSlot::Weapon, LegendarySet.WeaponTag);
	}

	if (LegendarySet.HelmetTag.IsValid())
	{
		bHelmetChanged = EquipByTag(EEquipSlot::Helmet, LegendarySet.HelmetTag);
	}

	if (bWeaponChanged || bHelmetChanged)
	{
		SavedNormalSetIndex = CurrentNormalSetIndex;
		CurrentSetMode = EWeaponSetMode::Legendary;

		return true;
	}

	return false;
}

bool UEquipmentComponent::DeactivateLegendaryWeaponSet()
{
	if (CurrentSetMode != EWeaponSetMode::Legendary) { return false; }

    FWeaponHelmetSet& restoreSet = NormalSets[SavedNormalSetIndex];

	bool bWeaponChanged = false;
	bool bHelmetChanged = false;

	if (restoreSet.WeaponTag.IsValid())
	{
		bWeaponChanged = EquipByTag(EEquipSlot::Weapon, restoreSet.WeaponTag);
	}

	if (restoreSet.HelmetTag.IsValid())
	{
		bHelmetChanged = EquipByTag(EEquipSlot::Helmet, restoreSet.HelmetTag);
	}

	CurrentNormalSetIndex = SavedNormalSetIndex;
	CurrentSetMode = EWeaponSetMode::Normal;

	return (bWeaponChanged || bHelmetChanged);
}
