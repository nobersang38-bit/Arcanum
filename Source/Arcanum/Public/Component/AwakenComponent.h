
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Components/ActorComponent.h"
//#include "Engine/DataTable.h"
//#include "GameplayTagContainer.h"
//#include "Data/Skill/AwakenRow.h"
//#include "Data/Skill/TriggerEffectRow.h"
//#include "Data/Skill/EffectRow.h"
//#include "Data/Skill/SkillContext.h"
//#include "AwakenComponent.generated.h"
//
///*
// * 캐릭터 
// * 각성(패시브) 컴포넌트
// * - 패시브: 게이지/스택 디버프/트리거(혈폭/응보/처단/정화)
// * - 스택 디버프: 맞으면 +1(MaxStack), Duration=8초 Refresh, 만료 시 제거
// * - CC(스턴/넉백): Duration=3초, Refresh 안 함(이미 있으면 무시)
// * - 트리거 발동 시: 조건 디버프 스택 즉시 0(초기화)
// * - 각성 전: 플레이어만 패시브 발동(아군 제외)
// * - 각성 후: 캐릭터 중심 지속 오라(아군도 디버프/오라 영향)
// * 데이터
// * - FAwakenRow        : 패시브 4종(분노/결의/집중/신성) 설계 묶음(게이지/스택디버프/효과/트리거 참조)
// * - FEffectRow        : 실제 효과(수치/대상/시간/스택/틱)
// * - FTriggerEffectRow : 조건 디버프 최대 스택이면 발동 + 실행할 EffectIDs
// * 
// * 추후에 수정 몇개 뺄수도 있음
// */
//
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAwakenPassiveChanged, FGameplayTag, InAwakenTag, const FAwakenRow*, InRow);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAwakenGaugeChanged, FGameplayTag, InAwakenTag, float, InValue, float, InMax);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAwakenedChanged, bool, bInAwakened);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAwakenReadyChanged, bool, bInReady);
//
//USTRUCT(BlueprintType)
//struct FAwakenRuntimeState
//{
//	GENERATED_BODY()
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	FGameplayTag AwakenTag;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	bool bAwakened = false;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	float GaugeValue = 0.0f;
//
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//	float GaugeMax = 100.0f;
//};
//
//UCLASS(ClassGroup = (Arcanum), meta = (BlueprintSpawnableComponent))
//class ARCANUM_API UAwakenComponent : public UActorComponent
//{
//	GENERATED_BODY()
//
//public:
//	UAwakenComponent();
//
//	/* ===============================
//	 * Init / Data
//	 * =============================== */
//
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Awaken")
//	void SetDataTables(UDataTable* InAwakenTable, UDataTable* InEffectTable, UDataTable* InTriggerTable);
//
//	/* 현재 패시브 선택(분노/결의/집중/신성) : FAwakenRow.AwakenTag 기준 */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Awaken")
//	bool SetCurrentAwakenTag(const FGameplayTag& InAwakenTag);
//
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken")
//	const FAwakenRow* GetCurrentAwakenRow() const { return CurrentRow; }
//
//	/* ===============================
//	 * UI 연결 (각성 버튼/게이지)
//	 * =============================== */
//
//	 /* 각성 버튼 눌렀을 때 */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Awaken|UI")
//	bool RequestAwaken();
//
//	/* 각성 해제(테스트/디버그/특수 조건용) */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Awaken|UI")
//	void ForceSetAwakened(bool bInAwakened);
//
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken|UI")
//	bool IsAwakened() const { return Runtime.bAwakened; }
//
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken|UI")
//	float GetGaugeValue() const { return Runtime.GaugeValue; }
//
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken|UI")
//	float GetGaugeMax() const { return Runtime.GaugeMax; }
//
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken|UI")
//	float GetGaugePercent() const	{
//		if (Runtime.GaugeMax <= 0.0f)
//		{
//			return 0.0f;
//		}
//		return Runtime.GaugeValue / Runtime.GaugeMax;
//	}
//
//	/* 게이지가 Max라서 각성 버튼 활성 가능한지 */
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken|UI")
//	bool IsAwakenReady() const;
//
//	/* ===============================
//	 * 스킬/전투 호출
//	 * =============================== */
//
//	 /* 적을 맞췄을 때(플레이어/아군 구분 포함) */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Awaken|Runtime")
//	void NotifyHit(const FSkillContext& InContext, AActor* InVictim, bool bInInstigatorIsPlayer, bool bInVictimIsAlly);
//
//	/* 아군이 맞았을 때(결의: 가시 부여 같은 케이스용) */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Awaken|Runtime")
//	void NotifyAllyDamaged(AActor* InAlly, AActor* InAttacker, bool bInAllyIsPlayer);
//
//	/* 무기 스킬을 사용했을 때(트리거 조건 체크용: 혈폭/응보/처단/정화) */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Awaken|Runtime")
//	void NotifyWeaponSkillUsed(const FSkillContext& InContext);
//
//	/* ===============================
//	 * Getters
//	 * =============================== */
//
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken")
//	const FAwakenRuntimeState& GetRuntimeState() const { return Runtime; }
//
//	/* 현재 패시브가 사용하는 스택 디버프 태그 */
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken")
//	FGameplayTag GetCurrentStackDebuffTag() const;
//
//	/* 각성 전 패시브 효과들 */
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken")
//	const TArray<FName>& GetPassiveEffectIDs() const;
//
//	/* 각성 후 오라 효과들 */
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken")
//	const TArray<FName>& GetAuraEffectIDs() const;
//
//	/* 트리거들 */
//	UFUNCTION(BlueprintPure, Category = "Arcanum|Awaken")
//	const TArray<FName>& GetTriggerIDs() const;
//
//protected:
//	virtual void BeginPlay() override;
//
//private:
//	/* Row 찾기: AwakenTag -> FAwakenRow */
//	const FAwakenRow* FindAwakenRow(const FGameplayTag& InAwakenTag) const;
//
//	/* 게이지 */
//	void AddGaugeInternal(float InDelta);
//	void ResetGaugeInternal();
//	void BroadcastGauge() const;
//	void BroadcastReadyIfChanged();
//
//	/* 트리거 체크(조건 디버프 최대 스택이면 발동) */
//	void TryTriggerByWeaponSkill(const FSkillContext& InContext);
//
//	/* 트리거 발동 후 조건 디버프 스택 초기화(즉시 0) */
//	void ResetConditionDebuffStacks(const FGameplayTag& InDebuffTag);
//
//private:
//	UPROPERTY(EditAnywhere, Category = "Arcanum|Awaken", meta = (AllowPrivateAccess = "true"))
//	TObjectPtr<UDataTable> AwakenTable;
//
//	UPROPERTY(EditAnywhere, Category = "Arcanum|Awaken", meta = (AllowPrivateAccess = "true"))
//	TObjectPtr<UDataTable> EffectTable;
//
//	UPROPERTY(EditAnywhere, Category = "Arcanum|Awaken", meta = (AllowPrivateAccess = "true"))
//	TObjectPtr<UDataTable> TriggerTable;
//
//	const FAwakenRow* CurrentRow = nullptr;
//
//	UPROPERTY(VisibleAnywhere, Category = "Arcanum|Awaken", meta = (AllowPrivateAccess = "true"))
//	FAwakenRuntimeState Runtime;
//
//	bool bLastReadyState = false;
//
//public:
//	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Awaken")
//	FOnAwakenPassiveChanged OnAwakenPassiveChanged;
//
//	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Awaken")
//	FOnAwakenGaugeChanged OnAwakenGaugeChanged;
//
//	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Awaken")
//	FOnAwakenedChanged OnAwakenedChanged;
//
//	/* 각성 버튼 Enable/Disable 연결용 */
//	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Awaken|UI")
//	FOnAwakenReadyChanged OnAwakenReadyChanged;
//};

