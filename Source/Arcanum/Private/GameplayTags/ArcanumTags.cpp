
#include "GameplayTags/ArcanumTags.h"

namespace Arcanum
{
	/* 상태 */
	namespace State	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(PreAwakened, "Arcanum.State.PreAwakened", "각성 전");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Awakened, "Arcanum.State.Awakened", "각성 후");
		/* 각성 오라 */
		namespace Aura {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(FuryActive, "Arcanum.State.Aura.FuryActive", "분노 오라 활성");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ResolveActive, "Arcanum.State.Aura.ResolveActive", "결의 오라 활성");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ArcaneActive, "Arcanum.State.Aura.ArcaneActive", "비전 오라 활성");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(FocusActive, "Arcanum.State.Aura.FocusActive", "집중 오라 활성");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HolyActive, "Arcanum.State.Aura.HolyActive", "신성 오라 활성");
		}
	}
	/* 아이템 */
	namespace Item {
		/* 장비 */
		namespace Equipment	{
			namespace Weapon {
				namespace Common {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(GreatSword, "Arcanum.Item.Equipment.Weapon.Common.GreatSword", "대검");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Staff, "Arcanum.Item.Equipment.Weapon.Common.Staff", "지팡이");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bow, "Arcanum.Item.Equipment.Weapon.Common.Bow", "활");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shield, "Arcanum.Item.Equipment.Weapon.Common.Shield", "방패");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Wand, "Arcanum.Item.Equipment.Weapon.Common.Wand", "완드");
				}
				namespace Legendary	{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Spear, "Arcanum.Item.Equipment.Weapon.Legendary.Spear", "번개창");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Blade, "Arcanum.Item.Equipment.Weapon.Legendary.Blade", "블레이드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scepter, "Arcanum.Item.Equipment.Weapon.Legendary.Scepter", "셉터");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scythe, "Arcanum.Item.Equipment.Weapon.Legendary.Scythe", "낫");
				}
			}
			namespace Armor	{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Item.Equipment.Armor.Helmet", "투구");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Item.Equipment.Armor.Chest", "갑옷");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Item.Equipment.Armor.Glove", "장갑");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Item.Equipment.Armor.Boot", "신발");
			}
			namespace SetItem {
				namespace Armor	{
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Talasha, "Arcanum.Item.Equipment.SetItem.Armor.Talasha", "탈라샤 세트");
				}
			}
		}
		namespace SetBonus {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Surge, "Arcanum.Item.SetBonus.Surge", "역류");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Majesty, "Arcanum.Item.SetBonus.Majesty", "위엄");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Avarice, "Arcanum.Item.SetBonus.Avarice", "탐욕");
		}
	}
	/* 행동 */
	namespace Ability {
		namespace Passive {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fury, "Arcanum.Ability.Passive.Fury", "분노");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resolve, "Arcanum.Ability.Passive.Resolve", "결의");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Arcane, "Arcanum.Ability.Passive.Arcane", "비전");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Focus, "Arcanum.Ability.Passive.Focus", "집중");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Holy, "Arcanum.Ability.Passive.Holy", "신성");
		}
		/* 스킬 */
		namespace Skill {
			namespace Weapon {
				namespace Common {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slash, "Arcanum.Ability.Skill.Weapon.Common.Slash", "베기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MissileArc, "Arcanum.Ability.Skill.Weapon.Common.MissileArc", "궤적 미사일");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(PiercingShot, "Arcanum.Ability.Skill.Weapon.Common.PiercingShot", "관통 사격");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShockWave, "Arcanum.Ability.Skill.Weapon.Common.ShockWave", "충격파");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Penance, "Arcanum.Ability.Skill.Weapon.Common.Penance", "참회");
				}
				namespace Legendary {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MjolnirStrike, "Arcanum.Ability.Skill.Weapon.Legendary.MjolnirStrike", "묠니르");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(VoidRift, "Arcanum.Ability.Skill.Weapon.Legendary.VoidRift", "보이드 블레이드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(SolarFlare, "Arcanum.Ability.Skill.Weapon.Legendary.SolarFlare", "솔라");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Lantern, "Arcanum.Ability.Skill.Weapon.Legendary.Lantern", "랜턴");
				}
			}
		}
		/* 발동 */
		namespace Event {
			namespace Trigger {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(BloodBurst, "Arcanum.Ability.Event.Trigger.BloodBurst", "혈폭");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Retribution, "Arcanum.Ability.Event.Trigger.Retribution", "응징");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Collapse, "Arcanum.Ability.Event.Trigger.Collapse", "붕괴");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Execution, "Arcanum.Ability.Event.Trigger.Execution", "처단");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Purification, "Arcanum.Ability.Event.Trigger.Purification", "정화");
			}
		}
	}
	/* 효과 */
	namespace Effect {
		namespace Buff {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Effect.Buff.AttackSpeed", "공격속도 증가");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageReduction, "Arcanum.Effect.Buff.DamageReduction", "피해 감소");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(SkillHaste, "Arcanum.Effect.Buff.SkillHaste", "스킬 가속");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(CritChance, "Arcanum.Effect.Buff.CritChance", "크리 증가");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthRegen, "Arcanum.Effect.Buff.HealthRegen", "지속 회복");
		}
		namespace Debuff {
			namespace Status {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bleed, "Arcanum.Effect.Debuff.Status.Bleed", "출혈");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Thorns, "Arcanum.Effect.Debuff.Status.Thorns", "가시");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Resonance, "Arcanum.Effect.Debuff.Status.Resonance", "공명");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mark, "Arcanum.Effect.Debuff.Status.Mark", "표식");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Atonement, "Arcanum.Effect.Debuff.Status.Atonement", "속죄");

				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shock, "Arcanum.Effect.Debuff.Status.Shock", "감전");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Burn, "Arcanum.Effect.Debuff.Status.Burn", "화상");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Fear, "Arcanum.Effect.Debuff.Status.Fear", "공포");
			}
			namespace Modifier {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(MoveSpeed, "Arcanum.Effect.Debuff.Modifier.MoveSpeed", "이동속도 감소");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Effect.Debuff.Modifier.AttackSpeed", "공격속도 감소");
			}
		}
	}
}

