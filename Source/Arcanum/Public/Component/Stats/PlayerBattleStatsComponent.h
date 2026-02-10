#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags/ArcanumTags.h"
#include "DataInfo/Stats/Data/FBattleStats.h"
#include "DataInfo/Stats/DataTable/DTBattleStats.h"
#include "PlayerBattleStatsComponent.generated.h"

// 1. Regen 계열용 (고기 정보)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCombatRegenStatChanged, const FRegenStat&);
// 2. NonRegen 계열용 (현재는 없음. 추후 추가 대비용)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCombatNonRegenStatChanged, const FNonRegenStat&);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UPlayerBattleStatsComponent : public UActorComponent
{
	GENERATED_BODY()

#pragma region 델리게이트
public:
	FOnCombatRegenStatChanged OnRegenStatChanged;
	FOnCombatNonRegenStatChanged OnNonRegenStatChanged;
private:
	void NotifyRegenStatChanged(const FRegenStat& Stat);
	void NotifyNonRegenStatChanged(const FNonRegenStat& Stat);
#pragma endregion

#pragma region 언리얼 기본 생성
public:	
	UPlayerBattleStatsComponent();
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
	UPROPERTY(EditDefaultsOnly, Category = "0_Stats|Base")	FDataTableRowHandle DTPlayerBattleRegenStatsRowHandle;
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") FDTPlayerBattleRegenStatsRow	DTPlayerBattleRegenStatsRow;
	
	UPROPERTY(EditDefaultsOnly, Category = "0_Stats|Base")	FDataTableRowHandle DTPlayerBattleNonRegenStatsRowHandle;
	UPROPERTY(VisibleAnywhere, Category = "1_Stats|RunTimeDebug") FDTPlayerBattleNonRegenStatsRow	DTPlayerBattleNonRegenStatsRow;
#pragma endregion

#pragma region 초기화
public:
	/** 리젠용 보너스 주고 싶을 때*/
	UFUNCTION(BlueprintCallable)
	void AddRegenBonus(const TArray<FRegenStat>& InBonusStats);
	/** 논리젠용 보너스 주고 싶을 때*/
	UFUNCTION(BlueprintCallable)
	void AddNonRegenBonus(const TArray<FNonRegenStat>& InBonusStats);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "0_Stats|Base")
	float TimerTick = 1.f;
private:
	void InitComponent();
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

private:
	UPROPERTY()
	TObjectPtr<AActor> LastDamageCauser;
#pragma endregion


#pragma region 내부 관리 변수
public:
	const TArray<FRegenStat>& GetRegenStats() const { return TotalRegenStats; }
	const TArray<FNonRegenStat>& GetNonRegenStats() const { return TotalNonRegenStats; }
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
