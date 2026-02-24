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
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Iris);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Celestia);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Lynette);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Arna);
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
		namespace Pickup {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot1);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot2);
		}
		namespace Always {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot1); // 무조건 플레이어 아군 전체 뽑기(플레이어, 쫄병)
		}
		namespace Item {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slot1); // 무조건 아이템 전체 뽑기
		}
	}
}

namespace Arcanum {
	namespace Skills {
		namespace SkillName {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slash);			// 무기 스킬: 베기
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ChainBolt);		// 무기 스킬: 체인 볼트
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PiercingShot);	// 무기 스킬: 관통 사격
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShockWave);		// 무기 스킬: 충격파
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MjolnirStrike);	// 무기 스킬: 묠니르
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(VoidEclipse);	// 무기 스킬: 보이드
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
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon1);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon2);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(LegendaryWeapon);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boots);
		}
		namespace Rarity {
			namespace Common {
				namespace Weapon {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GreatSword); // 대검
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Staff);      // 지팡이
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Bow);        // 활
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shield);     // 방패
				}
				namespace Armor {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet); // 투구 
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);  // 갑옷
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);  // 장갑
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boots);   // 신발
				}
			}
			namespace SetItem {
				namespace Weapon {
				}
				namespace Armor {
					namespace Talasha {
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet); // 투구 
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);  // 갑옷
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);  // 장갑
						ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boots);   // 신발
					}
				}
			}
			namespace Legendary {
				namespace Weapon {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spear);      // 창
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scepter);    // 셉터
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scythe);     // 낫
				}
				namespace Armor {
				}
			}
		}
		namespace SetBonus {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Surge);    // 역류
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Majesty);  // 위엄
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Avarice);  // 탐욕
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

// KYJ : 전투 상태
//	namespace Combat {
//		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//		namespace State {
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Alive);			// 살아있음
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dead);			// 죽음
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stunned);		// 스턴
//		}
//	}
//	// KYJ : 아군, 적군 유닛
//	namespace Unit {
//		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//		// 아군인지 적군인지 체크
//		namespace Faction {
//			// 아군
//			namespace Ally {
//				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				namespace Army {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Bard {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Maid {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Tinker {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//			}
//			// 적군
//			namespace Enemy {
//				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				namespace Macaron {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Leech {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace GhostMacaron {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Bat {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Cell {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Mask {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Wolfron {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace MageMacaron {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace Felice {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//				namespace CupFelice {
//					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//				}
//			}
//		}
//		
//		namespace Role {
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tank);				// 방어
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Melee);				// 근딜
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ranged);				// 원딜
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AoEDPS);				// 광딜
//		}
//	}
//	}
//
//	// KYJ : 전투 제어
//	namespace Battle {
//		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
//		namespace Mode {
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Manual);		// 수동 조작
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Auto);		// 자동 전투
//		}
//
//		namespace State {
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Active);		// 전투 중
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Paused);		// 일시정지
//			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ended);		// 전투 종료
//		}
//	}
//}