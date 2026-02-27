// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Types/BaseUnitData.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "Interface/UnitDataInterface.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "UnitCombatComponent.generated.h"

// 김도현
// 유닛 현재 상태 열거형
UENUM(BlueprintType)
enum class EUnitState : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	Move				UMETA(DisplayName = "Move"),
	Attack				UMETA(DisplayName = "Attack"),
	HitReaction			UMETA(DisplayName = "HitReaction"),
	Death				UMETA(DisplayName = "Death"),
};

// 김도현
// 자동 전투 컴포넌트(AI컴포넌트)
class USphereComponent;
class UUnitStateBase;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UUnitCombatComponent : public UActorComponent
{
	GENERATED_BODY()
	friend class UUnitState_Idle;
	friend class UUnitState_Move;
	friend class UUnitState_Attack;
	friend class UUnitState_HitReaction;
	friend class UUnitState_Death;

public:	
	UUnitCombatComponent();

public:
	// 현재 나를 공격중인 적은 몇명인가
	FORCEINLINE int32 GetAttackerCount() const { return AttackerCount; };

	// 런타입 유닛 데이터 가져오기
	FUnitRuntimeData& GetUnitRuntimeData() { return UnitRuntimeData; }

	void SendDamage(float InDamage);

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void DeferredBeginPlay();

	UFUNCTION()
	void TickUpdate();

	void TargetAssigned(AActor* Target);

	UFUNCTION()
	void OnBeginDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SelectBestTarget(const TSet<TWeakObjectPtr<AActor>>& InDetectedCharacters);
	AActor* GetHigherPriorityTarget(AActor* CurrentTarget, AActor* WinTarget, int32& WinScore);

	UPROPERTY()
	TWeakObjectPtr<UUnitStateBase> CurrentUnitState = nullptr;

	UPROPERTY()
	TMap<EUnitState, UUnitStateBase*> UnitStates;

	UFUNCTION()
	void StateChange(EUnitState InUnitState);

	UFUNCTION()
	void SetupStates();

	UFUNCTION()
	void SetupTick();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUnitAISetting UnitAISetting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUnitData UnitData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUnitRuntimeData UnitRuntimeData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebug_StopAI = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDebug_DrawMoveTargeLine = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag TeamTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AllyTeamTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EnemyTeamTag;


private:
	UFUNCTION()
	void AIInitialize();

	void LightHitReaction();
	void Death(const FRegenStat& InData);
	void StateReset();

private:
	UPROPERTY()
	TWeakObjectPtr<class AAIController> OwnerAIC = nullptr;

	FTimerHandle TickTimerHandle;
	FTimerHandle AttackTimerHandle;
	FTimerHandle DeathTimerHandle;

	FTimerHandle RotateTimerHandle;

	FBlackboard::FKey TargetActorKey;

	// 현재 내가 공격중인 적이거나 공격해야하는 적
	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActorBackup = nullptr;

	// 감지된 적 유닛
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> DetectedActors;

	// 현재 나를 공격중인 적은 몇명인가, 나중에 액터나 캐릭터 배열로 바꿀수도 있음
	int32 AttackerCount = 0;

	FGameplayTag TeamID;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetBasement = nullptr;

	UPROPERTY()
	FTargetPriorityWeightData TargetPriorityWeight;

	UPROPERTY()
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY()
	TWeakObjectPtr<class UCapsuleComponent> OwnerCapsuleComponent = nullptr;

	bool bIsDead = false;

	float RotateInterval = 0.05f;
	float RotateSpeed = 50.0f;
	FTimerDelegate RotateDelegate;
};
