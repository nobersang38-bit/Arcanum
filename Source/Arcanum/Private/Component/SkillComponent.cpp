#include "Component/SkillComponent.h"
#include "Component/EquipmentComponent.h"
#include "GameFramework/Character.h"
#include "GameplayTags/ArcanumTags.h"
#include "DataInfo/Skills/Instances/USkillBase.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();
}

void USkillComponent::InitComponent()
{
	if (bInitialized) { return; }

	if (AActor* const owner = GetOwner())
	{
		if (!EquipmentComponent)
		{
			EquipmentComponent = owner->FindComponentByClass<UEquipmentComponent>();
		}

		if (WeaponSkillMappingTable)
		{
			InitWeaponSkillMap();
		}

		if (SkillsDataTable)
		{
			if (!bSkillRowNameCacheBuilt)
			{
				BuildSkillRowNameCache();
			}
		}

		if (EquipmentComponent)
		{
			EquipmentComponent->OnEquipChanged.RemoveDynamic(this, &USkillComponent::HandleEquipChanged);
			EquipmentComponent->OnEquipChanged.AddDynamic(this, &USkillComponent::HandleEquipChanged);

			CurrentWeaponTag = EquipmentComponent->GetCurrentWeaponTag();
		}

		// 준비됐을 때만 초기화 완료
		if (EquipmentComponent && (WeaponSkillMappingTable || SkillsDataTable))
		{
			bInitialized = true;
		}
	}
}

void USkillComponent::BuildSkillRowNameCache()
{
	if (!bSkillRowNameCacheBuilt)
	{
		if (SkillsDataTable)
		{
			SkillRowNameByTag.Reset();

			SkillsDataTable->ForeachRow<FDTSkillsDataRow>(TEXT("BuildSkillRowNameCache"),
				[this](const FName& rowName, const FDTSkillsDataRow& row)
				{
					const FGameplayTag skillTag = row.SkillData.SkillNameTag;

					if (skillTag.IsValid())
					{
						SkillRowNameByTag.Add(skillTag, rowName);
					}
				}
			);

			bSkillRowNameCacheBuilt = true;
		}
	}
}

bool USkillComponent::ActivateSkill(const FGameplayTag& InSkillIDTag)
{
	UE_LOG(LogTemp, Warning, TEXT("ActivateSkill: %s"), *InSkillIDTag.ToString());

	if (InSkillIDTag.IsValid())
	{
		USkillBase* skill = GetSkill(InSkillIDTag);
		if (!skill)
		{
			if (AcquireSkill(InSkillIDTag))
			{
				skill = GetSkill(InSkillIDTag);
			}
		}
		if (skill)
		{
			if (FCooldownData* cooldownPtr = CooldownByTag.Find(InSkillIDTag))
			{
				if (cooldownPtr->RemainingSec > 0.0f)
				{
					OnSkillFailed.Broadcast(InSkillIDTag);

					return false;
				}
			}

			if (AActor* owner = GetOwner())
			{
				if (const FLevelModifierEntry* entry = skill->GetCurrentLevelEntry())
				{
					// 코스트(마나) 체크/차감은 StatsComponent
					// TODO: 스탯 컴포 타입/함수명으로 교체
					// UStatsComponent* stats = owner->FindComponentByClass<UStatsComponent>();
					// if (!stats) { OnSkillFailed.Broadcast(InSkillIDTag); return false; }
					// if (!stats->TryConsumeCost(entry->Cost)) { OnSkillFailed.Broadcast(InSkillIDTag); return false; }

					skill->ActivateSkill(owner);

					if (entry->Cooldown > 0.0f)
					{
						StartOrRefreshCooldown(InSkillIDTag, entry->Cooldown);
					}

					return true;
				}
			}
		}

		OnSkillFailed.Broadcast(InSkillIDTag);
	}

	return false;
}

