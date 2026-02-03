// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTags/ArcanumTags.h"

namespace Arcanum
{
	namespace Player
	{
		namespace ID
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Elara, "Arcanum.Player.ID.Elara", "엘라라");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Iris, "Arcanum.Player.ID.Iris", "아이리스");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Celestia, "Arcanum.Player.ID.Celestia", "셀레스티아");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Lynette, "Arcanum.Player.ID.Lynette", "리넷");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Arna, "Arcanum.Player.ID.Arna", "아르나");
		}

		namespace Grade
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Common, "Arcanum.Player.Grade.Common", "일반");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Rare, "Arcanum.Player.Grade.Rare", "레어");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Epic, "Arcanum.Player.Grade.Epic", "영웅");
		}
	}

	namespace Resource
	{
		namespace Health
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxHealth, "Arcanum.Resource.Health.MaxHealth", "체력 최대치");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ValueHealth, "Arcanum.Resource.Health.ValueHealth", "체력 현재치");
		}

		namespace Mana
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxMana, "Arcanum.Resource.Mana.MaxMana", "마나 최대치");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ValueMana, "Arcanum.Resource.Mana.ValueMana", "마나 현재치");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen, "Arcanum.Resource.Mana.Regen", "마나 회복량");
		}

		namespace Supply
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxSupply, "Arcanum.Resource.Supply.MaxSupply", "보급 최대치");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ValueSupply, "Arcanum.Resource.Supply.ValueSupply", "보급 현재치");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen, "Arcanum.Resource.Supply.Regen", "보급 회복량");
		}
	}

	namespace Combat
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Combat.Root", "전투 루트");

		namespace State
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Alive, "Arcanum.Combat.State.Alive", "살아있음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Dead, "Arcanum.Combat.State.Dead", "죽음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stunned, "Arcanum.Combat.State.Stunned", "스턴");
		}
	}

	namespace Unit
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Root", "유닛 루트");

		namespace Faction
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ally, "Arcanum.Unit.Faction.Ally", "아군");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy, "Arcanum.Unit.Faction.Enemy", "적군");
		}

		namespace Enhance
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enhanced, "Arcanum.Unit.Enhance.Enhanced", "강화됨");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Max, "Arcanum.Unit.Enhance.Max", "최대 강화");
		}

		namespace Role
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tank, "Arcanum.Unit.Role.Tank", "방어");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Melee, "Arcanum.Unit.Role.Melee", "근딜");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ranged, "Arcanum.Unit.Role.Ranged", "원딜");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(AoEDPS, "Arcanum.Unit.Role.AoEDPS", "광딜");
		}

		namespace SpawnType
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Manual, "Arcanum.Unit.SpawnType.Manual", "수동 배치 (아군)");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Auto, "Arcanum.Unit.SpawnType.Auto", "자동 스폰 (아군)");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Wave, "Arcanum.Unit.SpawnType.Wave", "웨이브 스폰 (적군)");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Summon, "Arcanum.Unit.SpawnType.Summon", "소환 (적군)");
		}

		namespace EnemyType
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Runner, "Arcanum.Unit.EnemyType.Runner", "일반형: 러너");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(BigHeavy, "Arcanum.Unit.EnemyType.BigHeavy", "일반형: 빅헤비");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shooter, "Arcanum.Unit.EnemyType.Shooter", "일반형: 슈터");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Spawner, "Arcanum.Unit.EnemyType.Spawner", "일반형: 스포너");

			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Buffer, "Arcanum.Unit.EnemyType.Buffer", "보스형: 버퍼");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Assassin, "Arcanum.Unit.EnemyType.Assassin", "보스형: 암살자");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Area, "Arcanum.Unit.EnemyType.Area", "보스형: 광역");
		}
	}

	namespace AI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.AI.Root", "AI 루트");

		namespace TargetPriority
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Nearest, "Arcanum.AI.TargetPriority.Nearest", "가장 가까운 적");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Weakest, "Arcanum.AI.TargetPriority.Weakest", "HP 낮은 적");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Strongest, "Arcanum.AI.TargetPriority.Strongest", "공격력 높은 적");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boss, "Arcanum.AI.TargetPriority.Boss", "보스 우선");
		}
	}

	namespace Battle
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Battle.Root", "전투 루트");

		namespace Mode
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Manual, "Arcanum.Battle.Mode.Manual", "수동 조작");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Auto, "Arcanum.Battle.Mode.Auto", "자동 전투");
		}

		namespace State
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Active, "Arcanum.Battle.State.Active", "전투 중");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Paused, "Arcanum.Battle.State.Paused", "일시정지");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ended, "Arcanum.Battle.State.Ended", "전투 종료");
		}
	}
}
