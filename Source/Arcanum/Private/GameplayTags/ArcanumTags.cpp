#include "GameplayTags/ArcanumTags.h"

namespace Arcanum {
	namespace DataTable {
		UE_DEFINE_GAMEPLAY_TAG(None, "Arcanum.DataTable.None");
		UE_DEFINE_GAMEPLAY_TAG(CharacterInfo, "Arcanum.DataTable.CharacterInfo");
		UE_DEFINE_GAMEPLAY_TAG(BattleStats, "Arcanum.DataTable.BattleStats");
		UE_DEFINE_GAMEPLAY_TAG(PlayerBattleStats, "Arcanum.DataTable.PlayerBattleStats");
		UE_DEFINE_GAMEPLAY_TAG(Equipment, "Arcanum.DataTable.Equipment");
		UE_DEFINE_GAMEPLAY_TAG(GachaTable, "Arcanum.DataTable.GachaTable");
		UE_DEFINE_GAMEPLAY_TAG(SkillData, "Arcanum.DataTable.SkillData");
		UE_DEFINE_GAMEPLAY_TAG(StageInfo, "Arcanum.DataTable.StageInfo");
		UE_DEFINE_GAMEPLAY_TAG(AllyUnitInfo, "Arcanum.DataTable.AllyUnitInfo");
		UE_DEFINE_GAMEPLAY_TAG(EnemyUnitInfo, "Arcanum.DataTable.EnemyUnitInfo");
		UE_DEFINE_GAMEPLAY_TAG(Potion, "Arcanum.DataTable.Potion");
		UE_DEFINE_GAMEPLAY_TAG(InventoryRule, "Arcanum.DataTable.InventoryRule");
		UE_DEFINE_GAMEPLAY_TAG(ItemCatalog, "Arcanum.DataTable.ItemCatalog");
		


		//UE_DEFINE_GAMEPLAY_TAG(BattleRegenStats,	"Arcanum.DataTable.BattleRegenStats");
		//UE_DEFINE_GAMEPLAY_TAG(BattleNonRegenStats, "Arcanum.DataTable.BattleNonRegenStats");
	}
}

namespace Arcanum {
	namespace BattleStage {
		namespace Normal {
			UE_DEFINE_GAMEPLAY_TAG(Stage1, "Arcanum.BattleStage.Normal.Stage1");
			UE_DEFINE_GAMEPLAY_TAG(Stage2, "Arcanum.BattleStage.Normal.Stage2");
			UE_DEFINE_GAMEPLAY_TAG(Stage3, "Arcanum.BattleStage.Normal.Stage3");
		}
		namespace Hard { // 추후 확장 가능하게
			UE_DEFINE_GAMEPLAY_TAG(Stage1, "Arcanum.BattleStage.Hard.Stage1");
			UE_DEFINE_GAMEPLAY_TAG(Stage2, "Arcanum.BattleStage.Hard.Stage2");
			UE_DEFINE_GAMEPLAY_TAG(Stage3, "Arcanum.BattleStage.Hard.Stage3");
		}
	}
}

namespace Arcanum
{
	namespace Player {
		namespace ID {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Player.ID", "아이디 루트");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Elara, "Arcanum.Player.ID.Elara", "엘라라");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Iris, "Arcanum.Player.ID.Iris", "아이리스");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Celestia, "Arcanum.Player.ID.Celestia", "셀레스티아");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Lynette, "Arcanum.Player.ID.Lynette", "리넷");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Arna, "Arcanum.Player.ID.Arna", "아르나");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Aiden, "Arcanum.Player.ID.Aiden", "Aiden");
		}

		namespace Grade
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Player.Grade", "등급 루트");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Common, "Arcanum.Player.Grade.Common.0", "일반");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Rare, "Arcanum.Player.Grade.Rare.1", "레어");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Epic, "Arcanum.Player.Grade.Epic.2", "영웅");
		}
	}
}

