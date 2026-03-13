#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

#pragma region 헬퍼 함수
FORCEINLINE static FName GetLeafNameFromTag(const FGameplayTag& InTag)
{
	if (!InTag.IsValid()) return NAME_None;

	FString TagString = InTag.GetTagName().ToString();
	int32 LastDotIndex = -1;

	if (TagString.FindLastChar('.', LastDotIndex)) return FName(*TagString.RightChop(LastDotIndex + 1));
	else return FName(*TagString);
}
FORCEINLINE static int32 GetGradePriority(const FGameplayTag& InGradeTag)
{
	if (!InGradeTag.IsValid()) return 0;
	FString TagName = InGradeTag.GetTagName().ToString();
	FString LeftSide, RightSide;
	if (TagName.Split(TEXT("."), &LeftSide, &RightSide, ESearchCase::IgnoreCase, ESearchDir::FromEnd)) {
		if (RightSide.IsNumeric()) return FCString::Atoi(*RightSide);
	}

	return 0;
}
#pragma endregion

namespace Arcanum {
	namespace DataTable {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CharacterInfo);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattleStats);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Equipment);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PlayerBattleStats);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GachaTable);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SkillData);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StageInfo);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AllyUnitInfo);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EnemyUnitInfo);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AllyUnitStatInfo);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EnemyUnitStatInfo);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Potion);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemCatalog);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatDisplay);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EnhanceRule);
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShopCategoryRule);

		//ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattleRegenStats);
		//ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattleNonRegenStats);
	}
}

namespace Arcanum {
	namespace BattleStage {
		namespace Normal {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage1);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage2);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage3);
		}
		namespace Hard { // 추후 확장 가능하게
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage1);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage2);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stage3);
		}
	}
}

namespace Arcanum {
	namespace Player {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		namespace ID {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Elara); 
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Iris);	// Iris : 빠른 공격 속도와 회피 능력을 강조하는 캐릭터.
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Celestia);   // Celestia : 강력한 공격력과 방어 능력을 갖춘 캐릭터, 지속적인 회복.
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Lynette);   // Lynette : 빠르고 치명적인 크리티컬, 높은 이동 속도와 공격 속도.
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Arna);  // Arna : 높은 공격력과 크리티컬 확률, 공격에 특화된 캐릭터.
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aiden);  // Aiden : 균형 잡힌 공격력, 회피 능력, 크리티컬 확률을 갖춘 다재다능한 캐릭터.
		}

		namespace Grade { // cpp 확인 필수
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common);				// 일반
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rare);				// 레어
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Epic);				// 영웅
		}
	}

}

namespace Arcanum {
	namespace Gacha {
		namespace Type {
			namespace Standard {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Standard);
			}
			namespace CharacterPickup {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CharacterPickup);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PickUp2601);
			}
			namespace WeaponPickup {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WeaponPickup);
			}
			namespace EventLimited {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(EventLimited);
			}
		}
	}
}

namespace Arcanum {
	namespace Skills {
		namespace SkillName {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slash);			// 무기 스킬: 베기(했음)
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ChainBolt);		// 무기 스킬: 체인 볼트(했음)
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PiercingShot);	// 무기 스킬: 관통 사격(했음)
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShockWave);		// 무기 스킬: 충격파(했음)
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MjolnirStrike);	// 무기 스킬: 묠니르
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(VoidEclipse);	// 무기 스킬: 보이드(했음)
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SolarFlare);		// 무기 스킬: 솔라
		}
		namespace SkillTypes {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Active);			// 실제 필드에 보일 애들
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Passive);		// 캐릭터 내부에서만 작동하는 애들
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Aura);			// 실제 외부에 나타나는 Passive 계열
		}
		namespace TargetingType {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Self);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ground);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Direction);
		}
		namespace TargetFilter {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ally);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Self);
		}
		namespace SkillVFX { // 이펙트 효과용 0을 기준으로 +버프, -디버프 => 추후 스킬컴포넌트에서 스택도 관리함
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Bleed);       // 출혈
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Guard);       // 수호
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mark);        // 표식
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Atonement);   // 속죄
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shock);       // 감전 
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Burn);        // 화상
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stun);        // 스턴
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Knockback);   // 넉백
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveSpeed);   // 이동속도
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackSpeed); // 공격속도
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageReduction); // 피해
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CritChance);      // 크리 증가
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthRegen);     // 지속 회복
		}
	}
	namespace Items {
		namespace ItemSlot {
			namespace Weapon {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(LeftHand);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(RightHand);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TwoHand);

				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot1);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot2);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Legendary);
			}
			namespace Armor {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boot);
			}
		}
		namespace Grade
		{
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SetItem);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Legendary);
		}
		namespace Rarity {
			namespace Common {
				namespace Weapon {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GreatSword); // 대검
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Staff);      // 지팡이
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Bow);        // 활
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shield);     // 방패
				}
				namespace Armor {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);      // 갑옷
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet);     // 투구 
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);      // 장갑
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boot);       // 신발
				}
			}
			namespace Legendary {
				namespace Weapon {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scepter);    // 셉터
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scythe);     // 낫
				}
			}
			namespace SetItem {
				namespace Talasha {
					namespace Armor {
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet); // 투구 
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);  // 갑옷
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);  // 장갑
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boot);   // 신발
					}
				}
				namespace Sigon {
					namespace Armor {
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet); // 투구 
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);  // 갑옷
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);  // 장갑
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boot);   // 신발
					}
				}
			}
		}
		namespace SetBonus {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Surge);    // 역류
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Avarice);  // 탐욕
		}
		namespace Type
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Equipment);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Potion);
		}
		namespace Potion
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RegenHealth);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Power);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Focus);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Frenzy);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Haste);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mana);
		}
	}
	namespace Inventory
	{
		namespace Store
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Guid);
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stack);
		}
	}
}

namespace Arcanum {
	namespace BattleStat {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		namespace Player {
			namespace Regen {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace Meat {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Regen);
				}
			}
			namespace NonRegen {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace CommandCenterStamina {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BonusValue);
				}
			}
		}
		namespace Character {
			namespace Regen {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace Health {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Regen);
				}
				namespace Mana {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Regen);
				}
			}
			namespace NonRegen {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace AttackPower {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BonusValue);
				}
				namespace AttackSpeed {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BonusValue);
				}
				namespace Evasion {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BonusValue);
				}
				namespace MoveSpeed {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BonusValue);
				}
				namespace CritChance {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BonusValue);
				}
				namespace DamageReduction {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BonusValue);
				}
			}
		}
	}
	namespace PlayerData {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		// 재화들
		namespace Currencies {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
			namespace Regen {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace Supply {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Regen);
				}
			}
			namespace NonRegen {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace Gold {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
				}
				namespace Shard {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
				}
				namespace Soul {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
				}
			}
		}
	}
}

//	// KYJ : 아군, 적군 유닛
namespace Arcanum {
	namespace Unit {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		// 아군인지 적군인지 체크
		namespace Faction {
			// 아군
			namespace Ally {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace Army {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Bard {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Maid {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Tinker {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace PunchMan {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
			}
			// 적군
			namespace Enemy {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace Macaron {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Leech {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace GhostMacaron {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Bat {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Cell {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Mask {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Wolfron {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace MageMacaron {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace Felice {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
				namespace CupFelice {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				}
			}
		}
	}
}