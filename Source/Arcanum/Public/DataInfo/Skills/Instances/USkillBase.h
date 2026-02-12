#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataInfo/Skills/Data/FSkillInfo.h"
#include "DataInfo/Skills/DataTable/DTSkillsData.h"
#include "USkillBase.generated.h"

/**
 * 스킬의 공통 로직을 담당하는 베이스 클래스 (UObject)
 */
UCLASS(Blueprintable, BlueprintType)
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

public:
	// Getters
	const FSkillInfo& GetSkillData() const { return SkillData; }
};