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


#pragma region 게터
public:
	// 현재 나를 공격중인 적은 몇명인가
	FORCEINLINE int32 GetAttackerCount() const { return AttackerCount; };

	// 런타입 유닛 데이터 가져오기
	FORCEINLINE FUnitRuntimeData& GetUnitRuntimeData() { return UnitRuntimeData; }

	FORCEINLINE bool GetIsDead() const { return bIsDead; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetAttackSpeed(float InAttackSpeedRate) { AttackSpeed = InAttackSpeedRate; }

#pragma endregion

#pragma region 액션
public:
	void SendDamage(float InDamage);

	void UnitActivate();
	void UnitDeactive();
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

	void EnemyFind();

	// 공격 대상 지정
	void TargetAssigned(AActor* Target);

	// 블랙보드의 타겟 오브젝트의 값 변경으로 이동
	void MoveToTarget(AActor* Target);

	// 최우선 타겟 지정
	void SelectBestTarget(const TSet<TWeakObjectPtr<AActor>>& InDetectedCharacters);

	// 최우선 타겟 검색 후 리턴
	AActor* GetHigherPriorityTarget(AActor* CurrentTarget, AActor* WinTarget, float& WinScore);

	// 공격이 가능한 거리라면 true
	bool IsCanAttackRange();

#pragma endregion

#pragma region 스탯 변경

#pragma endregion



#pragma region 상태
public:
	void LightHitReaction(float InDamage);
protected:
	void Death(const FRegenStat& InData);

#pragma endregion


#pragma region 수동 설정 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AllyTeamTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EnemyTeamTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> FloatingTextActorClass = nullptr;
#pragma endregion


#pragma region 자동 설정 데이터
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FUnitData UnitData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag TeamTag;

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
	FBlackboard::FKey AttackRangeKey;

	// 1이면 원래 몽타주 속도
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AttackSpeed = 1.0f;

#pragma endregion


#pragma region 런타임 데이터
private:
	UPROPERTY(VisibleAnywhere, Category = "RuntimeData")
	TWeakObjectPtr<UUnitStateBase> CurrentUnitState = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "RuntimeData")
	TMap<EUnitState, UUnitStateBase*> UnitStates;

	UPROPERTY(VisibleAnywhere, Category = "RuntimeData")
	TWeakObjectPtr<AActor> TargetActor = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "RuntimeData")
	TSet<TWeakObjectPtr<AActor>> DetectedActors;
	
	UPROPERTY(VisibleAnywhere, Category = "RuntimeData")
	FUnitRuntimeData UnitRuntimeData;

	// 현재 나를 공격중인 적은 몇명인가, 나중에 액터나 캐릭터 배열로 바꿀수도 있음
	int32 AttackerCount = 0;

	bool bIsDead = false;

#pragma endregion


#pragma region 핸들
	private:
		FTimerHandle TickTimerHandle;
		FTimerHandle DeathTimerHandle;

		FDelegateHandle DeathDelegateHandle;
#pragma endregion
};
