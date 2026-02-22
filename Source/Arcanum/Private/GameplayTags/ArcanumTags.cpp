
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
			// 아군
			namespace Ally {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Ally", "아군유닛");
				namespace Army {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Ally.Army", "아미");
				}
				namespace Bard {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Ally.Bard", "바드");
				}
				namespace Maid {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Ally.Maid", "메이드");
				}
				namespace Tinker {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Ally.Tinker", "팅커");
				}
			}
			// 적군
			namespace Enemy {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy", "적군유닛");
				namespace Macaron {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.Macaron", "마카롱");
				}
				namespace Leech {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.Leech", "리치");
				}
				namespace GhostMacaron {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.GhostMacaron", "유령 마카롱");
				}
				namespace Bat {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.Bat", "배트");
				}
				namespace Cell {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.Cell", "셀");
				}
				namespace Mask {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.Mask", "마스크");
				}
				namespace Wolfron {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.Wolfron", "울프론");
				}
				namespace MageMacaron {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.MageMacaron", "마법사 마카롱");
				}
				namespace Felice {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.Felice", "펠리스");
				}
				namespace CupFelice {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Unit.Enemy.CupFelice", "컵 펠리스");
				}
			}
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

//namespace Arcanum
//{
//	/* 상태 */
//	namespace State
//	{
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(PreAwakened, "Arcanum.State.PreAwakened", "각성 전");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Awakened, "Arcanum.State.Awakened", "각성 후");
//		/* 각성 오라 */
//		namespace Aura
//		{
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(FuryActive, "Arcanum.State.Aura.FuryActive", "분노 오라 활성");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ResolveActive, "Arcanum.State.Aura.ResolveActive", "결의 오라 활성");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(FocusActive, "Arcanum.State.Aura.FocusActive", "집중 오라 활성");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HolyActive, "Arcanum.State.Aura.HolyActive", "신성 오라 활성");
//		}
//	}
//	/* 아이템 */
//	namespace Item
//	{
//		/* 장비 */
//		namespace Equipment
//		{
//			namespace Weapon
//			{
//				namespace Common
//				{
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(GreatSword, "Arcanum.Item.Equipment.Weapon.Common.GreatSword", "대검");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Staff, "Arcanum.Item.Equipment.Weapon.Common.Staff", "지팡이");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bow, "Arcanum.Item.Equipment.Weapon.Common.Bow", "활");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shield, "Arcanum.Item.Equipment.Weapon.Common.Shield", "방패");
//				}
//				namespace Legendary
//				{
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Spear, "Arcanum.Item.Equipment.Weapon.Legendary.Spear", "창");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Blade, "Arcanum.Item.Equipment.Weapon.Legendary.Blade", "블레이드");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scepter, "Arcanum.Item.Equipment.Weapon.Legendary.Scepter", "셉터");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scythe, "Arcanum.Item.Equipment.Weapon.Legendary.Scythe", "낫");
//
//				}
//			}
//			namespace Armor
//			{
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Item.Equipment.Armor.Helmet", "투구");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Item.Equipment.Armor.Chest", "갑옷");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Item.Equipment.Armor.Glove", "장갑");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Item.Equipment.Armor.Boot", "신발");
//			}
//			namespace SetItem
//			{
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(ManaCycle, "Arcanum.Item.Equipment.SetItem.ManaCycle", "세트 아이템 : 마력 순환");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemEffectBonus, "Arcanum.Item.Equipment.SetItem.ItemEffectBonus", "세트 아이템 : 아이템 효과 증가");
//
//				namespace ManaCycle
//				{
//					namespace Armor
//					{
//						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Item.Equipment.SetItem.ManaCycle.Armor.Helmet", "마력 순환 세트 방어구: 투구");
//						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Item.Equipment.SetItem.ManaCycle.Armor.Chest", "마력 순환 세트 방어구: 갑옷");
//						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Item.Equipment.SetItem.ManaCycle.Armor.Glove", "마력 순환 세트 방어구: 장갑");
//						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Item.Equipment.SetItem.ManaCycle.Armor.Boot", "마력 순환 세트 방어구: 신발");
//					}
//				}
//
//				namespace ItemEffectBonus
//				{
//					namespace Armor
//					{
//						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Item.Equipment.SetItem.ItemEffectBonus.Armor.Helmet", "아이템 효과 증가 세트 방어구: 투구");
//						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Item.Equipment.SetItem.ItemEffectBonus.Armor.Chest", "아이템 효과 증가 세트 방어구: 갑옷");
//						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Item.Equipment.SetItem.ItemEffectBonus.Armor.Glove", "아이템 효과 증가 세트 방어구: 장갑");
//						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Item.Equipment.SetItem.ItemEffectBonus.Armor.Boot", "아이템 효과 증가 세트 방어구: 신발");
//					}
//				}
//			}
//			namespace SetBonus
//			{AY_TAG_EXTERN(ItemEffectBonus); // 아이템 효과 
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(ManaCycle, "Arcanum.Item.SetBonus.ManaCycle", "마나 순환");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemEffectBonus, "Arcanum.Item.SetBonus.ItemEffectBonus", "아이템 효과 증가");
//			}
//		}
//	}
//	/* 행동 */
//	namespace Ability 
//	{
//		namespace Attack
//		{
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(GreatSword, "Arcanum.Ability.Attack.GreatSword", "대검 평타");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Staff, "Arcanum.Ability.Attack.Staff", "지팡이 평타");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bow, "Arcanum.Ability.Attack.Bow", "활 평타");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shield, "Arcanum.Ability.Attack.Shield", "방패 평타");
//		}
//		namespace Passive {
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fury, "Arcanum.Ability.Passive.Fury", "분노");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resolve, "Arcanum.Ability.Passive.Resolve", "결의");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Focus, "Arcanum.Ability.Passive.Focus", "집중");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Holy, "Arcanum.Ability.Passive.Holy", "신성");
//		}
//		/* 스킬 */
//		namespace Skill
//		{
//			namespace Weapon
//			{
//				namespace Common
//				{
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slash, "Arcanum.Ability.Skill.Weapon.Common.Slash", "베기");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(ChainBolt, "Arcanum.Ability.Skill.Weapon.Common.ChainBolt", "체인 미사일");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(PiercingShot, "Arcanum.Ability.Skill.Weapon.Common.PiercingShot", "관통 사격");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShockWave, "Arcanum.Ability.Skill.Weapon.Common.ShockWave", "충격파");
//				}
//				namespace Legendary
//				{
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MjolnirStrike, "Arcanum.Ability.Skill.Weapon.Legendary.MjolnirStrike", "묠니르");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(VoidEclipse, "Arcanum.Ability.Skill.Weapon.Legendary.VoidEclipse", "보이드");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(SolarFlare, "Arcanum.Ability.Skill.Weapon.Legendary.SolarFlare", "솔라");
//				}
//			}
//		}
//		/* 발동 */
//		namespace Event
//		{
//			namespace Trigger
//			{
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(BloodBurst, "Arcanum.Ability.Event.Trigger.BloodBurst", "혈폭");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Retribution, "Arcanum.Ability.Event.Trigger.Retribution", "응보");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Execution, "Arcanum.Ability.Event.Trigger.Execution", "처단");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Purification, "Arcanum.Ability.Event.Trigger.Purification", "정화");
//			}
//		}
//	}
//	/* 효과 타입 */
//	namespace EffectType
//	{
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "Arcanum.EffectType.Damage", "효과 타입: 데미지");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Heal, "Arcanum.EffectType.Heal", "효과 타입: 회복");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(AddBarrier, "Arcanum.EffectType.AddBarrier", "효과 타입: 보호막 추가");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SpawnArea, "Arcanum.EffectType.SpawnArea", "효과 타입: 장판/영역 생성");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ApplyDebuff, "Arcanum.EffectType.ApplyDebuff", "효과 타입: 디버프 적용");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ResetDebuff, "Arcanum.EffectType.ResetDebuff", "효과 타입: 디버프 리셋");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ModifyStat, "Arcanum.EffectType.ModifyStat", "효과 타입: 스탯 수정");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(AddGauge, "Arcanum.EffectType.AddGauge", "효과 타입: 게이지 증가");
//		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Pull, "Arcanum.EffectType.Pull", "효과 타입: 끌어 당김");
//	}
//	/* 효과 */
//	namespace Effect
//	{
//		namespace Buff
//		{
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Effect.Buff.AttackSpeed", "공격속도 증가");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageReduction, "Arcanum.Effect.Buff.DamageReduction", "피해 감소");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(CritChance, "Arcanum.Effect.Buff.CritChance", "크리 증가");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthRegen, "Arcanum.Effect.Buff.HealthRegen", "지속 회복");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Barrier, "Arcanum.Effect.Buff.Barrier", "보호막");
//		}
//		namespace Debuff
//		{
//			namespace Status
//			{
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bleed, "Arcanum.Effect.Debuff.Status.Bleed", "출혈");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Guard, "Arcanum.Effect.Debuff.Status.Guard", "수호");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mark, "Arcanum.Effect.Debuff.Status.Mark", "표식");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Atonement, "Arcanum.Effect.Debuff.Status.Atonement", "속죄");
//
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shock, "Arcanum.Effect.Debuff.Status.Shock", "감전");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Burn, "Arcanum.Effect.Debuff.Status.Burn", "화상");
//			}
//			namespace Modifier
//			{
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(MoveSpeed, "Arcanum.Effect.Debuff.Modifier.MoveSpeed", "둔화");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Effect.Debuff.Modifier.AttackSpeed", "쇠약");
//			}
//			namespace CrowdControl
//			{
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stun, "Arcanum.Effect.Debuff.CrowdControl.Stun", "스턴");
//				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Knockback, "Arcanum.Effect.Debuff.CrowdControl.Knockback", "넉백");
//				namespace Modifier {
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MoveSpeed, "Arcanum.Effect.Debuff.Modifier.MoveSpeed", "이동속도 감소");
//					UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Effect.Debuff.Modifier.AttackSpeed", "공격속도 감소");
//				}
//			}
//		}
//		/* 타겟 타입 */
//		namespace TargetType
//		{
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Self, "Arcanum.TargetType.Self", "자기 자신");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Target, "Arcanum.TargetType.Target", "직접 타겟");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(AlliesInRadius, "Arcanum.TargetType.AlliesInRadius", "범위 내 아군");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(EnemiesInRadius, "Arcanum.TargetType.EnemiesInRadius", "범위 내 적");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(BothInRadius, "Arcanum.TargetType.BothInRadius", "범위 내 전체");
//		}
//
//		/* 타겟 필터 */
//		namespace TargetFilter
//		{
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ExcludeAllies, "Arcanum.TargetFilter.ExcludeAllies", "아군 제외");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(PlayersOnly, "Arcanum.TargetFilter.PlayersOnly", "플레이어만");
//			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ExcludePlayers, "Arcanum.TargetFilter.ExcludePlayers", "플레이어 제외");
//		}
//	}
//}


