#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags/ArcanumTags.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "DataInfo/CommonData/Stats/FStatModifier.h"
#include "Data/Rows/DTBattleStatsRow.h"
#include "DataInfo/BattleCharacter/BattleStats/Data/FGradeStatData.h"
#include "CharacterBattleStatsComponent.generated.h"

// 1. Regen 계열용 (HP, MP등 정보)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterRegenStatChanged, const FRegenStat&);
// 2. NonRegen 계열용 (이동속도 같은것들)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterNonRegenStatChanged, const FNonRegenStat&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UCharacterBattleStatsComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region 델리게이트
public:
	FOnCharacterRegenStatChanged OnCharacterRegenStatChanged;
	FOnCharacterNonRegenStatChanged OnCharacterNonRegenStatChanged;
private:
	void NotifyRegenStatChanged(const FRegenStat& Stat);
	void NotifyNonRegenStatChanged(const FNonRegenStat& Stat);
#pragma endregion
	
#pragma region 언리얼 기본 생성
public:	
	UCharacterBattleStatsComponent();
protected:
	virtual void BeginPlay() override;
private:
	void ProcessRegen();
#pragma endregion

#pragma region DataTable
protected:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	UPROPERTY(EditDefaultsOnly, Category = "0_Stats|Base")	FDataTableRowHandle DTBattleStatsRow;

	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") FGradeStatData GradeStatData;
#pragma endregion

#pragma region 초기화
public:
	/** 리젠용 보너스 주고 싶을 때*/
	UFUNCTION(BlueprintCallable)
	void AddRegenBonus(const TArray<FRegenStat>& InBonusStats);
	/** 논리젠용 보너스 주고 싶을 때*/
	UFUNCTION(BlueprintCallable)
	void AddNonRegenBonus(const TArray<FNonRegenStat>& InBonusStats);

	// 20260310 김도현 : 스탯 전체 브로드캐스트 함수 추가
	UFUNCTION()
	void BroadcastAllStats();

	// 20260312 김도현 : 스탯 초기화 함수 추가
	UFUNCTION()
	void SetData(const FGradeStatData& InGradeStatData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0_Stats|Base")
	float TimerTick = 1.f;
public:
	void InitComponent();
private:
	void RebuildTotalStats();
	UPROPERTY(VisibleAnywhere, Transient)
	bool bInitialized = false;
#pragma endregion

#pragma region 배틀 중 부를 함수
public:
	/// <summary>
	/// Value 변경될때 호출할 함수
	/// </summary>
	/// <param name="StatTag">변경할 스탯의 태그</param>
	/// <param name="DamageAmount">변경할 값(Damage는 -, 힐은 +)</param>
	/// <param name="DamageCauser">해당 수치를 변경한 주체</param>
	/// ex) 플레이어 건물이 적한테 맞았을때, 
	/// ChangeStatValue(Arcanum::BattleStat::Player::NonRegen::CommandCenterStamina, -피해량, 누구);
	UFUNCTION(BlueprintCallable)
	void ChangeStatValue(FGameplayTag StatTag, float DamageAmount, AActor* DamageCauser);

	/**
	 * 버프/디버프처럼 타이머가 필요한 일시적 보정 적용
	 * @param Modifier : 적용하고자 하는 구조체
	 */
	UFUNCTION(BlueprintCallable)
	void ApplyDurationModifier(const FDerivedStatModifier& Modifier);


	UFUNCTION()
	AActor* GetLastDamageCauser() { return LastDamageCauser; }
private:
	UPROPERTY()
	TObjectPtr<AActor> LastDamageCauser;
#pragma endregion

#pragma region GAS의 Duration 계열 함수
private:
	void StartModifierTimer(const FDerivedStatModifier& Mod);
	/**
	 * 스탯 최종 합산 함수 (d = a + b + c)
	 * - 주능력치에 의한 Base(a)와 외부 Modifier들(b, c)을 모두 합산하여 최종 실적용 수치를 결정한다.
	 * - 호출 시점:
	 * 1. RecalculateStatByPrimary 호출 이후 (주능력치 변동 시)
	 * 2. AddModifier / RemoveModifier 호출 이후 (아이템, 버프 변동 시)
	 * - 처리 내용:
	 * - RegenStat의 경우 ModifierMax 갱신 및 Current 값 Clamping
	 * - NonRegenStat의 경우 ModifierValue 갱신
	 * - 최종 수치 변경 알림(UI 반영 등) 수행
	 */
	void UpdateFinalStatValue(FGameplayTag Tag);


	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug")
	TArray<FDerivedStatModifier> ActiveModifiers;
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug")
	TMap<FGuid, FTimerHandle> ModifierTimers;
#pragma endregion


#pragma region 내부 관리 변수
public:
	const TArray<FRegenStat>& GetRegenStats() const { return TotalRegenStats; }
	const TArray<FNonRegenStat>& GetNonRegenStats() const { return TotalNonRegenStats; }
	const FRegenStat* FindRegenStat(const FGameplayTag& InFindTag) const;
	const FNonRegenStat* FindNonRegenStat(const FGameplayTag& InFindTag) const;
private:
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") TArray<FRegenStat> TotalRegenStats;
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") TArray<FNonRegenStat> TotalNonRegenStats;

	// 실제 모든 데이터가 들어있는 핵심 맵
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") TArray<FRegenStat> BaseRegenStats;
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") TArray<FRegenStat> BounsRegenStats;
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") TArray<FNonRegenStat> BaseNonRegenStats;
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") TArray<FNonRegenStat> BounsNonRegenStats;

	FTimerHandle RegenTimerHandle;
#pragma endregion
};
