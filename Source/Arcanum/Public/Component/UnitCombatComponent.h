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

#pragma region 언리얼 기본 생성
public:
	UUnitCombatComponent();

protected:
	virtual void BeginPlay() override;
#pragma endregion


#pragma region 생명 주기
protected:
	UFUNCTION()
	void DeferredBeginPlay();

	UFUNCTION()
	void TickUpdate();
#pragma endregion


#pragma region 초기설정
protected:
	UFUNCTION()
	void AIInitialize();

	UFUNCTION()
	void SetupTick();

	UFUNCTION()
	void SetupStates();
#pragma endregion


#pragma region 헬퍼
protected:
	// 상태변경
	void StateChange(EUnitState InUnitState);

	// Todo KDH : FSM 내부로 이동해야함
	void OnBeginDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 공격 대상 지정
	void TargetAssigned(AActor* Target);

	// 최우선 타겟 가지고 있기 및 지정
	void SelectBestTarget(const TSet<TWeakObjectPtr<AActor>>& InDetectedCharacters);

	// 최우선 타겟 검색 후 리턴
	AActor* GetHigherPriorityTarget(AActor* CurrentTarget, AActor* WinTarget, int32& WinScore);
	
#pragma endregion


#pragma region 상태
	void LightHitReaction();
	void Death(const FRegenStat& InData);
	void StateReset();

#pragma endregion


public:
	// 현재 나를 공격중인 적은 몇명인가
	FORCEINLINE int32 GetAttackerCount() const { return AttackerCount; };

	// 런타입 유닛 데이터 가져오기
	FUnitRuntimeData& GetUnitRuntimeData() { return UnitRuntimeData; }

	void SendDamage(float InDamage);


#pragma region 수동 설정 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AllyTeamTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EnemyTeamTag;

#pragma endregion


#pragma region 자동 설정 데이터
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUnitData UnitData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag TeamTag;

	UPROPERTY()
	FUnitAISetting UnitAISetting;

	UPROPERTY()
	FTargetPriorityWeightData TargetPriorityWeight;
#pragma endregion


#pragma region 캐싱
private:
	UPROPERTY()
	TWeakObjectPtr<class AAIController> OwnerAIC = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY()
	TWeakObjectPtr<class UCapsuleComponent> OwnerCapsuleComponent = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetBasement = nullptr;

	FBlackboard::FKey TargetActorKey;

#pragma endregion


#pragma region 런타임 데이터
private:
	UPROPERTY()
	TWeakObjectPtr<UUnitStateBase> CurrentUnitState = nullptr;

	UPROPERTY()
	TMap<EUnitState, UUnitStateBase*> UnitStates;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AActor> TargetActorBackup = nullptr;

	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> DetectedActors;

	UPROPERTY()
	FUnitRuntimeData UnitRuntimeData;

	// 현재 나를 공격중인 적은 몇명인가, 나중에 액터나 캐릭터 배열로 바꿀수도 있음
	int32 AttackerCount = 0;

	bool bIsDead = false;

	float RotateInterval = 0.05f;

	float RotateSpeed = 50.0f;

#pragma endregion


#pragma region 타이머 핸들
	private:
		FTimerHandle TickTimerHandle;
		FTimerHandle AttackTimerHandle;
		FTimerHandle DeathTimerHandle;

		FTimerHandle RotateTimerHandle;

		FTimerDelegate RotateDelegate;

#pragma endregion
};
