#include "Component/EquipmentComponent.h"
#include "GameplayTagsManager.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// 슬롯 검증 룰 캐시 초기화
	InitPickRules();

	EquipmentMap.FindOrAdd(EEquipSlot::Weapon);
	EquipmentMap.FindOrAdd(EEquipSlot::Helmet);
	EquipmentMap.FindOrAdd(EEquipSlot::Chest);
	EquipmentMap.FindOrAdd(EEquipSlot::Glove);
	EquipmentMap.FindOrAdd(EEquipSlot::Boot);
}

/* 슬롯 검증 규칙 캐시 (일반무기 2필수 / 전설 선택 / 방어구 4부위 전용) */
void UEquipmentComponent::InitPickRules()
{
	if (!bRulesInitialized)
	{
		WeaponAllowEmpty.Reset();
		ArmorRootBySlot.Reset();

		WeaponAllowEmpty.Add(EWeaponPickSlot::Slot1, false);
		WeaponAllowEmpty.Add(EWeaponPickSlot::Slot2, false);
		WeaponAllowEmpty.Add(EWeaponPickSlot::Legendary, true);

		WeaponRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Weapon.Common"), false);
		LegendaryWeaponRootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Weapon.Legendary"), false);

		ArmorRootBySlot.Add(EEquipSlot::Helmet, UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Armor.Helmet"), false));
		ArmorRootBySlot.Add(EEquipSlot::Chest, UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Armor.Chest"), false));
		ArmorRootBySlot.Add(EEquipSlot::Glove, UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Armor.Glove"), false));
		ArmorRootBySlot.Add(EEquipSlot::Boot, UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.Armor.Boot"), false));

		bRulesInitialized = true;
	}
}

/* SaveGame에서 불러온 로드아웃을 통째로 반영 (로비 UI/스탯 프리뷰 갱신용) */
bool UEquipmentComponent::SetLoadoutData(const FLoadoutData& InLoadoutData)
{
	InitPickRules();

	LoadoutData = InLoadoutData;

	OnWeaponSlotChanged.Broadcast(EWeaponPickSlot::Slot1, LoadoutData.WeaponSlot1Tag);
	OnWeaponSlotChanged.Broadcast(EWeaponPickSlot::Slot2, LoadoutData.WeaponSlot2Tag);
	OnWeaponSlotChanged.Broadcast(EWeaponPickSlot::Legendary, LoadoutData.LegendaryWeaponTag);

	RebuildArmorSetState();
	OnLoadoutChanged.Broadcast();

	return true;
}

/* 로비: 무기 슬롯(1/2/전설) 선택값 1개 교체 + UI/스탯 프리뷰 갱신 */
bool UEquipmentComponent::SetWeaponPick(EWeaponPickSlot InSlot, const FGameplayTag& InWeaponTag)
{
	InitPickRules();

	if (CanPickWeapon(InSlot, InWeaponTag))
	{
		if (InSlot == EWeaponPickSlot::Slot1)
		{
			LoadoutData.WeaponSlot1Tag = InWeaponTag;
		}
		else if (InSlot == EWeaponPickSlot::Slot2)
		{
			LoadoutData.WeaponSlot2Tag = InWeaponTag;
		}
		else
		{
			LoadoutData.LegendaryWeaponTag = InWeaponTag;
		}

		OnWeaponSlotChanged.Broadcast(InSlot, InWeaponTag);
		OnLoadoutChanged.Broadcast();

		return true;
	}

	return false;
}

/* 로비: 방어구 4부위 선택값 교체(각 슬롯 전용 태그만 허용) + 스탯 프리뷰 갱신 */
bool UEquipmentComponent::SetArmorPick(EEquipSlot InSlot, const FGameplayTag& InArmorTag)
{
	InitPickRules();

	if (CanPickArmor(InSlot, InArmorTag))
	{
		if (InSlot == EEquipSlot::Helmet)
		{
			LoadoutData.HelmetTag = InArmorTag;
		}
		else if (InSlot == EEquipSlot::Chest)
		{
			LoadoutData.ArmorTag = InArmorTag;
		}
		else if (InSlot == EEquipSlot::Glove)
		{
			LoadoutData.GloveTag = InArmorTag;
		}
		else
		{
			LoadoutData.BootTag = InArmorTag;
		}

		RebuildArmorSetState();
		OnLoadoutChanged.Broadcast();

		return true;
	}

	return false;
}

/* 게임 시작(또는 로비 확정): 필수 슬롯 검증 통과 시에만 인게임 장착 상태로 커밋 + 스탯 합산 트리거 */
bool UEquipmentComponent::CommitLoadoutToEquipped()
{
	InitPickRules();

	if (!ValidateLoadoutForStart())
	{
		return false;
	}

	CurrentSetMode = EWeaponSetMode::Common;

	bHasWeaponBackup = false;
	CachedWeaponTagBeforeLegendary = FGameplayTag();

	if (LoadoutData.StartWeaponSlot == EStartWeaponSlot::Slot2)
	{
		CurrentCommonWeaponIndex = 1;
	}
	else
	{
		CurrentCommonWeaponIndex = 0;
	}

	const FGameplayTag startWeaponTag = (CurrentCommonWeaponIndex == 0) ? LoadoutData.WeaponSlot1Tag : LoadoutData.WeaponSlot2Tag;

	SetEquippedTag(EEquipSlot::Helmet, LoadoutData.HelmetTag, true);
	SetEquippedTag(EEquipSlot::Chest, LoadoutData.ArmorTag, true);
	SetEquippedTag(EEquipSlot::Glove, LoadoutData.GloveTag, true);
	SetEquippedTag(EEquipSlot::Boot, LoadoutData.BootTag, true);
	SetEquippedTag(EEquipSlot::Weapon, startWeaponTag, true);

	RebuildArmorSetState();
	OnEquipmentStateChanged.Broadcast();

	return true;
}

/* 인게임: 슬롯1 <-> 슬롯2 스왑(전설 중 금지, 실패 시 인덱스/장착 상태 변경 없음) */
bool UEquipmentComponent::SwapCommonWeapon()
{
	InitPickRules();

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

		const FGameplayTag nextWeaponTag = (nextIndex == 0) ? LoadoutData.WeaponSlot1Tag : LoadoutData.WeaponSlot2Tag;

		if (nextWeaponTag.IsValid())
		{
			if (WeaponRootTag.IsValid())
			{
				if (nextWeaponTag.MatchesTag(WeaponRootTag))
				{
					CurrentCommonWeaponIndex = nextIndex;
					SetEquippedTag(EEquipSlot::Weapon, nextWeaponTag, true);

					return true;
				}
			}
		}
	}

	return false;
}

/* 인게임: 전설 모드 요청(전설 태그가 있을 때만, 실제 적용은 ApplyLegendaryWeapon에서) */
bool UEquipmentComponent::ActivateLegendaryMode()
{
	InitPickRules();

	if (CurrentSetMode == EWeaponSetMode::Common)
	{
		if (LoadoutData.LegendaryWeaponTag.IsValid())
		{
			if (CanPickWeapon(EWeaponPickSlot::Legendary, LoadoutData.LegendaryWeaponTag))
			{
				bHasWeaponBackup = false;
				CachedWeaponTagBeforeLegendary = FGameplayTag();

				CurrentSetMode = EWeaponSetMode::Legendary;
				return true;
			}
		}
	}

	return false;
}

/* 인게임: 전설 모드 해제 요청(백업이 있으면 복구 성공해야 true) */
bool UEquipmentComponent::DeactivateLegendaryMode()
{
	if (CurrentSetMode == EWeaponSetMode::Legendary)
	{
		if (bHasWeaponBackup)
		{
			if (RestoreCommonWeapon())
			{
				return true;
			}

			return false;
		}

		CurrentSetMode = EWeaponSetMode::Common;

		return true;
	}

	return false;
}

/* 노티파이 타이밍: 전설 무기 적용(복구용 백업 저장 후 무기 슬롯 교체) */
bool UEquipmentComponent::ApplyLegendaryWeapon()
{
	InitPickRules();

	if (CurrentSetMode == EWeaponSetMode::Legendary)
	{
		if (!bHasWeaponBackup)
		{
			if (LoadoutData.LegendaryWeaponTag.IsValid())
			{
				if (CanPickWeapon(EWeaponPickSlot::Legendary, LoadoutData.LegendaryWeaponTag))
				{
					CachedWeaponTagBeforeLegendary = GetEquippedTag(EEquipSlot::Weapon);

					bHasWeaponBackup = true;

					SetEquippedTag(EEquipSlot::Weapon, LoadoutData.LegendaryWeaponTag, true);

					return true;
				}
			}
		}
	}

	return false;
}

/* 노티파이 타이밍: 전설 종료 후 백업된 일반 무기로 복구(전설 모드 종료 포함) */
bool UEquipmentComponent::RestoreCommonWeapon()
{
	if (bHasWeaponBackup)
	{
		if (CachedWeaponTagBeforeLegendary.IsValid())
		{
			SetEquippedTag(EEquipSlot::Weapon, CachedWeaponTagBeforeLegendary, true);
		}

		bHasWeaponBackup = false;

		CachedWeaponTagBeforeLegendary = FGameplayTag();
		CurrentSetMode = EWeaponSetMode::Common;

		return true;
	}

	return false;
}

/* 내부: 장착 태그 세팅 + 변경 시 OnEquipChanged 브로드캐스트 */
bool UEquipmentComponent::SetEquippedTag(EEquipSlot InSlot, const FGameplayTag& InEquipTag, bool bForce)
{
	FGameplayTag& currentTag = EquipmentMap.FindOrAdd(InSlot);

	if (bForce)
	{
		currentTag = InEquipTag;
		OnEquipChanged.Broadcast(InSlot, InEquipTag);

		return true;
	}

	if (currentTag != InEquipTag)
	{
		currentTag = InEquipTag;
		OnEquipChanged.Broadcast(InSlot, InEquipTag);
	}

	return true;
}

bool UEquipmentComponent::IsWearingFullArmorSet(const FGameplayTag& InSetRoot) const
{
	if (InSetRoot.IsValid())
	{
		return
			LoadoutData.HelmetTag.IsValid() && LoadoutData.HelmetTag.MatchesTag(InSetRoot) &&
			LoadoutData.ArmorTag.IsValid() && LoadoutData.ArmorTag.MatchesTag(InSetRoot) &&
			LoadoutData.GloveTag.IsValid() && LoadoutData.GloveTag.MatchesTag(InSetRoot) &&
			LoadoutData.BootTag.IsValid() && LoadoutData.BootTag.MatchesTag(InSetRoot);
	}

	return false;
}

void UEquipmentComponent::RebuildArmorSetState()
{
	const FGameplayTag manaRoot = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.SetItem.ManaCycle"), false);
	const FGameplayTag itemEffectRoot = UGameplayTagsManager::Get().RequestGameplayTag(FName("Arcanum.Items.SetItem.ItemEffectBonus"), false);

	const bool manaActive = IsWearingFullArmorSet(manaRoot);
	const bool itemEffectActive = IsWearingFullArmorSet(itemEffectRoot);

	const bool wasManaActive = ActiveArmorSets.Contains(manaRoot);
	const bool wasItemEffectActive = ActiveArmorSets.Contains(itemEffectRoot);

	ActiveArmorSets.Reset();
	if (manaActive)
	{
		ActiveArmorSets.Add(manaRoot);
	}
	if (itemEffectActive)
	{
		ActiveArmorSets.Add(itemEffectRoot);
	}

	if (manaRoot.IsValid() && (wasManaActive != manaActive))
	{
		OnArmorSetChanged.Broadcast(manaRoot, manaActive);
	}
	if (itemEffectRoot.IsValid() && (wasItemEffectActive != itemEffectActive))
	{
		OnArmorSetChanged.Broadcast(itemEffectRoot, itemEffectActive);
	}
}

/* 검증: 무기 선택(슬롯1/2=Common 필수, 전설=Legendary 선택) */
bool UEquipmentComponent::CanPickWeapon(EWeaponPickSlot InSlot, const FGameplayTag& InTag) const
{
	const bool* allowEmptyPtr = WeaponAllowEmpty.Find(InSlot);

	if (allowEmptyPtr)
	{
		const bool allowEmpty = *allowEmptyPtr;

		if (!InTag.IsValid())
		{
			if (allowEmpty)
			{
				return true;
			}

			return false;
		}

		if (InSlot == EWeaponPickSlot::Legendary)
		{
			if (LegendaryWeaponRootTag.IsValid())
			{
				if (InTag.MatchesTag(LegendaryWeaponRootTag))
				{
					return true;
				}
			}

			return false;
		}

		if (WeaponRootTag.IsValid())
		{
			if (InTag.MatchesTag(WeaponRootTag))
			{
				return true;
			}
		}
	}

	return false;
}

/* 검증: 방어구 선택 (각 슬롯 전용 루트 태그 매칭) */
bool UEquipmentComponent::CanPickArmor(EEquipSlot InSlot, const FGameplayTag& InTag) const
{
	if (InSlot != EEquipSlot::Weapon)
	{
		const FGameplayTag* rootPtr = ArmorRootBySlot.Find(InSlot);

		if (rootPtr)
		{
			if (InTag.IsValid())
			{
				if (InTag.MatchesTag(*rootPtr))
				{
					return true;
				}
			}
		}
	}

	return false;
}

/* 게임 시작: 일반무기2+방어구4 필수, 전설은 선택 */
bool UEquipmentComponent::ValidateLoadoutForStart() const
{
	// 무기 검사
	const TPair<EWeaponPickSlot, const FGameplayTag*> requiredWeapons[] =
	{
		TPair<EWeaponPickSlot, const FGameplayTag*>(EWeaponPickSlot::Slot1, &LoadoutData.WeaponSlot1Tag),
		TPair<EWeaponPickSlot, const FGameplayTag*>(EWeaponPickSlot::Slot2, &LoadoutData.WeaponSlot2Tag),
	};

	for (const TPair<EWeaponPickSlot, const FGameplayTag*>& pair : requiredWeapons)
	{
		if (CanPickWeapon(pair.Key, *pair.Value) == false)
		{
			return false;
		}
	}

	// 방어구 검사
	const TPair<EEquipSlot, const FGameplayTag*> requiredArmors[] =
	{
		TPair<EEquipSlot, const FGameplayTag*>(EEquipSlot::Helmet, &LoadoutData.HelmetTag),
		TPair<EEquipSlot, const FGameplayTag*>(EEquipSlot::Chest,  &LoadoutData.ArmorTag),
		TPair<EEquipSlot, const FGameplayTag*>(EEquipSlot::Glove,  &LoadoutData.GloveTag),
		TPair<EEquipSlot, const FGameplayTag*>(EEquipSlot::Boot,   &LoadoutData.BootTag),
	};

	for (const TPair<EEquipSlot, const FGameplayTag*>& pair : requiredArmors)
	{
		if (CanPickArmor(pair.Key, *pair.Value) == false)
		{
			return false;
		}
	}

	// 전설 무기 (선택 사항)
	if (LoadoutData.LegendaryWeaponTag.IsValid())
	{
		if (CanPickWeapon(EWeaponPickSlot::Legendary, LoadoutData.LegendaryWeaponTag) == false)
		{
			return false;
		}
	}

	return true;
}