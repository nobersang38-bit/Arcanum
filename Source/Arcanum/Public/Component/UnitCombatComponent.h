// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Types/BaseUnitData.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"
#include "GameplayTagContainer.h"
#include "Interface/UnitDataInterface.h"
#include "UnitCombatComponent.generated.h"

// 김도현
// 유닛 현재 상태 열거형
UENUM(BlueprintType)
enum class EUnitState : uint8
{
	Idle				UMETA(DisplayName = "Idle"),
	Move				UMETA(DisplayName = "Move"),
	Attack				UMETA(DisplayName = "Attack"),
	ActionRestricted	UMETA(DisplayName = "ActionRestricted"),
	Death				UMETA(DisplayName = "Hit"),
};

// 김도현
// 자동 전투 컴포넌트(AI컴포넌트)
class USphereComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UUnitCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitCombatComponent();

public:
	// 현재 나를 공격중인 적은 몇명인가
	FORCEINLINE int32 GetAttackerCount() const { return AttackerCount; };

	// 런타입 유닛 데이터 가져오기
	FUnitRuntimeData& GetUnitRuntimeData() { return UnitRuntimeData; }

	// Todo : 임시 데미지 받기, 나중에 구조체 받아와야함
	void ApplyDamage(float InDamage, AActor* DamageCauser);

	void TakeDamage();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void DeferredBeginPlay();

	UFUNCTION()
	void TickUpdate();

	void TargetAssigned(ACharacter* Target);

	UFUNCTION()
	void OnBeginDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SelectBestTarget(const TSet<TWeakObjectPtr<ACharacter>>& InDetectedCharacters);
	ACharacter* GetHigherPriorityTarget(ACharacter* CurrentTarget, ACharacter* WinTarget, int32& WinScore);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<USphereComponent> DetectComponent = nullptr;

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

private:
	UFUNCTION()
	void AIInitialize();

	UFUNCTION()
	void Onhit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void Idle();
	void Move();
	void Attack();
	void ActionRestricted(FGameplayTag InActionRestrictedTag);
	void Death();

private:
	UPROPERTY()
	TWeakObjectPtr<class AAIController> OwnerAIC = nullptr;

	FTimerHandle TickTimerHandle;
	FTimerHandle AttackTimerHandle;
	FTimerHandle DeathTimerHandle;

	FBlackboard::FKey TargetActorKey;

	// 현재 내가 공격중인 적이거나 공격해야하는 적
	UPROPERTY()
	TWeakObjectPtr<ACharacter> TargetCharacter = nullptr;

	// 감지된 적 유닛
	UPROPERTY()
	TSet<TWeakObjectPtr<ACharacter>> DetectedCharacters;

	// 현재 나를 공격중인 적은 몇명인가, 나중에 액터나 캐릭터 배열로 바꿀수도 있음
	int32 AttackerCount = 0;

	FGameplayTag TeamID;

	EUnitState CurrentState = EUnitState::Idle;

	UPROPERTY()
	TWeakObjectPtr<ACharacter> TargetNexus = nullptr;

	UPROPERTY()
	TObjectPtr<class UDATargetPriorityWeight> TargetPriorityWeight = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY()
	TWeakObjectPtr<class UCapsuleComponent> OwnerCapsuleComponent = nullptr;
};
