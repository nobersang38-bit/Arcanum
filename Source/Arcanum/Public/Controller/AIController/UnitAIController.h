// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/TeamInterface.h"
#include "UnitAIController.generated.h"

// 김도현
// 아군 유닛, 적군 유닛 AI컨트롤러
// 타겟 우선순위를 두기 위한 기능포함
UCLASS()
class ARCANUM_API AUnitAIController : public AAIController, public ITeamInterface
{
	GENERATED_BODY()
public:
	AUnitAIController();
	virtual void BeginPlay() override;

public:
	// 현재 나를 공격중인 적은 몇명인가
	UFUNCTION(BlueprintCallable)
	int32 GetAttackerCount() const { return AttackerCount; };

	// Todo : 나중에 공격하는 함수에 추가해서 알아서 작동하게 해야함
	// 공격중인 적 하나 추가, 이 대상을 공격할때 외부에서 꼭 이 함수 호출해야함
	void AddAttackerCount() { AttackerCount++; };

	// Todo : 나중에 공격 안하는 함수에 추가해서 알아서 작동하게 해야함
	// 공격중인 적 하나 제거, 이 대상을 더이상 공격하지 않으면 꼭 이 함수 호출해야함
	void RemoveAttackerCount() { AttackerCount--; };

	// Todo : 나중에 데이터 테이블에서 가져와야함, 지금은 임시로 직접 넣음
	virtual FGameplayTag GetTeamTag_Implementation() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UBehaviorTree> BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UDATargetPriorityWeight> TargetPriorityWeight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Arcanum.TeamID"))
	FGameplayTag TeamID;

private:
	AActor* TargetCalcTest();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	// 감지용 구체 콜리전
	UPROPERTY()
	TObjectPtr<class USphereComponent> DetectComponent = nullptr;

	// 감지된 적 유닛
	UPROPERTY()
	TSet<TWeakObjectPtr<ACharacter>> DetectedCharacters;

	// 현재 나를 공격중인 적은 몇명인가, 나중에 액터나 캐릭터 배열로 바꿀수도 있음
	int32 AttackerCount = 0;
};