namespace Arcanum {
	namespace Gacha {
		namespace Type {
			namespace Standard {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Standard, "Arcanum.Gacha.Type.Standard", "상시 캐릭터 뽑기(전체)");
			}
			namespace CharacterPickup {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(CharacterPickup, "Arcanum.Gacha.Type.CharacterPickup", "픽업 캐릭터 뽑기");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(PickUp2601, "Arcanum.Gacha.Type.CharacterPickup.PickUp2601", "픽업 캐릭터 뽑기(26년 1주차)");
			}
			namespace WeaponPickup {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(WeaponPickup, "Arcanum.Gacha.Type.WeaponPickup", "무기/장비 뽑기");
			}
			namespace EventLimited {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(EventLimited, "Arcanum.Gacha.Type.EventLimited", "이벤트 뽑기(다시는 안나옴)");
			}
		}
	}
}

namespace Arcanum {
	namespace Skills {
		namespace SkillName {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.SkillName.None", "없음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Slash, "Arcanum.Skills.SkillName.Slash", "슬래시");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ChainBolt, "Arcanum.Skills.SkillName.ChainBolt", "체인볼트");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(PiercingShot, "Arcanum.Skills.SkillName.PiercingShot", "피어싱샷");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ShockWave, "Arcanum.Skills.SkillName.ShockWave", "쇼크웨이브");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MjolnirStrike, "Arcanum.Skills.SkillName.MjolnirStrike", "묠리르");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(VoidEclipse, "Arcanum.Skills.SkillName.VoidEclipse", "보이드");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(SolarFlare, "Arcanum.Skills.SkillName.SolarFlare", "솔라플레어");
		}
		namespace SkillTypes {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.SkillTypes.None", "없음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Active, "Arcanum.Skills.SkillTypes.Active", "액티브");  // 실제 필드에 보일 애들
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Passive, "Arcanum.Skills.SkillTypes.Passive", "패시브");	// 캐릭터 내부에서만 작동하는 애들
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Aura, "Arcanum.Skills.SkillTypes.Aura", "오라");		// 실제 외부에 나타나는 Passive 계열
		}
		namespace TargetingType {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.TargetingType.None", "없음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Self, "Arcanum.Skills.TargetingType.Self", "자신");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Unit, "Arcanum.Skills.TargetingType.Unit", "개체");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ground, "Arcanum.Skills.TargetingType.Ground", "범위");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Direction, "Arcanum.Skills.TargetingType.Direction", "전방향");
		}
		namespace TargetFilter {
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.TargetFilter.None", "없음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Enemy, "Arcanum.Skills.TargetFilter.Enemy", "적군");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ally, "Arcanum.Skills.TargetFilter.Ally", "아군");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Self, "Arcanum.Skills.TargetFilter.Self", "자신");
		}
		namespace SkillVFX { // 이펙트 효과용 0을 기준으로 +버프, -디버프 => 추후 스킬컴포넌트에서 스택도 관리함
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "Arcanum.Skills.SkillVFX.None", "없음");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bleed, "Arcanum.Skills.SkillVFX.Bleed", "출혈");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Guard, "Arcanum.Skills.SkillVFX.Guard", "수호");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mark, "Arcanum.Skills.SkillVFX.Mark", "표식");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Atonement, "Arcanum.Skills.SkillVFX.Atonement", "속죄");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shock, "Arcanum.Skills.SkillVFX.Shock", "감전");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Burn, "Arcanum.Skills.SkillVFX.Burn", "화상");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stun, "Arcanum.Skills.SkillVFX.Stun", "스턴");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Knockback, "Arcanum.Skills.SkillVFX.Knockback", "넉백");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MoveSpeed, "Arcanum.Skills.SkillVFX.MoveSpeed", "이동속도");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(AttackSpeed, "Arcanum.Skills.SkillVFX.AttackSpeed", "공격속도");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(DamageReduction, "Arcanum.Skills.SkillVFX.DamageReduction", "피해");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(CritChance, "Arcanum.Skills.SkillVFX.CritChance", "크리증가");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HealthRegen, "Arcanum.Skills.SkillVFX.HealthRegen", "지속회복");
		}
	}
	namespace Items {
		namespace ItemSlot {
			namespace Weapon {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(LeftHand, "Arcanum.Items.ItemSlot.Weapon.LeftHand", "왼손");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(RightHand, "Arcanum.Items.ItemSlot.Weapon.RightHand", "오른손");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(TwoHand, "Arcanum.Items.ItemSlot.Weapon.TwoHand", "양손");
			}
			namespace Armor {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Items.ItemSlot.Armor.Helmet", "머리");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Items.ItemSlot.Armor.Chest", "몸통");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Items.ItemSlot.Armor.Glove", "손");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Items.ItemSlot.Armor.Boot", "발");
			}
		}
		namespace Rarity {
			namespace Common {
				namespace Weapon {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(GreatSword, "Arcanum.Items.Rarity.Common.Weapon.GreatSword", "대검");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Staff, "Arcanum.Items.Rarity.Common.Weapon.Staff", "지팡이");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Bow, "Arcanum.Items.Rarity.Common.Weapon.Bow", "활");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shield, "Arcanum.Items.Rarity.Common.Weapon.Shield", "방패");
				}
				namespace Armor {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Items.Rarity.Common.Armor.Helmet", "투구");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Items.Rarity.Common.Armor.Chest", "갑옷");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Items.Rarity.Common.Armor.Glove", "장갑");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Items.Rarity.Common.Armor.Boot", "신발");
				}
			}
			namespace SetItem {
				namespace Talasha {
					namespace Armor {
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Items.Rarity.SetItem.Talasha.Armor.Helmet", "투구");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Items.Rarity.SetItem.Talasha.Armor.Chest", "갑옷");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Items.Rarity.SetItem.Talasha.Armor.Glove", "장갑");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Items.Rarity.SetItem.Talasha.Armor.Boot", "신발");
					}
				}
				namespace Sigon {
					namespace Armor {
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Helmet, "Arcanum.Items.Rarity.SetItem.Sigon.Armor.Helmet", "투구");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Chest, "Arcanum.Items.Rarity.SetItem.Sigon.Armor.Chest", "갑옷");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Glove, "Arcanum.Items.Rarity.SetItem.Sigon.Armor.Glove", "장갑");
						UE_DEFINE_GAMEPLAY_TAG_COMMENT(Boot, "Arcanum.Items.Rarity.SetItem.Sigon.Armor.Boot", "신발");
					}
				}
			}
			namespace Legendary {
				namespace Weapon {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scepter, "Arcanum.Items.Rarity.Legendary.Weapon.Scepter", "셉터");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Scythe, "Arcanum.Items.Rarity.Legendary.Weapon.Scythe", "낫");
				}
				namespace Armor {
				}
			}
			namespace SetBonus {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Surge, "Arcanum.Items.SetBonus.Surge", "역류");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Avarice, "Arcanum.Items.SetBonus.Avarice", "탐욕");
			}
		}
		namespace Type
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Equipment, "Arcanum.Items.Type.Equipment", "아이템 타입: 장비");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Potion, "Arcanum.Items.Type.Potion", "아이템 타입: 포션");
		}
		namespace Potion
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.Items.Potion", "포션 루트");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Health, "Arcanum.Items.Potion.Health", "체력 포션");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Power, "Arcanum.Items.Potion.Power", "힘의 포션");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Focus, "Arcanum.Items.Potion.Focus", "집중의 포션");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Frenzy, "Arcanum.Items.Potion.Frenzy", "광란의 포션");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Haste, "Arcanum.Items.Potion.Haste", "신속 포션");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Mana, "Arcanum.Items.Potion.Mana", "마나 포션");
		}
	}
	namespace Inventory
	{
		namespace Store
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Guid, "Arcanum.Inventory.Store.Guid", "저장 정책: 비중첩(GUID 1개=1칸)");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Stack, "Arcanum.Inventory.Store.Stack", "저장 정책: 스택(TMap Tag->Count)");
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
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.Regen.Health", "체력 루트 태그");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Character.Regen.Health.Value", "현재 체력");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.BattleStat.Character.Regen.Health.MaxValue", "최대 체력");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen, "Arcanum.BattleStat.Character.Regen.Health.Regen", "체력 재생량");
				}
				namespace Mana {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.Regen.Mana", "캐릭터 마나 루트 태그");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Character.Regen.Mana.Value", "현재 마나");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.BattleStat.Character.Regen.Mana.MaxValue", "최대 마나");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen, "Arcanum.BattleStat.Character.Regen.Mana.Regen", "마나 재생량");
				}
			}
			namespace NonRegen {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.NonRegen", "캐릭터 논리젠스탯 루트");
				namespace AttackPower {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.NonRegen.AttackPower", "공격력 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Character.NonRegen.AttackPower.Value", "기본 공격력");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue, "Arcanum.BattleStat.Character.NonRegen.AttackPower.BonusValue", "추가 공격력");
				}
				namespace AttackSpeed {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.NonRegen.AttackSpeed", "캐릭터 공격 속도 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Character.NonRegen.AttackSpeed.Value", "기본 공격 속도");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue, "Arcanum.BattleStat.Character.NonRegen.AttackSpeed.BonusValue", "추가 공격 속도");
				}
				namespace Evasion {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.NonRegen.Evasion", "캐릭터 회피율 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Character.NonRegen.Evasion.Value", "기본 회피율");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue, "Arcanum.BattleStat.Character.NonRegen.Evasion.BonusValue", "추가 회피율");
				}
				namespace MoveSpeed {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.NonRegen.MoveSpeed", "캐릭터 이동 속도 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Character.NonRegen.MoveSpeed.Value", "기본 이동 속도");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue, "Arcanum.BattleStat.Character.NonRegen.MoveSpeed.BonusValue", "추가 이동 속도");
				}
				namespace CritChance {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.NonRegen.CritChance", "캐릭터 치명타 확률 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Character.NonRegen.CritChance.Value", "기본 치명타 확률");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue, "Arcanum.BattleStat.Character.NonRegen.CritChance.BonusValue", "추가 치명타 확률");
				}
				namespace DamageReduction {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.BattleStat.Character.NonRegen.DamageReduction", "캐릭터 피해 감소 루트");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.BattleStat.Character.NonRegen.DamageReduction.Value", "기본 피해 감소");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(BonusValue, "Arcanum.BattleStat.Character.NonRegen.DamageReduction.BonusValue", "추가 피해 감소");
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
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.Regen.Supply", "고기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Currencies.Regen.Supply.Value", "현재 고기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.PlayerData.Currencies.Regen.Supply.MaxValue", "최대 고기");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Regen, "Arcanum.PlayerData.Currencies.Regen.Supply.Regen", "고기 재생시간");
				}
			}
			namespace NonRegen {
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.NonRegen", "플레이어 논리젠 재화 루트");
				namespace Gold {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.NonRegen.Gold", "골드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Currencies.NonRegen.Gold.Value", "현재 골드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.PlayerData.Currencies.NonRegen.Gold.MaxValue", "최대 골드 보유량");
				}
				namespace Shard {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.NonRegen.Shard", "샤드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Currencies.NonRegen.Shard.Value", "현재 샤드");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.PlayerData.Currencies.NonRegen.Shard.MaxValue", "최대 샤드 보유량");
				}
				namespace Soul {
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Arcanum.PlayerData.Currencies.NonRegen.Soul", "소울");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Arcanum.PlayerData.Currencies.NonRegen.Soul.Value", "현재 소울");
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue, "Arcanum.PlayerData.Currencies.NonRegen.Soul.MaxValue", "최대 소울 보유량");
				}
			}
		}
	}
}


// 김유진
namespace Arcanum
{
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
	}
}
