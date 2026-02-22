#include "DataInfo/Skills/Instances/USkillBase.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Component/SkillComponent.h"
#include "GameplayTags/ArcanumTags.h"

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
	if (!InInstigator || !InTarget) { return false; }

	// 자기 자신
	if (InTargetFilterTag == Arcanum::Skills::TargetFilter::Self)
	{
		if (InInstigator == InTarget)
		{
			return true;
		}

		return false;
	}

	// TODO : 팀 판정 로직 Enemy / Ally 분기
	// 테스트라 Self만 제외하고 통과
	if (InTargetFilterTag == Arcanum::Skills::TargetFilter::Enemy)
	{
		if (InInstigator != InTarget)
		{
			return true;
		}

		return false;
	}

	if (InTargetFilterTag == Arcanum::Skills::TargetFilter::Ally)
	{
		// 팀 판정 전이라 임시 false
		return false;
	}

	return true;
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
			// 적중 시 트리거 체크
			TryExecuteTriggerSkill(InInstigator, InTarget);

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

bool USkillBase::BuildHitModifiersWithAwaken(const TArray<FDerivedStatModifier>& InBaseModifiers, TArray<FDerivedStatModifier>& OutFinalModifiers) const
{
	OutFinalModifiers = InBaseModifiers;

	AddAwakenDebuffModifiers(OutFinalModifiers);

	return (OutFinalModifiers.Num() > 0);
}

void USkillBase::AddAwakenDebuffModifiers(TArray<FDerivedStatModifier>& InOutModifiers) const
{
	const FGameplayTag awakenTag = GetCurrentAwakenTagFromOwner();

	if (!awakenTag.IsValid())
	{
		return;
	}
}

FGameplayTag USkillBase::GetCurrentAwakenTagFromOwner() const
{
	if (OwnerActor)
	{
		USkillComponent* skillComponent = OwnerActor->FindComponentByClass<USkillComponent>();

		if (skillComponent)
		{
			return skillComponent->GetCurrentAwakenTag();
		}
	}

	return FGameplayTag();
}
