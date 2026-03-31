// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Types/BTPlayerStruct.h"
#include "UI/Battle/BattlePlayerController.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Object/Skills/SkillBase.h"

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
		PlayerController->Internal_SpawnUnit(AllyBasement->GetActorLocation() + (AllyBasement->GetActorForwardVector() * 450.0f) + (FVector::UpVector * 20.0f));
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
		bool bIsSkillSuccess = CostCheck();
		switch (SkillType)
		{
		case EBSkillType::None:
			break;

		case EBSkillType::BasicAttack:
			PlayerController->TriggerBasicAttackHit();
          	break;

		case EBSkillType::BasicSkill:
			PlayerController->TriggerSkill();
			break;

		case EBSkillType::UltimateSkill:
			if (PlayerController->CachedPlayerCharacter.IsValid())
			{
				APlayerCharacter* PlayerCharacter = PlayerController->CachedPlayerCharacter.Get();
				UObject* TargetObject = PlayerCharacter->CachedAIC->GetBlackboardComponent()->GetValueAsObject(TargetActorName);
				AActor* TargetActor = Cast<AActor>(TargetObject);

				PlayerController->UltimateSkillPressed();

				//GetWorld()->GetTimerManager().ClearTimer(UltimateStep1TimerHandle);
				GetWorld()->GetTimerManager().ClearTimer(UltimateStep2TimerHandle);
				//GetWorld()->GetTimerManager().SetTimer(UltimateStep1TimerHandle, this, &UBTPlayerDataObject::UltimateStep1, 0.1f, false, 0.1f);
				GetWorld()->GetTimerManager().SetTimer(UltimateStep2TimerHandle, this, &UBTPlayerDataObject::UltimateStep2, 0.5f, false, 0.5f);
				PlayerCharacter->UpdateUltimatePreviewLocation(TargetActor->GetActorLocation());
				//PlayerController->UltimateSkillReleased();
			}
			else return false;
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

		return bIsSkillSuccess;
	}

	return false;
}

bool UBTPlayerDataObject::RangeCheck(EBSkillType InSkillType)
{
	if (!PlayerController.IsValid()) return false;
	if (!PlayerController->CachedPlayerCharacter.IsValid()) return false;

	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!BattleSubsystem) return false;

	APlayerCharacter* PlayerCharacter = PlayerController->CachedPlayerCharacter.Get();
	float CharacterToTargetDistance = PlayerCharacter->CachedAIC->GetBlackboardComponent()->GetValueAsFloat(CharacterToTargetDistanceName);
	
	switch (InSkillType)
	{
	case EBSkillType::None:
	{
		return false;
	}
		break;
	case EBSkillType::BasicAttack:
	{
		FGameplayTag Tag = BattleSubsystem->GetCurrentBasicAttackSkillTag();
		if (TObjectPtr<USkillBase>* SkillBaseTO = PlayerController->SkillBaseInstances.Find(Tag))
		{
			if (USkillBase* SkillBase = SkillBaseTO->Get())
			{
				if (SkillBase->GetSkillInfo().EnabledRange > CharacterToTargetDistance)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
		break;
	case EBSkillType::BasicSkill:
	{
		FGameplayTag Tag = BattleSubsystem->GetCurrentBasicSkillTag();
		if (TObjectPtr<USkillBase>* SkillBaseTO = PlayerController->SkillBaseInstances.Find(Tag))
		{
			if (USkillBase* SkillBase = SkillBaseTO->Get())
			{
				if (SkillBase->GetSkillInfo().EnabledRange > CharacterToTargetDistance)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
		break;
	case EBSkillType::UltimateSkill:
	{
		FGameplayTag Tag = BattleSubsystem->GetCurrentLegendarySkillData()->SkillTag;
		if (TObjectPtr<USkillBase>* SkillBaseTO = PlayerController->SkillBaseInstances.Find(Tag))
		{
			if (USkillBase* SkillBase = SkillBaseTO->Get())
			{
				if (SkillBase->GetSkillInfo().EnabledRange > CharacterToTargetDistance)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		}
	}
		break;
	case EBSkillType::Item01:
	{
		return true;
	}
		break;
	case EBSkillType::Item02:
	{
		return true;
	}
		break;
	case EBSkillType::Swap:
	{
		return true;
	}
		break;
	default:
		break;
	}
	return false;
}

void UBTPlayerDataObject::UltimateStep1()
{
	APlayerCharacter* PlayerCharacter = PlayerController->CachedPlayerCharacter.Get();
	UObject* TargetObject = PlayerCharacter->CachedAIC->GetBlackboardComponent()->GetValueAsObject(TargetActorName);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	PlayerCharacter->UpdateUltimatePreviewLocation(TargetActor->GetActorLocation());
}

void UBTPlayerDataObject::UltimateStep2()
{
	PlayerController->UltimateSkillReleased();
}
