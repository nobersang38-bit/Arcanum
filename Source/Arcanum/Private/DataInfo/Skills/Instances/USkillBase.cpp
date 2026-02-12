#include "DataInfo/Skills/Instances/USkillBase.h"

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
