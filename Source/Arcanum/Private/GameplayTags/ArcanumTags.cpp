#include "GameplayTags/ArcanumTags.h"

// 김유진
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

// 추영호
namespace Arcanum
{
	/* 상태 */
	namespace State 
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(PreAwakened, "Arcanum.State.PreAwakened", "각성 전");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Awakened, "Arcanum.State.Awakened", "각성 후");
		/* 각성 오라 */
		namespace Aura
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(FuryActive, "Arcanum.State.Aura.FuryActive", "분노 오라 활성");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ResolveActive, "Arcanum.State.Aura.ResolveActive", "결의 오라 활성");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ArcaneActive, "Arcanum.State.Aura.ArcaneActive", "비전 오라 활성");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(FocusActive, "Arcanum.State.Aura.FocusActive", "집중 오라 활성");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HolyActive, "Arcanum.State.Aura.HolyActive", "신성 오라 활성");
		}
	}
	/* 아이템 */
	namespace Item 
	{
		/* 장비 */
		namespace Equipment 
		{
			namespace Weapon 
			{
				namespace Common 
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(GreatSword, "Arcanum.Item.Equipment.Weapon.Common.GreatSword", "대검");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Staff, "Arcanum.Item.Equipment.Weapon.Common.Staff", "지팡이");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bow, "Arcanum.Item.Equipment.Weapon.Common.Bow", "활");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shield, "Arcanum.Item.Equipment.Weapon.Common.Shield", "방패");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Wand, "Arcanum.Item.Equipment.Weapon.Common.Wand", "완드");
				}
				namespace Legendary 
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Spear, "Arcanum.Item.Equipment.Weapon.Legendary.Spear", "번개창");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Blade, "Arcanum.Item.Equipment.Weapon.Legendary.Blade", "블레이드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scepter, "Arcanum.Item.Equipment.Weapon.Legendary.Scepter", "셉터");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scythe, "Arcanum.Item.Equipment.Weapon.Legendary.Scythe", "낫");
				}
			}
			namespace Armor
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Item.Equipment.Armor.Helmet", "투구");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Item.Equipment.Armor.Chest", "갑옷");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Item.Equipment.Armor.Glove", "장갑");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Item.Equipment.Armor.Boot", "신발");
			}
			namespace SetItem 
			{
				namespace Armor 
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Talasha, "Arcanum.Item.Equipment.SetItem.Armor.Talasha", "탈라샤 세트");
				}
			}
		}
		namespace SetBonus 
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Surge, "Arcanum.Item.SetBonus.Surge", "역류");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Majesty, "Arcanum.Item.SetBonus.Majesty", "위엄");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Avarice, "Arcanum.Item.SetBonus.Avarice", "탐욕");
		}
	}
	/* 능력치 */
	namespace Stat
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(BaseAttack, "Arcanum.Stat.BaseAttack", "기본 공격력");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SkillPower, "Arcanum.Stat.SkillPower", "스킬 공격력");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(LifeSteal, "Arcanum.Stat.LifeSteal", "생명력 흡수");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Stat.AttackSpeed", "공격 속도");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Evasion, "Arcanum.Stat.Evasion", "회피율");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CooldownReduction, "Arcanum.Stat.CooldownReduction", "쿨다운 감소");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(MoveSpeed, "Arcanum.Stat.MoveSpeed", "이동속도");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CritChance, "Arcanum.Stat.CritChance", "치명타 확률");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Spirit, "Arcanum.Stat.Spirit", "정신력");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mana, "Arcanum.Stat.Mana", "마나");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Health, "Arcanum.Stat.Health", "체력");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageReduction, "Arcanum.Stat.DamageReduction", "피해 감소");
	}
	/* 행동 */
	namespace Ability 
	{
		namespace Passive 
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fury, "Arcanum.Ability.Passive.Fury", "분노");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resolve, "Arcanum.Ability.Passive.Resolve", "결의");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Arcane, "Arcanum.Ability.Passive.Arcane", "비전");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Focus, "Arcanum.Ability.Passive.Focus", "집중");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Holy, "Arcanum.Ability.Passive.Holy", "신성");
		}
		/* 스킬 */
		namespace Skill
		{
			namespace Weapon
			{
				namespace Common
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slash, "Arcanum.Ability.Skill.Weapon.Common.Slash", "베기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MissileArc, "Arcanum.Ability.Skill.Weapon.Common.MissileArc", "궤적 미사일");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(PiercingShot, "Arcanum.Ability.Skill.Weapon.Common.PiercingShot", "관통 사격");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShockWave, "Arcanum.Ability.Skill.Weapon.Common.ShockWave", "충격파");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Penance, "Arcanum.Ability.Skill.Weapon.Common.Penance", "참회");
				}
				namespace Legendary
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MjolnirStrike, "Arcanum.Ability.Skill.Weapon.Legendary.MjolnirStrike", "묠니르");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(VoidRift, "Arcanum.Ability.Skill.Weapon.Legendary.VoidRift", "보이드 블레이드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(SolarFlare, "Arcanum.Ability.Skill.Weapon.Legendary.SolarFlare", "솔라");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Lantern, "Arcanum.Ability.Skill.Weapon.Legendary.Lantern", "랜턴");
				}
			}
		}
		/* 발동 */
		namespace Event 
		{
			namespace Trigger
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(BloodBurst, "Arcanum.Ability.Event.Trigger.BloodBurst", "혈폭");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Retribution, "Arcanum.Ability.Event.Trigger.Retribution", "응징");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Collapse, "Arcanum.Ability.Event.Trigger.Collapse", "붕괴");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Execution, "Arcanum.Ability.Event.Trigger.Execution", "처단");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Purification, "Arcanum.Ability.Event.Trigger.Purification", "정화");
			}
		}
	}
	/* 효과 */
	namespace Effect
	{
		namespace Buff
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Effect.Buff.AttackSpeed", "공격속도 증가");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageReduction, "Arcanum.Effect.Buff.DamageReduction", "피해 감소");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(SkillHaste, "Arcanum.Effect.Buff.SkillHaste", "스킬 가속");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(CritChance, "Arcanum.Effect.Buff.CritChance", "크리 증가");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthRegen, "Arcanum.Effect.Buff.HealthRegen", "지속 회복");
		}
		namespace Debuff
		{
			namespace Status
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bleed, "Arcanum.Effect.Debuff.Status.Bleed", "출혈");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Thorns, "Arcanum.Effect.Debuff.Status.Thorns", "가시");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resonance, "Arcanum.Effect.Debuff.Status.Resonance", "공명");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mark, "Arcanum.Effect.Debuff.Status.Mark", "표식");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Atonement, "Arcanum.Effect.Debuff.Status.Atonement", "속죄");

				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shock, "Arcanum.Effect.Debuff.Status.Shock", "감전");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Burn, "Arcanum.Effect.Debuff.Status.Burn", "화상");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fear, "Arcanum.Effect.Debuff.Status.Fear", "공포");
			}
			namespace Modifier
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(MoveSpeed, "Arcanum.Effect.Debuff.Modifier.MoveSpeed", "이동속도 감소");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Effect.Debuff.Modifier.AttackSpeed", "공격속도 감소");
			}
		}
	}
}

