// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/FSM/Unit/UnitState_Move.h"
#include "Component/UnitCombatComponent.h"
#include "Engine/OverlapResult.h"

void UUnitState_Move::OnEnter(UUnitCombatComponent* UnitCombatComponent)
{
	if (!UnitCombatComponent) return;
	Internal_UnitCombatComponent = UnitCombatComponent;
	UE_LOG(LogTemp, Warning, TEXT("UUnitState_Move::OnEnter"));
}

void UUnitState_Move::OnTick(float DeltaTime)
{
	if (!Internal_UnitCombatComponent.IsValid()) return;

	Internal_UnitCombatComponent->StateReset();

	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape MySphere = FCollisionShape::MakeSphere(Internal_UnitCombatComponent->TargetPriorityWeight.GetDetectDistance());
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Internal_UnitCombatComponent->GetOwner());

	// 특정 위치(GetActorLocation)에서 한 프레임 즉시 검사
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OutOverlaps,
		Internal_UnitCombatComponent->GetOwner()->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn, // 설정한 채널
		MySphere,
		Params
	);

	Internal_UnitCombatComponent->DetectedActors.Empty();
	for (int i = 0; i < OutOverlaps.Num(); i++)
	{
		Internal_UnitCombatComponent->OnBeginDetected(nullptr, OutOverlaps[i].GetActor(), nullptr, 0, false, FHitResult());
	}
	if (Internal_UnitCombatComponent->DetectedActors.Num() > 0)
	{
		Internal_UnitCombatComponent->SelectBestTarget(Internal_UnitCombatComponent->DetectedActors);
	}
	else
	{
		Internal_UnitCombatComponent->TargetAssigned(Internal_UnitCombatComponent->TargetBasement.Get());
	}

	if (Internal_UnitCombatComponent->TargetActor.IsValid())
	{
		float Distance = (Internal_UnitCombatComponent->TargetActor->GetActorLocation() - Internal_UnitCombatComponent->GetOwner()->GetActorLocation()).SquaredLength();
		if (Distance <= Internal_UnitCombatComponent->UnitAISetting.AttackRange)
		{
			Internal_UnitCombatComponent->StateChange(EUnitState::Attack);
		}
	}
}

void UUnitState_Move::OnExit()
{
}
