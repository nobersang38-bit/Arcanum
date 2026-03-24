// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Types/BTPlayerStruct.h"
#include "UI/Battle/BattlePlayerController.h"

bool FBTPlayerStruct::CostCheck()
{
	if (PlayerController.IsValid())
	{
		//return PlayerController->SkillCostChecker(SkillTag, Level);
	}
	return false;
}

void FBTPlayerStruct::UseSkill()
{
	if (PlayerController.IsValid())
	{
		switch (SkillType)
		{
		case EBSkillType::None:
			break;
		case EBSkillType::BasicAttack:
			//PlayerController->BasicAttack();
			PlayerController->TriggerBasicAttackHit();
          	break;
		case EBSkillType::BasicSkill:
			//PlayerController->BasicSkill();
			PlayerController->TriggerSkill();
			break;
		case EBSkillType::UltimateSkill:
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
	}
}