// 김도현
namespace Arcanum
{
	namespace SaveData
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.SaveData", "저장 데이터 모음")
			namespace PlayerDataTags
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.SaveData.PlayerData", "플레이어가 가지고 있어야할 정보의 모음");
			namespace Currencies // 재화들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.SaveData.PlayerData.Currencies", "플레이어가 가지고 있는 재화들");
				namespace Gold // 골드
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.SaveData.PlayerData.Currencies.Gold", "골드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.Currencies.Gold.Value", "골드 현재값");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.SaveData.PlayerData.Currencies.Gold.MaxValue", "골드 최대값");
				}
				namespace Shard // 조각
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.SaveData.PlayerData.Currencies.Shard", "조각");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.Currencies.Shard.Value", "조각 현재값");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.SaveData.PlayerData.Currencies.Shard.MaxValue", "조각 최대값");
				}
				namespace Soul // 소울
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.SaveData.PlayerData.Currencies.Soul", "소울");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.Currencies.Soul.Value", "소울 현재값");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.SaveData.PlayerData.Currencies.Soul.MaxValue", "소울 최대값");
				}
			}

			namespace Inventories
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.SaveData.PlayerData.Inventories", "현재 플레이어가 가지고 있는 것들");
				namespace ConsumedItems // 사용 아이템들
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.Inventories.ConsumedItems", "사용 아이템들");
				}

				namespace Weapons // 무기들
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.Inventories.Weapons", "무기들");
				}

				namespace Equipments // 장비들
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.Inventories.Equipments", "장비들");
				}

				namespace Heroes // 영웅들(플레이어 캐릭터)
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.Inventories.Heroes", "영웅들");
				}

				namespace Units // 아군 유닛들
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.Inventories.Units", "아군 유닛들");
				}
			}

			namespace StageInfo // 클리어한 스테이지 정보
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.StageInfo", "클리어한 스테이지 정보") // 나중에 구조체에서는 배열같은 값을 불러올 것 같습니다
			}

			namespace LastEquips // 마지막으로 장착한 객체들
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.SaveData.PlayerData.LastEquips", "마지막으로 장착한 객체들(게임시작시 불러옴)");
				namespace Hero // 영웅(플레이어 캐릭터)
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.LastEquips.Hero", "영웅(플레이어 캐릭터)");
				}

				namespace ConsumedItems // 사용 아이템들
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.LastEquips.ConsumedItems", "사용 아이테들");
				}

				namespace Equipments // 장비들
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.LastEquips.Equipments", "장비들");
				}

				namespace Weapons // 무기들
				{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.SaveData.PlayerData.LastEquips.Weapons", "무기들");
				}
			}
		}
	}

	// 고정 데이터
	namespace StaticData
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.StaticData", "고정 데이터");

		namespace Stage
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.StaticData.Stage", "스테이지");

			// 테스트 스테이지
			namespace TestStage01
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.StaticData.Stage.TestStage01", "테스트용 스테이지 태그");
			}
		}
	}
}


