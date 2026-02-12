#include "Component/Entity/SkillContainerComponent.h"
#include "DataInfo/Skills/Instances/USkillBase.h"

USkillContainerComponent::USkillContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void USkillContainerComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void USkillContainerComponent::AddSkill(FGameplayTag SkillTag)
{
    if (SkillInstances.Contains(SkillTag)) return;

    static const FString ContextString(TEXT("Skill Mapping Context"));
    FDTSkillMappingRow* MappingRow = SkillMappingTable->FindRow<FDTSkillMappingRow>(SkillTag.GetTagName(), ContextString);

    if (!MappingRow || MappingRow->SkillClass.IsNull()) return;
    UClass* SkillClass = MappingRow->SkillClass.LoadSynchronous();

    if (!SkillClass) return;
    USkillBase* NewSkill = NewObject<USkillBase>(GetOwner(), SkillClass);
    if (FSkillInfo* Info = FindSkillInfo(SkillTag)) {
        NewSkill->Initialize(GetOwner(), *Info);
        SkillInstances.Add(SkillTag, NewSkill);
    }
}

void USkillContainerComponent::ExecuteSkill(FGameplayTag SkillTag)
{
    if (SkillInstances.Contains(SkillTag)) SkillInstances[SkillTag]->ActivateSkill(GetOwner());
}

void USkillContainerComponent::RemoveSkill(FGameplayTag SkillTag)
{
    if (SkillInstances.Contains(SkillTag)) {
        SkillInstances[SkillTag]->DeactivateSkill(GetOwner());
        SkillInstances.Remove(SkillTag);
    }
}

FSkillInfo* USkillContainerComponent::FindSkillInfo(FGameplayTag SkillTag)
{
    if (!SkillInfoTable) return nullptr;

    static const FString ContextString(TEXT("FindSkillInfo"));
    TArray<FDTSkillsDataRow*> Rows;
    SkillInfoTable->GetAllRows(ContextString, Rows);

    for (FDTSkillsDataRow* Row : Rows) {
        if (!Row) continue;
        if (Row->SkillData.SkillNameTag == SkillTag) return &Row->SkillData;
    }

    return nullptr;
}