#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DataInfo/Skills/Data/FSkillInfo.h"
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
enum class EEquipSlot : uint8;

USTRUCT(BlueprintType)
struct FCooldownData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DurationSec = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float RemainingSec = 0.f;
};

#pragma region 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBasicAttackHit, AActor*, InInstigator, AActor*, InTarget,	FGameplayTag, InWeaponTag);
/* 게이지 UI 갱신 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGaugePercentChanged, float, InPercent);
/* 각성 상태 변경 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAwakenStateChanged, bool, bInAwakenActive, FGameplayTag, InAwakenTag);
/* 오라 상태 변경 알림 (VFX/오라판정 on/off 연결용) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAwakenAuraChanged, bool, bInActive, FGameplayTag, InAwakenTag);
/* 스킬 쿨다운 퍼센트 변경 알림 (0~1) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillCooldownPercentChanged, FGameplayTag, InSkillTag, float, InPercent);
/* 스킬 쿨다운 남은시간 변경 알림 (초) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillCooldownRemainingChanged, FGameplayTag, InSkillTag, float, InRemainingSec);
/* 스킬 사용 실패 (UI 메시지용) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillFailed, FGameplayTag, InSkillTag);

#pragma endregion

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARCANUM_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();
protected:
	virtual void BeginPlay() override;

#pragma region 초기화
public:
	void InitComponent();

private:
	UPROPERTY(VisibleAnywhere, Transient)
	bool bInitialized = false;
#pragma endregion

#pragma region 데이터
private:
	// 스킬 DT를 훑어서 SkillTag -> RowName 캐시 생성
	void BuildSkillRowNameCache();

private:
	/** 스킬 마스터 데이터 테이블 */
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Data")
	TObjectPtr<UDataTable> SkillsDataTable;

	/* 무기 태그와 스킬 쌍을 매핑해둔 테이블 */
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Data")
	TObjectPtr<UDataTable> WeaponSkillMappingTable;

	/** 현재 보유 중인 스킬 인스턴스들 */
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USkillBase>> OwnedSkills;

	/* 스킬 태그->DT RowName 캐시(FindRow 안정화용) */
	UPROPERTY()
	TMap<FGameplayTag, FName> SkillRowNameByTag;

	/* 캐시 빌드 여부 */
	UPROPERTY(VisibleAnywhere, Transient)
	bool bSkillRowNameCacheBuilt = false;
#pragma endregion

#pragma region Skill Life-cycle
public:
	 /** 스킬 획득 */
	bool AcquireSkill(const FGameplayTag& InSkillIDTag);


	/** 스킬 제거 */
	bool RemoveSkill(const FGameplayTag& InSkillIDTag);

	/** 보유 중인 스킬 조회 */
	USkillBase* GetSkill(const FGameplayTag& InSkillIDTag) const;
#pragma endregion

#pragma region Active Skill
	 /** 액티브 스킬 실행 */
	bool ActivateSkill(const FGameplayTag& InSkillIDTag);
#pragma endregion

#pragma region Spawn Helper
	 /**
	  * 스킬 효과(Projectile, AreaActor 등)의
	  * 스폰 기준 Transform을 반환
	  *
	  * - 소켓 결정은 SkillComponent 책임
	  * - Skill은 이걸 호출만 함
	  */
	FTransform GetSkillSpawnTransform(const FSkillInfo& InSkillData) const;
#pragma endregion

#pragma region 무기 동작
public:
	/* 무기 Hit */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void NotifyBasicAttackHit(AActor* InTarget);

	/* 현재 무기에 맞는 평타 스킬 태그 반환 */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	FGameplayTag GetBasicAttackSkillTag() const;

	/* 현재 무기에 맞는 무기 스킬 태그 반환 */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	FGameplayTag GetWeaponActiveSkillTag() const;

private:
	/* 무기 태그 -> 스킬쌍 조회 */
	const FWeaponSkillPair* FindWeaponSkillPair(const FGameplayTag& InWeaponTag) const;

	/* 무기 변경 */
	UFUNCTION()
	void HandleEquipChanged(EEquipSlot InSlot, FGameplayTag InNewEquipTag);
		
	/* 무기별 스킬 매핑 초기화 */
	void InitWeaponSkillMap();
public:
	UPROPERTY(BlueprintAssignable, Category = "Weapon")
	FOnBasicAttackHit OnBasicAttackHit;

private:
	/* 무기 스왑 시 현재 무기 태그 캐시 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FGameplayTag CurrentWeaponTag;

	/* 무기 태그 -> (평타/무기스킬) 매핑 캐시 */
	UPROPERTY()
	TMap<FGameplayTag, FWeaponSkillPair> WeaponSkillMap;

	/* 장비 컴포넌트 캐시 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipmentComponent> EquipmentComponent = nullptr;
#pragma endregion

#pragma region 게이지
public:
	/* 게이지 증가: 기본공격 적중 */
	UFUNCTION(BlueprintCallable, Category = "Gauge")
	void AddGaugeByBasicHit();

	/* 게이지 증가: 무기스킬 적중 */
	UFUNCTION(BlueprintCallable, Category = "Gauge")
	void AddGaugeByWeaponSkillHit();

	/* 게이지 비율 계산 + 브로드캐스트 */
	UFUNCTION(BlueprintCallable, Category = "Gauge")
	void UpdateGaugePercent();

	/* 현재 게이지 비율(0~1) */
	UFUNCTION(BlueprintPure, Category = "Gauge")
	float GetGaugePercent() const {	return GaugePercent; }

	/* 가득 찼는지 */
	UFUNCTION(BlueprintPure, Category = "Gauge")
	bool IsGaugeFull() const { return CurrentGauge >= MaxGauge; }

