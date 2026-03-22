// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Types/BTPlayerStruct.h"
#include "UI/Battle/BattlePlayerController.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"

void UBTPlayerDataObject::SpawnUnit()
{
	//UsingAllyUnitSlots
	if (PlayerController.IsValid())
	{
		TArray<FGameplayTag> KeyArray;
		TArray<UBattleAllyUnitSlotWidget*> ValueArray;
		PlayerController->UsingAllyUnitSlots.GenerateKeyArray(KeyArray);
		PlayerController->UsingAllyUnitSlots.GenerateValueArray(ValueArray);

		int32 RandomIDX = FMath::RandRange(0, FMath::Max<int32>(0, KeyArray.Num() - 1));
		PlayerController->ReadySpawnUnit(KeyArray[RandomIDX], ValueArray[RandomIDX]);

		UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		
		AActor* AllyBasement = BattleSubsystem->GetAllyBasement();
		PlayerController->Internal_SpawnUnit(AllyBasement->GetActorLocation() + AllyBasement->GetActorForwardVector() * 450.0f);
	}
}

void UBTPlayerDataObject::SetTargetActor(AActor* InTargetACtor)
{
	if (PlayerController.IsValid())
	{
		PlayerController->SkillTargetActor = InTargetACtor;
	}
}

bool UBTPlayerDataObject::CostCheck()
{
	if (PlayerController.IsValid())
	{
		UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		FGameplayTag SkillTag;
		int32 SkillLevel;
		if (BattleSubsystem)
		{
			switch (SkillType)
			{
			case EBSkillType::None:
				break;

			case EBSkillType::BasicAttack:
				SkillTag = BattleSubsystem->GetCurrentBasicAttackSkillTag();
				SkillLevel = BattleSubsystem->GetCurrentBasicAttackSkillLevel();
				return PlayerController->SkillCostChecker(SkillTag, SkillLevel);
				break;

			case EBSkillType::BasicSkill:
				SkillTag = BattleSubsystem->GetCurrentBasicSkillTag();
				SkillLevel = BattleSubsystem->GetCurrentBasicSkillLevel();
				return PlayerController->SkillCostChecker(SkillTag, SkillLevel);
				break;

			case EBSkillType::UltimateSkill:
				SkillTag = BattleSubsystem->GetLegendaryUltimateSkillTag();
				SkillLevel = BattleSubsystem->GetLegendaryUltimateSkillLevel();
				return PlayerController->SkillCostChecker(SkillTag, SkillLevel);
				break;

			case EBSkillType::Item01:
				
				break;

			case EBSkillType::Item02:
				
				break;

			case EBSkillType::Swap:
				return true;
				break;

			default:
				break;
			}
		}
	}
	return false;
}

bool UBTPlayerDataObject::UseSkill()
{
	if (PlayerController.IsValid())
	{
		switch (SkillType)
		{
		case EBSkillType::None:
			break;
		case EBSkillType::BasicAttack:
			PlayerController->BasicAttack();
			break;
		case EBSkillType::BasicSkill:
			PlayerController->BasicSkill();
			break;
		case EBSkillType::UltimateSkill:
			return false;
			//PlayerController->UltimateSkill();
			break;
		case EBSkillType::Item01:
			PlayerController->Item1();
			break;
		case EBSkillType::Item02:
			PlayerController->Item2();
			break;
		case EBSkillType::Swap:
			PlayerController->WeaponSwap();
			break;
		default:
			break;
		}

		return PlayerController->bIsSkillSuccess;
	}

	return false;
}
