// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TeamInterface.h"
#include "Data/Types/UnitData.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"
#include "Interface/UnitDataInterface.h"
#include "Interface/RuntimeUnitDataInterface.h"
#include "Interface/CombatInterface.h"
#include "Interface/PoolingInterface.h"
#include "Interface/StatModifierInterface.h"
#include "Interface/StatInterface.h"
#include "BaseUnitCharacter.generated.h"


// 김도현
// 베이스 유닛 클래스
UCLASS()
class ARCANUM_API ABaseUnitCharacter : public ACharacter, 
	public ITeamInterface, public IUnitDataInterface, public IRuntimeUnitDataInterface, public ICombatInterface, public IPoolingInterface, public IStatModifierInterface, public IStatInterface
{
	GENERATED_BODY()
	friend class UStatusAction_UnitHealth;
public:
#pragma region Debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Test")
	bool RandomRvoWeight = false;
#pragma endregion

	ABaseUnitCharacter();

public:
	void SetUnit(FUnitInfoSetting InUnitData, bool bUseReadyHologram = false);

	virtual FGameplayTag GetTeamTag() const override;

	class UCharacterBattleStatsComponent* GetCharacterBattleStatsComponent() { return CharacterBattleStatsComponent; }

	UFUNCTION(BlueprintCallable)
	class UUnitCombatComponent* GetUnitCombatComponent() { return UnitCombatComponent; }

	void OuntLineStart(const UCurveFloat* CurveFloat, float InTime, float DeltaTime);
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// IUnitDataInterface을(를) 통해 상속됨
	const FUnitData& GetUnitData() override;
	virtual bool GetIsDead() override;

	void UnitActivate();
	void UnitDeactive();


	void SetHologramType(bool bUseHologram);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UUnitCombatComponent> UnitCombatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCharacterBattleStatsComponent> CharacterBattleStatsComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStatusActionComponent> StatusActionComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HealthBarComponent = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainData")
	FDataTableRowHandle DTUnitDataRowHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainData", meta = (DisplayPriority = "0"))
	FUnitData UnitData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MaterialCurve")
	TObjectPtr<UCurveFloat> OutLineCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MaterialCurve")
	float OutLineTime = 0.3f;

	float RefOutlineTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HologramMaterial")
	TObjectPtr<UMaterial> HologramMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TSubclassOf<class AFloatingDamageText> TextFloatingClass = nullptr;

private:
	void AnimSetting();
	float GetAttackPower();

	UFUNCTION()
	void DataInitialize(bool bUseReadyHologram = false);

	virtual FUnitRuntimeData& GetUnitRuntimeData() override;

	// ICombatInterface을(를) 통해 상속됨
	void OnAttackNotifyTriggered() override;

	UFUNCTION()
	void RecievedDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void UpdateUnitData();

	bool IsSetupUnit = false;

	// IPoolingInterface을(를) 통해 상속됨
	void ActivateItem() override;
	void DeactiveItem() override;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> OutlineDynamicMI = nullptr;

	FTimerHandle OutlineTimeHandle;

	FDelegateHandle SetPercentDelegateHandle;

	UPROPERTY()
	TArray<UMaterialInterface*> MaterialBackup;

	// IStatModifierInterface을(를) 통해 상속됨
	void AddLevelModifierEntry(const FLevelModifierEntry& LevelModifierEntry) override;
	void AddDerivedStatModifier(const FDerivedStatModifier& DerivedStatModifier) override;

	// IStatModifierInterface을(를) 통해 상속됨
	void ChangeStat(const FGameplayTag& InTag, float InValue) override;

	// IStatInterface을(를) 통해 상속됨
	const UCharacterBattleStatsComponent* GetStatComponent() const override;
};