public:
	UPROPERTY(BlueprintAssignable, Category = "Gauge")
	FOnGaugePercentChanged OnGaugePercentChanged;

private:
	/* 현재 게이지 값 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gauge", meta = (AllowPrivateAccess = "true"))
	float CurrentGauge = 0.f;

	/* 최대 게이지 값 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gauge", meta = (AllowPrivateAccess = "true"))
	float MaxGauge = 100.f;

	/* UI 머티리얼 Percentage에 넣을 0~1 값 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gauge", meta = (AllowPrivateAccess = "true"))
	float GaugePercent = 0.f;
#pragma endregion

#pragma region 각성
public:
	/* 각성 발동 버튼 (게이지 가득 찼을 때만 성공) */
	UFUNCTION(BlueprintCallable, Category = "Awaken")
	bool ActivateAwaken();

	/* 각성 종료 (게이지 소진/강제 종료 공통) */
	UFUNCTION(BlueprintCallable, Category = "Awaken")
	void DeactivateAwaken();

	/* 각성 중 갱신 (지속시간 동안 게이지 감소 처리) */
	UFUNCTION(BlueprintCallable, Category = "Awaken")
	void UpdateAwaken(float InDeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Awaken")
	void SetCurrentAwakenTag(const FGameplayTag& InAwakenTag);

	/* 현재 각성 중인지 */
	UFUNCTION(BlueprintPure, Category = "Awaken")
	bool IsAwakenActive() const { return bAwakenActive; }

	/* 현재 각성 태그 */
	UFUNCTION(BlueprintPure, Category = "Awaken")
	FGameplayTag GetCurrentAwakenTag() const { return CurrentAwakenTag; }

private:
	/* 타이머 콜백 */
	UFUNCTION()
	void HandleAwakenUpdateTimer();

public:
	/* 각성 시작/종료 브로드캐스트 */
	UPROPERTY(BlueprintAssignable, Category = "Awaken")
	FOnAwakenStateChanged OnAwakenStateChanged;

private:
	/* 현재 선택/적용된 각성 종류 (분노/집중/신성) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Awaken", meta = (AllowPrivateAccess = "true"))
	FGameplayTag CurrentAwakenTag;

	/* 각성 활성 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Awaken", meta = (AllowPrivateAccess = "true"))
	bool bAwakenActive = false;

	/* 각성 남은 시간(초) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Awaken", meta = (AllowPrivateAccess = "true"))
	float AwakenRemainTime = 0.f;

	/* 타이머 호출 주기 */
	UPROPERTY(EditDefaultsOnly, Category = "Awaken")
	float AwakenUpdateInterval = 0.05f;

	/* 각성 게이지 감소 타이머 */
	FTimerHandle AwakenUpdateTimerHandle;
#pragma endregion

#pragma region 오라
public:
	/* 오라 활성/비활성 (각성 시작/종료에서 호출) */
	UFUNCTION(BlueprintCallable, Category = "Awaken|Aura")
	void SetAwakenAuraActive(bool bInActive);

	/* 현재 오라 활성 여부 */
	UFUNCTION(BlueprintPure, Category = "Awaken|Aura")
	bool IsAwakenAuraActive() const { return bAuraActive; }

public:
	/* 오라 브로드캐스트 */
	UPROPERTY(BlueprintAssignable, Category = "Awaken|Aura")
	FOnAwakenAuraChanged OnAwakenAuraChanged;

private:
	/* 오라 활성 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Awaken|Aura", meta = (AllowPrivateAccess = "true"))
	bool bAuraActive = false;
#pragma endregion

#pragma region 쿨다운
private:
	/* 스킬 태그로 쿨다운 등록/리프레시 */
	void StartOrRefreshCooldown(const FGameplayTag& InSkillTag, float InTotalSeconds);

	/* 타이머 틱마다 모든 쿨다운 감소 */
	void TickCooldownByTimer();

	/* 남은 쿨다운 없으면 타이머 정지 */
	void StopCooldownTimerIfIdle();

public:
	UPROPERTY(BlueprintAssignable, Category = "Skill|Cooldown")
	FOnSkillCooldownPercentChanged OnSkillCooldownPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "Skill|Cooldown")
	FOnSkillCooldownRemainingChanged OnSkillCooldownRemainingChanged;

	UPROPERTY(BlueprintAssignable, Category = "Skill|Event")
	FOnSkillFailed OnSkillFailed;

private:
	/* 스킬별 쿨다운 상태 */
	UPROPERTY()
	TMap<FGameplayTag, FCooldownData> CooldownByTag;

	/* 쿨다운 갱신 타이머 */
	FTimerHandle CooldownTimerHandle;

	/* 쿨다운 타이머 틱 간격 */
	UPROPERTY(EditDefaultsOnly, Category = "Skill|Cooldown")
	float CooldownTickSec = 0.05f;
#pragma endregion
};