bool USkillComponent::AcquireSkill(const FGameplayTag& InSkillIDTag)
{
	if (InSkillIDTag.IsValid())
	{
		if (SkillsDataTable)
		{
			if (!bSkillRowNameCacheBuilt)
			{
				BuildSkillRowNameCache();
			}

			if (OwnedSkills.Contains(InSkillIDTag))
			{
				return true;
			}

			if (const FName* rowNamePtr = SkillRowNameByTag.Find(InSkillIDTag))
			{
				if (FDTSkillsDataRow* row = SkillsDataTable->FindRow<FDTSkillsDataRow>(*rowNamePtr, TEXT("AcquireSkill")))
				{
					const FSkillInfo& skillInfo = row->SkillData;

					if (row->SkillClass)
					{
						if (AActor* owner = GetOwner())
						{
							USkillBase* newSkill = NewObject<USkillBase>(this, row->SkillClass);
							if (newSkill)
							{
								newSkill->Initialize(owner, skillInfo);

								OwnedSkills.Add(InSkillIDTag, newSkill);

								return true;
							}
						}
					}
				}
			}
		}
	}

	return false;
}

bool USkillComponent::RemoveSkill(const FGameplayTag& InSkillIDTag)
{
	if (InSkillIDTag.IsValid())
	{
		if (OwnedSkills.Contains(InSkillIDTag))
		{
			if (USkillBase* skill = OwnedSkills.FindRef(InSkillIDTag))
			{
				AActor* owner = GetOwner();

				skill->DeactivateSkill(owner);
			}

			OwnedSkills.Remove(InSkillIDTag);

			return true;
		}
	}

	return false;
}

USkillBase* USkillComponent::GetSkill(const FGameplayTag& InSkillIDTag) const
{
	if (InSkillIDTag.IsValid())
	{
		if (OwnedSkills.Contains(InSkillIDTag))
		{
			return OwnedSkills.FindRef(InSkillIDTag);
		}
	}

	return nullptr;
}

FTransform USkillComponent::GetSkillSpawnTransform(const FSkillInfo& InSkillData) const
{
	return FTransform();
}

void USkillComponent::NotifyBasicAttackHit(AActor* InTarget)
{
	if (!bInitialized)
	{
		InitComponent();
	}

	if (AActor* const Owner = GetOwner())
	{
		if (InTarget)
		{
			// 장비컴포->현재 무기 태그 무기 스왑 
			if (EquipmentComponent)
			{
				CurrentWeaponTag = EquipmentComponent->GetCurrentWeaponTag();
			}

			AddGaugeByBasicHit();

			OnBasicAttackHit.Broadcast(Owner, InTarget, CurrentWeaponTag);
		}
	}
}

FGameplayTag USkillComponent::GetBasicAttackSkillTag() const
{
	FGameplayTag weaponTag = CurrentWeaponTag;

	if (!weaponTag.IsValid())
	{
		if (EquipmentComponent)
		{
			weaponTag = EquipmentComponent->GetCurrentWeaponTag();
		}
	}

	if (const FWeaponSkillPair* pair = FindWeaponSkillPair(weaponTag))
	{
		return pair->BasicAttackSkillTag;
	}

	return FGameplayTag();
}

FGameplayTag USkillComponent::GetWeaponActiveSkillTag() const
{
	FGameplayTag weaponTag = CurrentWeaponTag;

	if (!weaponTag.IsValid())
	{
		if (EquipmentComponent)
		{
			weaponTag = EquipmentComponent->GetCurrentWeaponTag();
		}
	}

	if (const FWeaponSkillPair* pair = FindWeaponSkillPair(weaponTag))
	{
		return pair->WeaponSkillTag;
	}

	return FGameplayTag();
}

const FWeaponSkillPair* USkillComponent::FindWeaponSkillPair(const FGameplayTag& InWeaponTag) const
{
	if (InWeaponTag.IsValid())
	{
		return WeaponSkillMap.Find(InWeaponTag);
	}

	return nullptr;
}

void USkillComponent::HandleEquipChanged(EEquipSlot InSlot, FGameplayTag InNewEquipTag)
{
	if (InSlot == EEquipSlot::Weapon)
	{
		CurrentWeaponTag = InNewEquipTag;
	}
}

