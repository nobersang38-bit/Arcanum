#include "DataInfo/Skills/Instances/USkillBase.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Character/BaseUnitCharacter.h"
#include "Component/SkillComponent.h"
#include "GameplayTags/ArcanumTags.h"
#include "Interface/TeamInterface.h"
#include "Kismet/KismetSystemLibrary.h"


USkillBase::USkillBase()
{
	// 생성자 로직
}

void USkillBase::Initialize(AActor* InOwner, const FSkillInfo& InSkillData)
{
	OwnerActor = InOwner;
	SkillData = InSkillData;
}

const FLevelModifierEntry* USkillBase::GetCurrentLevelEntry() const
{
	int32 TargetIndex = SkillData.CurrLevel;
	if (SkillData.LevelModifiers.IsValidIndex(TargetIndex)) return &SkillData.LevelModifiers[TargetIndex];
	else return nullptr;
}

void USkillBase::ActivateSkill(AActor* Instigator)
{
	if (!OwnerActor) return;

	const FLevelModifierEntry* Entry = GetCurrentLevelEntry();
	if (!Entry) return;
	// 상속받은 Active/Passive에서 구체적으로 구현
}

void USkillBase::DeactivateSkill(AActor* Instigator)
{
	// 상속받은 클래스에서 정리 로직 구현
}

void USkillBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(USkillBase, DTSkillsDataRowHandle)) {
		if (DTSkillsDataRowHandle.DataTable && !DTSkillsDataRowHandle.RowName.IsNone()) {
			const FDTSkillsDataRow* Row = DTSkillsDataRowHandle.DataTable->FindRow<FDTSkillsDataRow>(DTSkillsDataRowHandle.RowName, TEXT("Editor StatsRegen Load"));

			if (Row) {
				DTSkillsDataRow = *Row;
				SkillData = Row->SkillData;
			}
		}
	}
}

bool USkillBase::CanAffectTarget(AActor* InInstigator, AActor* InTarget, const FGameplayTag& InTargetFilterTag) const
{
	if (!InInstigator || !InTarget)	{ return false; }

	// 인터페이스 체크
	const bool bHasInstigatorInterface = InInstigator->GetClass()->ImplementsInterface(UTeamInterface::StaticClass());
	const bool bHasTargetInterface = InTarget->GetClass()->ImplementsInterface(UTeamInterface::StaticClass());

	if (!bHasInstigatorInterface || !bHasTargetInterface) {	return false; }

	ITeamInterface* instigatorTeamInterface = Cast<ITeamInterface>(InInstigator);
	ITeamInterface* targetTeamInterface = Cast<ITeamInterface>(InTarget);

	if (!instigatorTeamInterface || !targetTeamInterface) {	return false; }

	const FGameplayTag instigatorTeam = instigatorTeamInterface->GetTeamTag();
	const FGameplayTag targetTeam = targetTeamInterface->GetTeamTag();

	// 팀 태그가 비어있으면 판정 불가
	if (!instigatorTeam.IsValid() || !targetTeam.IsValid()) { return false; }

	const bool bIsSameActor = (InInstigator == InTarget);
	const bool bIsSameTeam = (instigatorTeam == targetTeam);

	// 필더 로직
	using namespace Arcanum::Skills::TargetFilter;
	if (InTargetFilterTag == Self)
	{
		return bIsSameActor;
	}

	if (InTargetFilterTag == Enemy)
	{
		return !bIsSameActor && !bIsSameTeam;
	}

	if (InTargetFilterTag == Ally)
	{
		// Self는 위에서 별도 처리하므로 여기서는 자기 자신 제외
		return !bIsSameActor && bIsSameTeam;
	}

	return false;
}

bool USkillBase::ApplyModifiersToTarget(AActor* InInstigator, AActor* InTarget, const TArray<FDerivedStatModifier>& InModifiers)
{
	if (!InInstigator || !InTarget || InModifiers.IsEmpty()) return false;

	bool bHasSuccess = false;

	for (const FDerivedStatModifier& modifier : InModifiers)
	{
		if (ApplySingleModifierToTarget(InInstigator, InTarget, modifier))
		{
			bHasSuccess = true;

			UE_LOG(LogTemp, Log, TEXT("[Applied] %s"), *modifier.StatTag.ToString());
		}
	}

	return bHasSuccess;
}

