// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BasementCombatComponent.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameplayTags/ArcanumTags.h"

// Sets default values for this component's properties
UBasementCombatComponent::UBasementCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UBasementCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.RemoveDynamic(this, &UBasementCombatComponent::RecievedDamage);
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UBasementCombatComponent::RecievedDamage);

#pragma region 디버그
	if (UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		SetBasementStat(BattleSubsystem->GetBasementStat(Arcanum::Unit::Faction::Enemy::Root));
	}
#pragma endregion
}


// Called every frame
void UBasementCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBasementCombatComponent::SetBasementStat(const FBasementStat& InBasementStat)
{
	BasementStat = InBasementStat;
	BasementStat.CommandCenterMaxHP = BasementStat.CommandCenterCurrentHP;
}

void UBasementCombatComponent::RecievedDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	float Health = BasementStat.CommandCenterCurrentHP.BaseValue;
	BasementStat.CommandCenterCurrentHP.BaseValue = FMath::Clamp(Health - Damage, 0, FLT_MAX);
	OnBasementChangeHealth.Broadcast(BasementStat.CommandCenterCurrentHP.BaseValue, BasementStat.CommandCenterMaxHP.BaseValue);
}