void USkillComponent::InitWeaponSkillMap()
{
	if (WeaponSkillMap.Num() > 0) { return; }

	if (WeaponSkillMappingTable)
	{
		TArray<FDTWeaponSkillMappingRow*> allRows;
		WeaponSkillMappingTable->GetAllRows<FDTWeaponSkillMappingRow>(TEXT("InitContext"), allRows);

		for (const FDTWeaponSkillMappingRow* row : allRows)
		{
			if (row && row->WeaponTag.IsValid())
			{
				WeaponSkillMap.Add(row->WeaponTag, row->SkillPair);
			}
		}
	}
}

void USkillComponent::AddGaugeByBasicHit()
{
	if (!bAwakenActive)
	{
		if (CurrentAwakenTag.IsValid())
		{
			if (USkillBase* const awakenSkill = GetSkill(CurrentAwakenTag))
			{
				const FSkillInfo& skillInfo = awakenSkill->GetSkillData();

				MaxGauge = skillInfo.Awaken.MaxGauge;

				CurrentGauge = FMath::Clamp(CurrentGauge + skillInfo.Awaken.GainOnBasicHit, 0.f, MaxGauge);

				UpdateGaugePercent();
			}
		}
	}
}

void USkillComponent::AddGaugeByWeaponSkillHit()
{
	if (!bAwakenActive)
	{
		if (CurrentAwakenTag.IsValid())
		{
			if (USkillBase* const awakenSkill = GetSkill(CurrentAwakenTag))
			{
				const FSkillInfo& skillInfo = awakenSkill->GetSkillData();

				MaxGauge = skillInfo.Awaken.MaxGauge;

				CurrentGauge = FMath::Clamp(
					CurrentGauge + skillInfo.Awaken.GainOnWeaponSkillHit, 0.f, MaxGauge);

				UpdateGaugePercent();
			}
		}
	}
}

void USkillComponent::UpdateGaugePercent()
{
	if (MaxGauge > 0.f)
	{
		GaugePercent = FMath::Clamp(CurrentGauge / MaxGauge, 0.f, 1.f);
	}
	else
	{
		GaugePercent = 0.f;
	}

	OnGaugePercentChanged.Broadcast(GaugePercent);
}

bool USkillComponent::ActivateAwaken()
{
	if (CurrentAwakenTag.IsValid())
	{
		if (!bAwakenActive)
		{
			if (USkillBase* const awakenSkill = GetSkill(CurrentAwakenTag))
			{
				const FSkillInfo& skillInfo = awakenSkill->GetSkillData();

				MaxGauge = skillInfo.Awaken.MaxGauge;

				if (CurrentGauge >= MaxGauge)
				{
					bAwakenActive = true;
					AwakenRemainTime = skillInfo.Awaken.DurationSec;

					SetAwakenAuraActive(true);

					if (UWorld* world = GetWorld())
					{
						world->GetTimerManager().SetTimer(
							AwakenUpdateTimerHandle,
							this,
							&USkillComponent::HandleAwakenUpdateTimer,
							AwakenUpdateInterval,
							true
						);
					}

					OnAwakenStateChanged.Broadcast(true, CurrentAwakenTag);

					return true;
				}
			}
		}
	}

	return false;
}

void USkillComponent::DeactivateAwaken()
{
	if (bAwakenActive)
	{
		if (UWorld* world = GetWorld())
		{
			world->GetTimerManager().ClearTimer(AwakenUpdateTimerHandle);
		}

		bAwakenActive = false;
		AwakenRemainTime = 0.f;
		CurrentGauge = 0.f;

		UpdateGaugePercent();

		SetAwakenAuraActive(false);

		OnAwakenStateChanged.Broadcast(false, CurrentAwakenTag);
	}
}

