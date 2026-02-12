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

#pragma endregion

	ABaseUnitCharacter();

public:
	// Todo : 나중에 데이터 테이블에서 가져와야함, 지금은 임시로 직접 넣음
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainData")
	FDataTableRowHandle DTUnitDataRowHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainData")
	FUnitData UnitData;

private:
	//UFUNCTION()
	//void AIInitialize();

	UFUNCTION()
	void DataInitialize();

	virtual FUnitRuntimeData& GetUnitRuntimeData() override;

	// ICombatInterface을(를) 통해 상속됨
	void OnAttackNotifyTriggered() override;

	// ICombatInterface을(를) 통해 상속됨
	void ApplyDamage(float InDamage, AActor* DamageCauser) override;
};