namespace Arcanum {
	namespace Skills {
		namespace SkillName {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.SkillName.None", "Null");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slash, "Arcanum.Skills.SkillName.Slash", "베기");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ChainBolt, "Arcanum.Skills.SkillName.ChainBolt", "체인 볼트");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(PiercingShot, "Arcanum.Skills.SkillName.PiercingShot", "관통 사격");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShockWave, "Arcanum.Skills.SkillName.ShockWave", "충격파");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(VoidEclipse, "Arcanum.Skills.SkillName.VoidEclipse", "보이드");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(SolarFlare, "Arcanum.Skills.SkillName.SolarFlare", "솔라");
		}
		namespace BasicAttack {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.BasicAttack.None", "Null");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(GreatSword, "Arcanum.Skills.BasicAttack.GreatSword", "기본 공격: 대검");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shield, "Arcanum.Skills.BasicAttack.Shield", "기본 공격: 방패");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Staff, "Arcanum.Skills.BasicAttack.Staff", "기본 공격: 지팡이");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bow, "Arcanum.Skills.BasicAttack.Bow", "기본 공격: 활");
		}
		namespace Awaken {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.Awaken.None", "Null");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fury, "Arcanum.Skills.Awaken.Fury", "각성 오라: 분노");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Focus, "Arcanum.Skills.Awaken.Focus", "각성 오라: 집중");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Holy, "Arcanum.Skills.Awaken.Holy", "각성 오라: 신성");
		}
		namespace Trigger {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.Trigger.None", "Null");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(BloodBurst, "Arcanum.Skills.Trigger.BloodBurst", "트리거: 혈폭");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Execution, "Arcanum.Skills.Trigger.Execution", "트리거: 처단");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Purification, "Arcanum.Skills.Trigger.Purification", "트리거: 정화");
		}
		namespace SkillTypes {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None,	"Arcanum.Skills.SkillTypes.None", "Null");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Active,	"Arcanum.Skills.SkillTypes.Active", "액티브 스킬");		// 실제 필드에 보일 애들
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Passive, "Arcanum.Skills.SkillTypes.Passive", "패시브 스킬");		// 캐릭터 내부에서만 작동하는 애들
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Aura,	"Arcanum.Skills.SkillTypes.Aura", "오라");			    // 실제 외부에 나타나는 Passive 계열
		}
		namespace TargetingType {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None,		"Arcanum.Skills.TargetingType.None", "타겟 없음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Self,		"Arcanum.Skills.TargetingType.Self", "자기 자신");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Unit,		"Arcanum.Skills.TargetingType.Unit", "유닛 선택");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ground,		"Arcanum.Skills.TargetingType.Ground", "지면 위치 지정");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Direction,	"Arcanum.Skills.TargetingType.Direction", "방향 지정");
		}
		namespace TargetFilter {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None,	"Arcanum.Skills.TargetFilter.None", "제한 없음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy,	"Arcanum.Skills.TargetFilter.Enemy", "적 대상");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ally,	"Arcanum.Skills.TargetFilter.Ally", "아군 대상");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Self,	"Arcanum.Skills.TargetFilter.Self", "자기 자신");
		}
		namespace SkillVFX { // 이펙트 효과용 0을 기준으로 +버프, -디버프 => 추후 스킬컴포넌트에서 스택도 관리함
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None,			"Arcanum.Skills.SkillVFX.None",				"없음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bleed,			"Arcanum.Skills.SkillVFX.Bleed",			"출혈");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Aegis,			"Arcanum.Skills.SkillVFX.Aegis",			"수호");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mark,			"Arcanum.Skills.SkillVFX.Mark",				"표식");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shock,			"Arcanum.Skills.SkillVFX.Shock",			"감전");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Burn,			"Arcanum.Skills.SkillVFX.Burn",				"화상");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stun,			"Arcanum.Skills.SkillVFX.Stun",				"스턴");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Knockback,		"Arcanum.Skills.SkillVFX.Knockback",		"넉백");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MoveSpeed,		"Arcanum.Skills.SkillVFX.MoveSpeed",		"이동속도");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed,		"Arcanum.Skills.SkillVFX.AttackSpeed",		"공격속도");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageReduction, "Arcanum.Skills.SkillVFX.DamageReduction",	"피해");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(CritChance,		"Arcanum.Skills.SkillVFX.CritChance",		"크리 증가");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthRegen,		"Arcanum.Skills.SkillVFX.HealthRegen",		"지속 회복");
		}
	}
	namespace Items {
		namespace Weapon {
			namespace Common {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(GreatSword, "Arcanum.Items.Weapon.Common.GreatSword", "일반 무기: 대검");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Staff, "Arcanum.Items.Weapon.Common.Staff", "일반 무기: 지팡이");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bow, "Arcanum.Items.Weapon.Common.Bow", "일반 무기: 활");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shield, "Arcanum.Items.Weapon.Common.Shield", "일반 무기: 방패");
			}
			namespace Legendary {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scepter, "Arcanum.Items.Weapon.Legendary.Scepter", "전설 무기: 셉터");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scythe, "Arcanum.Items.Weapon.Legendary.Scythe", "전설 무기: 낫");
			}
		}
		namespace Armor {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Items.Armor.Helmet", "일반 방어구: 투구");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Items.Armor.Chest", "일반 방어구: 갑옷");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Items.Armor.Glove", "일반 방어구: 장갑");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Items.Armor.Boot", "일반 방어구: 신발");
		}
		namespace SetItem {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ManaCycle, "Arcanum.Items.SetItem.ManaCycle", "세트: 마력 순환 4셋");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ItemEffectBonus, "Arcanum.Items.SetItem.ItemEffectBonus", "세트: 아이템 효과 증가 4셋");

			namespace ManaCycleSet	{
				namespace Armor	{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Items.SetItem.ManaCycle.Armor.Helmet", "마력 순환(세트): 투구");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Items.SetItem.ManaCycle.Armor.Chest", "마력 순환(세트): 갑옷");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Items.SetItem.ManaCycle.Armor.Glove", "마력 순환(세트): 장갑");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Items.SetItem.ManaCycle.Armor.Boot", "마력 순환(세트): 신발");
				}
			}
			namespace ItemEffectBonusSet {
				namespace Armor {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Items.SetItem.ItemEffectBonus.Armor.Helmet", "아이템 효과 증가(세트): 투구");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Items.SetItem.ItemEffectBonus.Armor.Chest", "아이템 효과 증가(세트): 갑옷");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Items.SetItem.ItemEffectBonus.Armor.Glove", "아이템 효과 증가(세트): 장갑");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Items.SetItem.ItemEffectBonus.Armor.Boot", "아이템 효과 증가(세트): 신발");
				}
			}
		}
	}
}

