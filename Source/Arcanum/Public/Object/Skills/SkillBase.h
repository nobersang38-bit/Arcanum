#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataInfo/SkillData/Data/FSkillInfo.h"
#include "DataInfo/SkillData/DataTable/DTSkillsData.h"
#include "DataInfo/BattleCharacter/Equipment/Data/FEquipmentData.h"
#include "NativeGameplayTags.h"
#include "SkillBase.generated.h"

/**
 * 스킬의 공통 로직을 담당하는 베이스 클래스 (UObject)
 */
UCLASS(Blueprintable)
class USkillBase : public UObject
{
    GENERATED_BODY()

public:
    USkillBase();

    virtual void Initialize(
        AActor* InOwner, 
        const FSkillInfo* InSkillInfo,
        int32 InLevel, 
        FGameplayTag InTargetFilterTag, 
        const FItemDefinition* InEquipmentData);

    virtual void ActivateSkill(AActor* Instigator);
    virtual void DeactivateSkill(AActor* Instigator);

    const FLevelModifierEntry* GetCurrentLevelEntry() const;
    const FSkillInfo* GetSkillInfo() const { return SkillInfo; }
    const FItemDefinition* GetEquipmentData() const { return EquipmentData; }

protected:

    TWeakObjectPtr<AActor> OwnerActor;

    const FSkillInfo* SkillInfo;
    const FItemDefinition* EquipmentData = nullptr;

    FSkillState SkillState;

    FGameplayTag TargetFilterTag;
};