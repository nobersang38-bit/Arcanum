#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "SkillComponent.generated.h"

/*
 * 추영호
 * 스킬의 획득 / 제거 / 실행을 총괄하는 컴포넌트
 * - 스킬 정의는 DT
 * - 스킬 로직은 UObject (USkillBase)
 * - 월드 개입은 여기서 중계
 */

class USkillBase;
class UEquipmentComponent;
struct FSkillInfo;

#pragma region 델리게이트
/* 스킬 쿨타운 변경 (UI 쿨다운 표시) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillCooldownChanged, FGameplayTag, InSkillTag, float, InPercent);
/* 마나 부족으로 스킬 사용 실패 (UI 메시지용) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCostFailed, FGameplayTag, InSkillTag);
/* 쿨다운 중으로 스킬 사용 실패 (UI 메시지용) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCooldownFailed, FGameplayTag, InSkillTag);
/* 각성 버튼 활성/비활성 (게이지 Max 도달시 true) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAwakenReadyChanged, bool, bReady);
#pragma endregion

UCLASS(ClassGroup = (Arcanum), meta = (BlueprintSpawnableComponent))
class ARCANUM_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

protected:
	virtual void BeginPlay() override;

#pragma region 초기화 관련
public:
	void InitComponent();

private:
	UPROPERTY(VisibleAnywhere, Transient)
	bool bInitialized = false;
#pragma endregion

	/* ===============================
	 * Data
	 * =============================== */

	 /* 스킬 마스터 데이터 테이블 */
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Data")
	TObjectPtr<UDataTable> SkillsDataTable;

	/* DT 기반 스킬 실행용 기본 클래스  */
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Data")
	TSubclassOf<USkillBase> DefaultSkillClass;

	/* 장비 변경(스왑/전설) 따라 현재 무기/스킬 태그 갱신 */
	UPROPERTY(VisibleAnywhere, Transient, Category = "Skill|Ref")
	TObjectPtr<UEquipmentComponent> EquipmentComponent;

	/* 현재 장착 무기 기반으로 계산된 '기본공격/무기스킬/전설스킬' 태그 캐시(UI 버튼은 이걸로 실행) */
	UPROPERTY(VisibleAnywhere, Transient, Category = "Skill|Runtime")
	FGameplayTag CurrentBasicAttackTag;
	UPROPERTY(VisibleAnywhere, Transient, Category = "Skill|Runtime")
	FGameplayTag CurrentWeaponSkillTag;
	UPROPERTY(VisibleAnywhere, Transient, Category = "Skill|Runtime")
	FGameplayTag CurrentLegendarySkillTag;

	/* 현재 보유 중인 스킬 인스턴스들 */
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USkillBase>> OwnedSkills;

public:
	/* ===============================
	 * Skill Life-cycle
	 * =============================== */

	 /** 스킬 획득 */
	bool AcquireSkill(const FGameplayTag& SkillIDTag);

	/** 스킬 제거 */
	bool RemoveSkill(const FGameplayTag& SkillIDTag);

	/** 보유 중인 스킬 조회 */
	USkillBase* GetSkill(const FGameplayTag& SkillIDTag) const;

	/* ===============================
	 * Active Skill
	 * =============================== */

	 /** 액티브 스킬 실행 */
	bool ActivateSkill(const FGameplayTag& SkillIDTag);

	/* ===============================
	 * Spawn Helper (중요)
	 * =============================== */

	 /**
	  * 스킬 효과(Projectile, AreaActor 등)의
	  * 스폰 기준 Transform을 반환
	  *
	  * - 소켓 결정은 SkillComponent 책임
	  * - Skill은 이걸 호출만 함
	  */
	FTransform GetSkillSpawnTransform(const FSkillInfo& SkillData) const;

private:
	/* SkillNameTag -> DataTable RowName 변환 */
	// static FName MakeRowNameFromSkillTag(const FGameplayTag& SkillIDTag);

	/* BeginPlay에서 장비컴포 바인딩 + 현재 태그 초기화 */
	void BindEquipmentEvents();  

	/* 무기 태그 바뀔 때마다 Current*Tag 재계산 */
	void RefreshCurrentSkillTags();

public:
	UPROPERTY(BlueprintAssignable, Category = "Skill|Event")
	FOnSkillCooldownChanged OnSkillCooldownChanged;

	UPROPERTY(BlueprintAssignable, Category = "Skill|Event")
	FOnSkillCostFailed OnSkillCostFailed;

	UPROPERTY(BlueprintAssignable, Category = "Skill|Event")
	FOnSkillCooldownFailed OnSkillCooldownFailed;

	UPROPERTY(BlueprintAssignable, Category = "Skill|Event")
	FOnAwakenReadyChanged OnAwakenReadyChanged;
};

