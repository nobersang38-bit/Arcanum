// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/UnitCombatComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/UnitDataInterface.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UUnitCombatComponent::UUnitCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUnitCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UUnitCombatComponent::DeferredBeginPlay);
}

void UUnitCombatComponent::DeferredBeginPlay()
{
	if (!GetOwner()->IsA<ACharacter>())
	{
		UE_LOG(LogTemp, Error, TEXT("부모가 ACharacter가 아닙니다"));
		return;
	}
	// 감지 컴포넌트가 없다면 부모액터에 넣기
	if (!DetectComponent.IsValid()) 
	{
		if (GetOwner())
		{
			DetectComponent = NewObject<USphereComponent>(GetOwner(), USphereComponent::StaticClass());
		}

		if (DetectComponent.IsValid())
		{
			DetectComponent->RegisterComponent();
			DetectComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}

	// AI 세팅 가져오기
	if (GetOwner()->GetClass()->ImplementsInterface(UUnitDataInterface::StaticClass()))
	{
		auto Interface = Cast<IUnitDataInterface>(GetOwner());
		UnitAISetting = Interface->GetUnitData().Info.AISetting;
	}

	float InitialDelay = FMath::FRandRange(UnitAISetting.AITickParams.InitialDelayRange.X, UnitAISetting.AITickParams.InitialDelayRange.Y);
	float UpdateInterval = FMath::FRandRange(UnitAISetting.AITickParams.UpdateIntervalRange.X, UnitAISetting.AITickParams.UpdateIntervalRange.Y);
	GetWorld()->GetTimerManager().ClearTimer(TickTimerHandle);
	// 틱 재생
	GetWorld()->GetTimerManager().SetTimer(TickTimerHandle, this, &UUnitCombatComponent::TickUpdate, UpdateInterval, true, InitialDelay);
}

void UUnitCombatComponent::TickUpdate()
{

}