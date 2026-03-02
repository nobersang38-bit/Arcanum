// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Types/BattleStageInfo.h"
#include "BasementCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBasementChangeHealth, float, CurrentHealth, float, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UBasementCombatComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
public:
	// Sets default values for this component's properties
	UBasementCombatComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma endregion

public:
	FOnBasementChangeHealth OnBasementChangeHealth;
	void SetBasementStat(const FBasementStat& InBasementStat);
	const FBasementStat& GetBasementStat() const { return BasementStat; }

protected:
	UFUNCTION()
	void RecievedDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FBasementStat BasementStat;
};