void USkillComponent::UpdateAwaken(float InDeltaTime)
{
	if (bAwakenActive)
	{
		if (USkillBase* const awakenSkill = GetSkill(CurrentAwakenTag))
		{
			const FSkillInfo& skillInfo = awakenSkill->GetSkillData();

			MaxGauge = skillInfo.Awaken.MaxGauge;

			if (skillInfo.Awaken.DurationSec > 0.f)
			{
				AwakenRemainTime = FMath::Max(0.f, AwakenRemainTime - InDeltaTime);

				CurrentGauge = FMath::Clamp((AwakenRemainTime / skillInfo.Awaken.DurationSec) * MaxGauge, 0.f, MaxGauge);

				UpdateGaugePercent();

				if (AwakenRemainTime <= 0.f)
				{
					DeactivateAwaken();
				}
			}
			else
			{
				DeactivateAwaken();
			}
		}
		else
		{
			DeactivateAwaken();
		}
	}
}

void USkillComponent::SetCurrentAwakenTag(const FGameplayTag& InAwakenTag)
{
	if (InAwakenTag.IsValid())
	{
		if (CurrentAwakenTag != InAwakenTag)
		{
			CurrentAwakenTag = InAwakenTag;

			AcquireSkill(CurrentAwakenTag);

			UpdateGaugePercent();
		}
	}
}

void USkillComponent::HandleAwakenUpdateTimer()
{
	UpdateAwaken(AwakenUpdateInterval);
}

void USkillComponent::SetAwakenAuraActive(bool bInActive)
{
	if (bAuraActive != bInActive)
	{
		bAuraActive = bInActive;

		OnAwakenAuraChanged.Broadcast(bAuraActive, CurrentAwakenTag);
	}
}

void USkillComponent::StartOrRefreshCooldown(const FGameplayTag& InSkillTag, float InTotalSeconds)
{
	if (InSkillTag.IsValid())
	{
		if (InTotalSeconds > 0.0f)
		{
			FCooldownData& data = CooldownByTag.FindOrAdd(InSkillTag);

			data.DurationSec = InTotalSeconds;     // 총 쿨다운 시간 갱신
			data.RemainingSec = InTotalSeconds;    // 남은 시간 리셋(리프레시)

			if (UWorld* world = GetWorld())
			{
				if (!world->GetTimerManager().IsTimerActive(CooldownTimerHandle))
				{
					world->GetTimerManager().SetTimer(
						CooldownTimerHandle,
						this,
						&USkillComponent::TickCooldownByTimer,
						CooldownTickSec,
						true
					);
				}
			}
		}
	}
}


void USkillComponent::TickCooldownByTimer()
{
	if (CooldownByTag.IsEmpty())
	{
		StopCooldownTimerIfIdle();
		return;
	}

	TArray<FGameplayTag> finishedTags;

	for (auto& pair : CooldownByTag)
	{
		const FGameplayTag& tag = pair.Key;
		FCooldownData& data = pair.Value;

		if (data.RemainingSec > 0.0f)
		{
			// 시간 차감
			data.RemainingSec = FMath::Max(0.0f, data.RemainingSec - CooldownTickSec);

			// percent
			float percent = (data.DurationSec > 0.0f) ? (data.RemainingSec / data.DurationSec) : 0.0f;

			OnSkillCooldownPercentChanged.Broadcast(tag, percent);
			OnSkillCooldownRemainingChanged.Broadcast(tag, data.RemainingSec);
		}

		// 0이 됐거나 이미 0이었던 것들 삭제 목록에 추가
		if (data.RemainingSec <= 0.0f)
		{
			finishedTags.Add(tag);
		}
	}

	// 일괄 삭제
	for (const FGameplayTag& tag : finishedTags)
	{
		CooldownByTag.Remove(tag);

		// OnSkillCooldownFinished.Broadcast(tag); 
	}

	StopCooldownTimerIfIdle();
}

void USkillComponent::StopCooldownTimerIfIdle()
{
	if (CooldownByTag.IsEmpty())
	{
		if (UWorld* world = GetWorld())
		{
			if (world->GetTimerManager().IsTimerActive(CooldownTimerHandle))
			{
				world->GetTimerManager().ClearTimer(CooldownTimerHandle);
			}
		}
	}
}

