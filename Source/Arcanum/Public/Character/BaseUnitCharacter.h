// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TeamInterface.h"
#include "Data/Types/UnitData.h"
#include "BehaviorTree/Blackboard/BlackboardKey.h"
#include "Interface/UnitDataInterface.h"
#include "BaseUnitCharacter.generated.h"

// 김도현
// 베이스 유닛 클래스
UCLASS()
class ARCANUM_API ABaseUnitCharacter : public ACharacter, public ITeamInterface, public IUnitDataInterface
{
	GENERATED_BODY()
public:
	ABaseUnitCharacter();

public:
	// 현재 나를 공격중인 적은 몇명인가
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetAttackerCount() const { return AttackerCount; };

	// Todo : 나중에 공격하는 함수에 추가해서 알아서 작동하게 해야함
	// 공격중인 적 하나 추가, 이 대상을 공격할때 외부에서 꼭 이 함수 호출해야함
	FORCEINLINE void AddAttackerCount() { AttackerCount++; };

	// Todo : 나중에 공격 안하는 함수에 추가해서 알아서 작동하게 해야함
	// 공격중인 적 하나 제거, 이 대상을 더이상 공격하지 않으면 꼭 이 함수 호출해야함
	FORCEINLINE void RemoveAttackerCount() { AttackerCount--; };

	// Todo : 나중에 데이터 테이블에서 가져와야함, 지금은 임시로 직접 넣음
	virtual FGameplayTag GetTeamTag_Implementation() override;

	// 매니저가 타겟을 할당해줌
	void TargetAssigned(ACharacter* Target);

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
	TObjectPtr<class USphereComponent> DetectComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UUnitCombatComponent> UnitCombatComponent = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MainData")
	FDataTableRowHandle DTUnitDataRowHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainData")
	FUnitData UnitData;

private:
	UFUNCTION()
	void AIInitialize();

	UFUNCTION()
	void DataInitialize();

	AActor* TargetCalcTest();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	UPROPERTY()
	TWeakObjectPtr<class AAIController> AIController = nullptr;

	// 현재 내가 공격중인 적이거나 공격해야하는 적
	UPROPERTY()
	TWeakObjectPtr<ACharacter> TargetCharacter = nullptr;

	// 감지된 적 유닛
	UPROPERTY()
	TSet<TWeakObjectPtr<ACharacter>> DetectedCharacters;

	// 현재 나를 공격중인 적은 몇명인가, 나중에 액터나 캐릭터 배열로 바꿀수도 있음
	int32 AttackerCount = 0;

	FBlackboard::FKey TargetActorKey;
};