bool USkillBase::ApplySingleModifierToTarget(AActor* InInstigator, AActor* InTarget, const FDerivedStatModifier& InModifier)
{
	if (!InInstigator || !InTarget) { return false; }

	if (CanAffectTarget(InInstigator, InTarget, InModifier.ApplyTargetFilterTag))
	{
		if (UCharacterBattleStatsComponent* targetStats = InTarget->FindComponentByClass<UCharacterBattleStatsComponent>())
		{
			// 데미지/회복 처리
			if (IsHealthValueTag(InModifier.StatTag))
			{
				targetStats->ChangeStatValue(Arcanum::BattleStat::Character::Regen::Health::Root, InModifier.Value.Flat, InInstigator);
			}
			else
			{
				// 지속 효과(버프/디버프)
				targetStats->ApplyDurationModifier(InModifier);
			}

			return true;
		}
	}

	return false;
}

void USkillBase::TryExecuteTriggerSkill(AActor* InInstigator, AActor* InTarget)
{
	// TODO :
	// 1) 현재 스킬이 "무기스킬"인지 확인
	// 2) 대상에게 걸린 디버프 스택/최대스택 확인
	// 3) 현재 각성 스킬의 TriggerSkillTag 확인
	// 4) TriggerSkillTag 스킬 실행
	// 5) 디버프 스택 초기화
}

bool USkillBase::IsHealthValueTag(const FGameplayTag& InStatTag) const
{
	if (InStatTag == Arcanum::BattleStat::Character::Regen::Health::Value)
	{
		return true;
	}

	return false;
}

bool USkillBase::BuildHitModifiersWithAwaken(AActor* InInstigator, const TArray<FDerivedStatModifier>& InBaseModifiers, TArray<FDerivedStatModifier>& OutFinalModifiers) const
{
	OutFinalModifiers = InBaseModifiers;

	AddAwakenDebuffModifiers(InInstigator, OutFinalModifiers);

	return (OutFinalModifiers.Num() > 0);
}

void USkillBase::AddAwakenDebuffModifiers(AActor* InInstigator, TArray<FDerivedStatModifier>& InOutModifiers) const
{
	if (InInstigator)
	{
		if (USkillComponent* skillComponent = InInstigator->FindComponentByClass<USkillComponent>())
		{
			FGameplayTag awakenTag = skillComponent->GetCurrentAwakenTag();
			if (awakenTag.IsValid())
			{
				// 현재 선택된 각성 가져오기
				USkillBase* awakenSkill = skillComponent->GetSkill(awakenTag);

				if (!awakenSkill)
				{
					// 아직 안 들고 있으면 획득 시도
					if (skillComponent->AcquireSkill(awakenTag))
					{
						awakenSkill = skillComponent->GetSkill(awakenTag);
					}
				}

				if (awakenSkill)
				{
					const FLevelModifierEntry* awakenEntry = awakenSkill->GetCurrentLevelEntry();

					if (awakenEntry)
					{
						// 각성 스킬 DT의 OtherCharacterModifiers를 기본공격 시 묻히는 디버프로 사용
						for (const FDerivedStatModifier& modifier : awakenEntry->OtherCharacterModifiers)
						{
							InOutModifiers.Add(modifier);
						}
					}
				}
			}
		}
	}
}

void USkillBase::CollectTargetsInRange(AActor* InInstigator, float InRange, TArray<AActor*>& OutTargets) const
{
	OutTargets.Reset();

	if (InInstigator)
	{
		if (UWorld* world = InInstigator->GetWorld())
		{
			TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
			objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

			TArray<AActor*> actorsToIgnore;
			actorsToIgnore.Add(InInstigator);

			TArray<AActor*> overlappedActors;

			const bool bHit = UKismetSystemLibrary::SphereOverlapActors(
				world,
				InInstigator->GetActorLocation(),
				InRange,
				objectTypes,
				ABaseUnitCharacter::StaticClass(),
				actorsToIgnore,
				overlappedActors
			);

			if (bHit)
			{
				for (AActor* actor : overlappedActors)
				{
					if (actor)
					{
						// 적만 후보로 넣음 (Self/Ally 제외)
						if (CanAffectTarget(InInstigator, actor, Arcanum::Skills::TargetFilter::Enemy))
						{
							// 죽은 대상 제외
							if (IsAliveTarget(actor))
							{
								OutTargets.Add(actor);
							}
						}
					}
				}
			}
		}
	}
}