// 김도현
namespace Arcanum
{
	namespace PlayerTags
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData", "플레이어가 가지고 있어야할 정보의 모음");
		namespace Currencies // 재화들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies", "플레이어가 가지고 있는 재화들");
			namespace Gold // 골드
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.Gold", "골드");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Currencies.Gold.Value", "골드 현재값");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.PlayerData.Currencies.Gold.MaxValue", "골드 최대값");
			}
			namespace Shard // 조각
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.Shard", "조각");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Currencies.Shard.Value", "조각 현재값");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.PlayerData.Currencies.Shard.MaxValue", "조각 최대값");
			}
			namespace Soul // 소울
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.Soul", "소울");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Currencies.Soul.Value", "소울 현재값");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.PlayerData.Currencies.Soul.MaxValue", "소울 최대값");
			}
		}

		namespace Inventories
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Inventories", "현재 플레이어가 가지고 있는 것들");
			namespace ConsumedItems // 사용 아이템들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Inventories.ConsumedItems", "사용 아이템들");
			}

			namespace Weapons // 무기들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Inventories.Weapons", "무기들");
			}

			namespace Equipments // 장비들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Inventories.Equipments", "장비들");
			}

			namespace Heroes // 영웅들(플레이어 캐릭터)
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Inventories.Heroes", "영웅들");
			}

			namespace Units // 아군 유닛들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Inventories.Units", "아군 유닛들");
			}
		}

		namespace StageInfo // 클리어한 스테이지 정보
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.StageInfo", "클리어한 스테이지 정보") // 나중에 구조체에서는 배열같은 값을 불러올 것 같습니다
		}

		namespace LastEquips // 마지막으로 장착한 객체들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.LastEquips", "마지막으로 장착한 객체들(게임시작시 불러옴)");
			namespace Hero // 영웅(플레이어 캐릭터)
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.LastEquips.Hero", "영웅(플레이어 캐릭터)");
			}

			namespace ConsumedItems // 사용 아이템들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.LastEquips.ConsumedItems", "사용 아이테들");
			}

			namespace Equipments // 장비들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.LastEquips.Equipments", "장비들");
			}

			namespace Weapons // 무기들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.LastEquips.Weapons", "무기들");
			}
		}
	}
}
