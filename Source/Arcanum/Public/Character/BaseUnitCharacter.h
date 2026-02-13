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
#include "BaseUnitCharacter.generated.h"

// 김도현
// 베이스 유닛 클래스
UCLASS()
class ARCANUM_API ABaseUnitCharacter : public ACharacter, public ITeamInterface, public IUnitDataInterface, public IRuntimeUnitDataInterface, public ICombatInterface
{
	GENERATED_BODY()
public:
#pragma region Debug
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Test")
	//float Speed = 400.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Test")
	//float Range = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Test")
	bool RandomRvoWeight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Test")
	bool bUseStaticMeshAnim = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Test")
	bool MeshHide = false;

#pragma endregion

	ABaseUnitCharacter();

public:
	virtual FGameplayTag GetTeamTag() override;

	// Todo : 연결 느슨하게 변경해야함
	class UCharacterBattleStatsComponent* GetCharacterBattleStatsComponent() { return CharacterBattleStatsComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	// IUnitDataInterface을(를) 통해 상속됨
	const FUnitData& GetUnitData() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UUnitCombatComponent> UnitCombatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCharacterBattleStatsComponent> CharacterBattleStatsComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> HealthBarComponent = nullptr;

#pragma region Debug
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UHierarchicalInstancedStaticMeshComponent> StaticMeshCharacter0 = nullptr;
#pragma endregion


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainData")
	FDataTableRowHandle DTUnitDataRowHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainData")
	FUnitData UnitData;

private:
	void AnimSetting();
	float GetAttackPower();

	UFUNCTION()
	void DataInitialize();

	virtual FUnitRuntimeData& GetUnitRuntimeData() override;

	// ICombatInterface을(를) 통해 상속됨
	void OnAttackNotifyTriggered() override;

	UFUNCTION()
	void RecievedDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