namespace Arcanum {
	namespace BattleStat {
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat", "배틀 스탯 루트");
		namespace Player {
			namespace Regen {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Player.Regen", "플레이어 리젠스탯 루트");
				namespace Meat {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Player.Regen.Meat", "고기 루트 태그");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Player.Regen.Meat.Value", "현재 고기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.BattleStat.Player.Regen.Meat.MaxValue", "최대 고기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen, "Arcanum.BattleStat.Player.Regen.Meat.Regen", "고기 재생량");
				}
			}
			namespace NonRegen {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Player.NonRegen", "플레이어 논리젠스탯 루트");
				namespace CommandCenterStamina {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Player.NonRegen.CommandCenterStamina", "기지 체력");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Player.NonRegen.CommandCenterStamina.Value", "기지 기본체력");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue, "Arcanum.BattleStat.Player.NonRegen.CommandCenterStamina.BonusValue", "기지 추가 체력");
				}
			}
		}

		namespace Character {
			namespace Regen {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.Regen", "캐릭터 리젠스탯 루트");
				namespace Health {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,	"Arcanum.BattleStat.Character.Regen.Health", "체력 루트 태그");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,	"Arcanum.BattleStat.Character.Regen.Health.Value", "현재 체력");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,"Arcanum.BattleStat.Character.Regen.Health.MaxValue", "최대 체력");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen,	"Arcanum.BattleStat.Character.Regen.Health.Regen", "체력 재생량");
				}
				namespace Mana {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,	"Arcanum.BattleStat.Character.Regen.Mana", "캐릭터 마나 루트 태그");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,	"Arcanum.BattleStat.Character.Regen.Mana.Value", "현재 마나");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,"Arcanum.BattleStat.Character.Regen.Mana.MaxValue", "최대 마나");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen,	"Arcanum.BattleStat.Character.Regen.Mana.Regen", "마나 재생량");
				}
			}
			namespace NonRegen {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.NonRegen", "캐릭터 논리젠스탯 루트");
				namespace AttackPower {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Arcanum.BattleStat.Character.NonRegen.AttackPower", "공격력 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Arcanum.BattleStat.Character.NonRegen.AttackPower.Value", "기본 공격력");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue,	"Arcanum.BattleStat.Character.NonRegen.AttackPower.BonusValue", "추가 공격력");
				}
				namespace AttackSpeed {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Arcanum.BattleStat.Character.NonRegen.AttackSpeed", "캐릭터 공격 속도 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Arcanum.BattleStat.Character.NonRegen.AttackSpeed.Value", "기본 공격 속도");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue,	"Arcanum.BattleStat.Character.NonRegen.AttackSpeed.BonusValue", "추가 공격 속도");
				}
				namespace Evasion {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Arcanum.BattleStat.Character.NonRegen.Evasion", "캐릭터 회피율 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Arcanum.BattleStat.Character.NonRegen.Evasion.Value", "기본 회피율");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue,	"Arcanum.BattleStat.Character.NonRegen.Evasion.BonusValue", "추가 회피율");
				}
				namespace MoveSpeed {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Arcanum.BattleStat.Character.NonRegen.MoveSpeed", "캐릭터 이동 속도 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Arcanum.BattleStat.Character.NonRegen.MoveSpeed.Value", "기본 이동 속도");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue,	"Arcanum.BattleStat.Character.NonRegen.MoveSpeed.BonusValue", "추가 이동 속도");
				}
				namespace CritChance {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Arcanum.BattleStat.Character.NonRegen.CritChance", "캐릭터 치명타 확률 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Arcanum.BattleStat.Character.NonRegen.CritChance.Value", "기본 치명타 확률");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue,	"Arcanum.BattleStat.Character.NonRegen.CritChance.BonusValue", "추가 치명타 확률");
				}
				namespace DamageReduction {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Arcanum.BattleStat.Character.NonRegen.DamageReduction", "캐릭터 피해 감소 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Arcanum.BattleStat.Character.NonRegen.DamageReduction.Value", "기본 피해 감소");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue,	"Arcanum.BattleStat.Character.NonRegen.DamageReduction.BonusValue", "추가 피해 감소");
				}
			}
		}
	}
	namespace PlayerData {
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData", "플레이어 데이터 루트");

		namespace Currencies {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies", "재화 루트");
			namespace Regen {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.Regen", "플레이어 리젠 재화 루트");
				namespace Supply {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,	"Arcanum.PlayerData.Currencies.Regen.Supply", "고기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,	"Arcanum.PlayerData.Currencies.Regen.Supply.Value", "현재 고기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,"Arcanum.PlayerData.Currencies.Regen.Supply.MaxValue", "최대 고기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen,	"Arcanum.PlayerData.Currencies.Regen.Supply.Regen", "고기 재생시간");
				}
			}
			namespace NonRegen {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.NonRegen", "플레이어 논리젠 재화 루트");
				namespace Gold {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,	"Arcanum.PlayerData.Currencies.NonRegen.Gold", "골드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,	"Arcanum.PlayerData.Currencies.NonRegen.Gold.Value", "현재 골드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,"Arcanum.PlayerData.Currencies.NonRegen.Gold.MaxValue", "최대 골드 보유량");
				}
				namespace Shard {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,	"Arcanum.PlayerData.Currencies.NonRegen.Shard", "샤드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,	"Arcanum.PlayerData.Currencies.NonRegen.Shard.Value", "현재 샤드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,"Arcanum.PlayerData.Currencies.NonRegen.Shard.MaxValue", "최대 샤드 보유량");
				}
				namespace Soul {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,	"Arcanum.PlayerData.Currencies.NonRegen.Soul", "소울");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,	"Arcanum.PlayerData.Currencies.NonRegen.Soul.Value", "현재 소울");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,"Arcanum.PlayerData.Currencies.NonRegen.Soul.MaxValue", "최대 소울 보유량");
				}
			}
		}
	}
}
namespace Arcanum {
	namespace UI {
		namespace LoginUI {
			namespace State {
				UE_DEFINE_GAMEPLAY_TAG(Idle,		"Arcanum.UI.LoginUI.State.Idle");
				UE_DEFINE_GAMEPLAY_TAG(Sync,		"Arcanum.UI.LoginUI.State.Sync");
				UE_DEFINE_GAMEPLAY_TAG(Announce,	"Arcanum.UI.LoginUI.State.Announce");
				UE_DEFINE_GAMEPLAY_TAG(Setting,		"Arcanum.UI.LoginUI.State.Setting");
				UE_DEFINE_GAMEPLAY_TAG(QuitGame,	"Arcanum.UI.LoginUI.State.QuitGame");
			}
			namespace BtnPage {
				UE_DEFINE_GAMEPLAY_TAG(None,		"Arcanum.UI.LoginUI.BtnPage.None");
				UE_DEFINE_GAMEPLAY_TAG(Announce,	"Arcanum.UI.LoginUI.BtnPage.Announce");
				UE_DEFINE_GAMEPLAY_TAG(Setting,		"Arcanum.UI.LoginUI.BtnPage.Setting");
				UE_DEFINE_GAMEPLAY_TAG(Play,		"Arcanum.UI.LoginUI.BtnPage.Play");
				UE_DEFINE_GAMEPLAY_TAG(QuitGame,	"Arcanum.UI.LoginUI.BtnPage.QuitGame");
			}
		}
	}
}