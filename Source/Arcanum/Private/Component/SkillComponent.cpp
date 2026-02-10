#include "Component/SkillComponent.h"
#include "Component/EquipmentComponent.h"
#include "Component/PoolingComponent.h"
#include "Interface/Poolable.h"
#include "Data/Rows/WeaponSkillMapRow.h"
#include "Data/Skill/SkillRow.h"
#include "Data/Skill/EffectRow.h"
#include "GameplayTags/ArcanumTags.h"

USkillComponent::USkillComponent()
{
}

void USkillComponent::BeginPlay()
{
}

bool USkillComponent::ActivateWeaponSkill()
{
	return false;
}

bool USkillComponent::ActivateLegendaryWeaponSkill()
{
	return false;
}

void USkillComponent::EndLegendaryWeaponSkill()
{
}

void USkillComponent::SetEquipmentComponent(UEquipmentComponent* InEquipmentComponent)
{
}

void USkillComponent::HandleEquipChanged(EEquipSlot InSlot, FGameplayTag InNewEquipTag)
{
}

void USkillComponent::BindEquipment()
{
}

void USkillComponent::BuildCaches()
{
}

bool USkillComponent::ResolveSkillIDByWeaponTag(const FGameplayTag& InWeaponTag, FGameplayTag& OutSkillIDTag) const
{
	return false;
}

const FSkillRow* USkillComponent::FindSkillRowBySkillID(const FGameplayTag& InSkillIDTag) const
{
	return nullptr;
}

bool USkillComponent::ExecuteSkillRow(const FSkillRow& InSkillRow)
{
	return false;
}


/*
USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!EquipmentComponent)
	{
		if (AActor* owner = GetOwner())
		{
			EquipmentComponent = owner->FindComponentByClass<UEquipmentComponent>();
		}
	}

	BindEquipment();
}

void USkillComponent::SetEquipmentComponent(UEquipmentComponent* InEquipmentComponent)
{
	if (EquipmentComponent)
	{
		EquipmentComponent->OnEquipChanged.RemoveDynamic(this, &USkillComponent::HandleEquipChanged);
	}

	EquipmentComponent = InEquipmentComponent;
	BindEquipment();
}

void USkillComponent::BindEquipment()
{
	if (EquipmentComponent)
	{
		EquipmentComponent->OnEquipChanged.RemoveDynamic(this, &USkillComponent::HandleEquipChanged);
		EquipmentComponent->OnEquipChanged.AddDynamic(this, &USkillComponent::HandleEquipChanged);

		WeaponSkillIDByWeaponTag(EquipmentComponent->GetCurrentWeaponTag());
	}
}

void USkillComponent::BuildCaches()
{
}

void USkillComponent::HandleEquipChanged(EEquipSlot InSlot, FGameplayTag InNewEquipTag)
{
	if (InSlot == EEquipSlot::Weapon)
	{
		WeaponSkillIDByWeaponTag(InNewEquipTag);
	}
}

void USkillComponent::WeaponSkillIDByWeaponTag(const FGameplayTag& InWeaponTag)
{
	CurrentWeaponSkillIDTag = FGameplayTag();

	if (InWeaponTag.IsValid())
	{
		if (WeaponSkillMapTable)
		{
			TArray<FWeaponSkillMapRow*> rows;
			WeaponSkillMapTable->GetAllRows(TEXT("WeaponSkillIDByWeaponTag"), rows);

			for (FWeaponSkillMapRow* row : rows)
			{
				if (row)
				{
					// Common이면 CommonSkillID만, Legendary면 LegendarySkillID만 채워짐
					if (row->WeaponTag == InWeaponTag)
					{
						if (row->CommonSkillID.IsValid())
						{
							CurrentWeaponSkillIDTag = row->CommonSkillID;
						}
						else if (row->LegendarySkillID.IsValid())
						{
							CurrentWeaponSkillIDTag = row->LegendarySkillID;
						}

						break;
					}
				}
			}
		}
	}
}

// TODO : 나중에 SkillIDTag -> Row* 캐시(TMap)
FSkillRow* USkillComponent::FindSkillRowBySkillID(const FGameplayTag& InSkillIDTag) const
{
	if (InSkillIDTag.IsValid())
	{
		if (SkillTable)
		{
			TArray<FSkillRow*> rows;
			SkillTable->GetAllRows(TEXT("FindSkillRowBySkillID"), rows);

			for (FSkillRow* row : rows)
			{
				if (row)
				{
					if (row->SkillID == InSkillIDTag)
					{
						return row;
					}
				}
			}
		}
	}

	return nullptr;
}

bool USkillComponent::ExecuteSkillRow(const FSkillRow& InSkillRow)
{
	if (!EffectTable) { return false; }

	bool bAnyProcessed = false;

	for (const FName& effectID : InSkillRow.EffectIDs)
	{
		if (!effectID.IsNone())
		{
			if (const FEffectRow* effectRow = EffectTable->FindRow<FEffectRow>(effectID, TEXT("ExecuteSkillRow")))
			{
				// TODO: 여기서 EffectTypeTag 분기
				// 풀링, damage, heal, debuff, 

				bAnyProcessed = true;
			}
		}
	}

	return bAnyProcessed;
}

bool USkillComponent::ActivateWeaponSkill()
{
	// 캐시가 비었으면 현재 무기 태그로 갱신
	if (!CurrentWeaponSkillIDTag.IsValid())
	{
		if (EquipmentComponent)
		{
			WeaponSkillIDByWeaponTag(EquipmentComponent->GetCurrentWeaponTag());
		}
	}

	if (CurrentWeaponSkillIDTag.IsValid())
	{
		FSkillRow* skillRow = FindSkillRowBySkillID(CurrentWeaponSkillIDTag);
		if (skillRow)
		{
			if (ExecuteSkillRow(*skillRow))
			{
				OnWeaponSkillExecuted.Broadcast(GetOwner(), CurrentWeaponSkillIDTag);
				return true;
			}
		}
	}

	return false;
}
*/

