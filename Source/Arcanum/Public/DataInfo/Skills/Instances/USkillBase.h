#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataInfo/Skills/Data/FSkillInfo.h"
#include "DataInfo/Skills/DataTable/DTSkillsData.h"
#include "USkillBase.generated.h"

/**
 * 스킬의 공통 로직을 담당하는 베이스 클래스 (UObject)
 */

class USkillComponent;

UCLASS(Abstract, Blueprintable, BlueprintType)
class ARCANUM_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	USkillBase();

	// --- 데이터 초기화 ---

	/** 마스터 데이터로부터 스킬 정보를 초기화합니다. */
	virtual void Initialize(AActor* InOwner, const FSkillInfo& InSkillData);

	/** 현재 레벨에 맞는 모디파이어 엔트리를 반환합니다. */
	const FLevelModifierEntry* GetCurrentLevelEntry() const;

	// --- 실행 로직 (인터페이스) ---

	/** 스킬 실행 (액티브는 발사, 패시브는 적용 등) */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void ActivateSkill(AActor* Instigator);

	/** 스킬 중지 또는 해제 */
	UFUNCTION(BlueprintCallable, Category = "Skill")
	virtual void DeactivateSkill(AActor* Instigator);

protected:
	/** 스킬의 정적 데이터 (DT에서 읽어온 값) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Data")
	FSkillInfo SkillData;

	/** 이 스킬을 소유하고 있는 액터 (시전자) */
	UPROPERTY(BlueprintReadOnly, Category = "Skill|State")
	TObjectPtr<AActor> OwnerActor;

#pragma region DataTable 관련
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UPROPERTY(EditDefaultsOnly, Category = "0_Base") FDataTableRowHandle DTSkillsDataRowHandle;
	UPROPERTY(VisibleAnywhere, Category = "1_RunTimeDebug") FDTSkillsDataRow	DTSkillsDataRow;
#pragma endregion

#pragma region 디버프/트리거 공통 처리
protected:
	/* 대상 필터 검사 (Enemy/Ally/Self) */
	bool CanAffectTarget(AActor* InInstigator, AActor* InTarget, const FGameplayTag& InTargetFilterTag) const;

	/* 대상에게 모디파이어 배열 적용 (데미지/디버프 공통) */
	bool ApplyModifiersToTarget(AActor* InInstigator, AActor* InTarget, const TArray<FDerivedStatModifier>& InModifiers);

	/* 단일 모디파이어 적용 (데미지/디버프 분기) */
	bool ApplySingleModifierToTarget(AActor* InInstigator, AActor* InTarget, const FDerivedStatModifier& InModifier);

	/* 무기스킬 적중 시 트리거 발동 체크 (디버프 최대스택 + 트리거스킬) */
	void TryExecuteTriggerSkill(AActor* InInstigator, AActor* InTarget);

	/* 체력 데미지용 태그 판별 */
	bool IsHealthValueTag(const FGameplayTag& InStatTag) const;
#pragma endregion

#pragma region 각성 디버프 결합
protected:
	/* 기본 모디파이어 + 현재 각성 디버프를 합침 */
	bool BuildHitModifiersWithAwaken(AActor* InInstigator, const TArray<FDerivedStatModifier>& InBaseModifiers, TArray<FDerivedStatModifier>& OutFinalModifiers) const;

	/* 현재 각성 태그 기준 디버프 모디파이어를 DT에서 꺼내서 추가 */
	void AddAwakenDebuffModifiers(AActor* InInstigator, TArray<FDerivedStatModifier>& InOutModifiers) const;
#pragma endregion

public:
	// Getters
	const FSkillInfo& GetSkillData() const { return SkillData; }
};