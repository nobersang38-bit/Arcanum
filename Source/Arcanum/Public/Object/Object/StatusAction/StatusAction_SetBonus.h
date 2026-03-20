#pragma once

#include "CoreMinimal.h"
#include "Object/Object/StatusAction/StatusAction.h"
#include "DataInfo/CommonData/Stats/FStatModifier.h"
#include "StatusAction_SetBonus.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

/**
 * 추영호
 * 장비 세트 효과 탈라샤
 * - 체력 일정수치 이하면 체력 지속 회복
 */

UCLASS()
class ARCANUM_API UStatusAction_SetBonus : public UStatusAction
{
	GENERATED_BODY()

public:
	UStatusAction_SetBonus();

    virtual void StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) override;

protected:
	virtual void InitializeAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus")
	FGameplayTag SetBonusTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus")
	float TriggerHealthPercent = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus")
	FDerivedStatModifier SetBonusModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus")
	TObjectPtr<UNiagaraSystem> SetBonusNiagara = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus")
	FName AttachSocketName = TEXT("Chest");

	UPROPERTY(Transient)
	TObjectPtr<UNiagaraComponent> ActiveNiagaraComponent = nullptr;

#pragma region 세트 보너스 지속시간
protected:
	void StartBonusTimer();
	void EndBonus();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus")
	float BonusDuration = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SetBonus")
	bool bIsBonusActive = false;

	FTimerHandle BonusTimerHandle;
#pragma endregion

#pragma region 세트 보너스 쿨타임
protected:
	void StartCooldown();
	void EndCooldown();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SetBonus")
	float CooldownDuration = 30.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SetBonus")
	bool bIsCooldown = false;

	FTimerHandle CooldownTimerHandle;
#pragma endregion
};
