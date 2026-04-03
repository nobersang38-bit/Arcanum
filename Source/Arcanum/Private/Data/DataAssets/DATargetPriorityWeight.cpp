// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "Interface/UnitDataInterface.h"
#include "Interface/RuntimeUnitDataInterface.h"
#include "GameFramework/Character.h"

float FTargetPriorityWeightData::CalculateScore(AActor* MyCharacter, AActor* TargetCharacter) const
{
	if (!MyCharacter) return 0.0f;
	if (!TargetCharacter) return 0.0f;
	auto MyDataInterface = Cast<IUnitDataInterface>(MyCharacter);
	auto MyRuntimeDataInterface = Cast<IRuntimeUnitDataInterface>(MyCharacter);
	auto TargetDataInterface = Cast<IUnitDataInterface>(TargetCharacter);
	auto TargetRuntimeDataInterface = Cast<IRuntimeUnitDataInterface>(TargetCharacter);
	if (!MyDataInterface || !MyRuntimeDataInterface) return 0.0f;
	if (!TargetDataInterface || !TargetRuntimeDataInterface) return 0.0f;

	AActor* ResultCharacter = nullptr;
	float CurrentTargetScore = 0;

	const FUnitData& MyUnitData = MyDataInterface->GetUnitData();
	const FUnitRuntimeData& MyUnitRuntimeData = MyRuntimeDataInterface->GetUnitRuntimeData();
	const FUnitData& TargetUnitData = TargetDataInterface->GetUnitData();
	const FUnitRuntimeData& TargetUnitRuntimeData = TargetRuntimeDataInterface->GetUnitRuntimeData();

	// 파트당 최대 점수는 10, 최소 점수는  0

	// 랜덤하게 지정할 점수
	CurrentTargetScore += CalculateScoreRandomTargetWeight();
	
	// 해당 적을 공격하고 있는 아군이 적을수록 점수 올라감
	CurrentTargetScore += CalculateScoreAttackingTargetNumWeight(TargetUnitRuntimeData);

	// 나와 적의 거리가 가까울 수록 점수가 높음
	CurrentTargetScore += CalculateScoreDistanceWeight(MyCharacter, TargetCharacter);
	
	// 나를 공격하는 대상에게 점수를 줌
	CurrentTargetScore += CalculateScoreAggroWeight(MyUnitRuntimeData, TargetCharacter);

	// 해당 적의 체력이 적으면 점수가 높음(백분율)
	CurrentTargetScore += CalculateScoreLowHealthWeight();

	// 엘리트일수록 점수가 높음
	CurrentTargetScore += CalculateScoreEliteWeight(TargetUnitData);
	
    return CurrentTargetScore;
}

AActor* FTargetPriorityWeightData::CalculateWinActor(AActor* InMyCharacter, TArray<AActor*> InActors) const
{
	if (!InMyCharacter) return nullptr;
	if (InActors.IsEmpty()) return nullptr;

	AActor* ResultTarget = nullptr;
	float HighScore = 0.0f;

	for (const auto& Actor : InActors)
	{
		if (Actor->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
		{
			auto Interface = Cast<IRuntimeUnitDataInterface>(Actor);
			bool bIsDead = Interface->GetIsDead();
			if (bIsDead) continue;
		}
		float CurrentScore = CalculateScore(InMyCharacter, Actor);
		if (HighScore <= CurrentScore)
		{
			ResultTarget = Actor;
			HighScore = CurrentScore;
		}
	}
	return ResultTarget;
}

// 랜덤하게 지정할 점수
float FTargetPriorityWeightData::CalculateScoreRandomTargetWeight() const
{
	float CurrentTargetScore = 0.0f;
	if (RandomTargetWeightSet.bUseWeight)
	{
		float PartScore = static_cast<float>(FMath::RandRange(0, 10));
		PartScore *= RandomTargetWeightSet.TargetWeight;
		PartScore = FMath::Clamp(PartScore, 0.0f, 10.0f);
		CurrentTargetScore += PartScore;
	}
	return CurrentTargetScore;
}

// 해당 적을 공격하고 있는 아군이 적을수록 점수 올라감
float FTargetPriorityWeightData::CalculateScoreAttackingTargetNumWeight(const FUnitRuntimeData& TargetUnitRuntimeData) const
{
	float CurrentTargetScore = 0.0f;
	if (AttackingTargetNumWeightSet.bUseWeight)
	{
		float PartScore = 10.0f;

		PartScore -= TargetUnitRuntimeData.AttackingTargets.Num();
		PartScore = FMath::Clamp(PartScore, 0.0f, 10.0f);
		PartScore *= AttackingTargetNumWeightSet.TargetWeight;
		PartScore = FMath::Clamp(PartScore, 0.0f, 10.0f);

		CurrentTargetScore += PartScore;
	}
	return CurrentTargetScore;
}

// 나와 적의 거리가 가까울 수록 점수가 높음
float FTargetPriorityWeightData::CalculateScoreDistanceWeight(AActor* MyCharacter, AActor* TargetCharacter) const
{
	float CurrentTargetScore = 0.0f;
	if (DistanceWeightSet.bUseWeight)
	{
		float PartScore = 10.0f;

		float Distance = (MyCharacter->GetActorLocation() - TargetCharacter->GetActorLocation()).SquaredLength();
		Distance *= 0.0001f;

		PartScore -= Distance;
		PartScore *= DistanceWeightSet.TargetWeight;
		PartScore = FMath::Clamp(PartScore, 0.0f, 10.0f);

		CurrentTargetScore += PartScore;
	}
	return CurrentTargetScore;
}

// 나를 공격하는 대상에게 점수를 줌
float FTargetPriorityWeightData::CalculateScoreAggroWeight(const FUnitRuntimeData& MyUnitRuntimeData, AActor* TargetCharacter) const
{
	if (!TargetCharacter) return 0.0f;

	float CurrentTargetScore = 0.0f;
	if (AggroWeightSet.bUseWeight)
	{
		if (MyUnitRuntimeData.AttackingTargets.Contains(TargetCharacter))
		{
			float PartScore = 0.0f;
			PartScore += 10.0f;
			PartScore *= AggroWeightSet.TargetWeight;
			PartScore = FMath::Clamp(PartScore, 0.0f, 10.0f);

			CurrentTargetScore += PartScore;
		}
	}
	return CurrentTargetScore;
}

// 해당 적의 체력이 적으면 점수가 높음(백분율)
// Todo : CalculateScoreLowHealthWeight 미완성
float FTargetPriorityWeightData::CalculateScoreLowHealthWeight() const
{
	return 0.0f;
}

// 엘리트일수록 점수가 높음
float FTargetPriorityWeightData::CalculateScoreEliteWeight(const FUnitData& TargetUnitData) const
{
	float CurrentTargetScore = 0.0f;
	if (EliteWeightSet.bUseWeight)
	{
		float PartScore = 0.0f;
		if (TargetUnitData.Info.AISetting.bIsElite)
		{
			PartScore += 10.0f;
			PartScore *= EliteWeightSet.TargetWeight;
			PartScore = FMath::Clamp(PartScore, 0.0f, 10.0f);

			CurrentTargetScore += PartScore;
		}
	}
	return CurrentTargetScore;
}
