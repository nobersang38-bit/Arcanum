#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * TODO : 태그
 * 추영호 : State, Item, Ability, Effect
 */
namespace Arcanum {
	/* 상태 */
	namespace State {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PreAwakened); // 각성 전
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Awakened);    // 각성 후
		/* 각성 오라 */
		namespace Aura {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FuryActive);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ResolveActive);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ArcaneActive);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FocusActive);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HolyActive);
		}
	}
	/* 아이템 */
	namespace Item {
		/* 장비 */
		namespace Equipment {
			namespace Weapon {
				namespace Common {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GreatSword); // 대검
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Staff);      // 지팡이
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Bow);        // 활
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shield);     // 방패
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Wand);       // 완드
				}
				namespace Legendary {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spear);      // 번개창
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Blade);      // 블레이드
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scepter);    // 셉터
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scythe);     // 낫
				}
			}
			namespace Armor {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet); // 투구 
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);  // 갑옷
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);  // 장갑
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boot);   // 신발
			}
			namespace SetItem {
				namespace Armor {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Talasha);
				}
			}
		}
		namespace SetBonus
		{
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Surge);    // 역류
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Majesty);  // 위엄
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Avarice);  // 탐욕
		}
	}
	/* 행동 */
	namespace Ability {
		namespace Passive {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fury);    // 분노
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Resolve); // 결의
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Arcane);  // 비전
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Focus);   // 집중
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Holy);    // 신성
		}
		/* 스킬 */
		namespace Skill {
			namespace Weapon {
				namespace Common {
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slash);         // 베기
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MissileArc);    // 궤적 미사일
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PiercingShot);  // 관통 사격
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShockWave);     // 충격파
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Penance);       // 참회
				}
				namespace Legendary {  // 궁극기
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MjolnirStrike); // 묠니르
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(VoidRift);      // 보이드 블레이드
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SolarFlare);    // 솔라
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Lantern);       // 랜턴
				}
			}
		}
		/* 발동 */
		namespace Event	{
			namespace Trigger {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BloodBurst);    // 혈폭
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Retribution);   // 응징
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Collapse);      // 붕괴
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Execution);     // 처단
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Purification);  // 정화
			}
		}	    
	}
	/* 효과 */
	namespace Effect {
		namespace Buff {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackSpeed);     // 공격속도 증가
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageReduction); // 피해 감소
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SkillHaste);      // 스킬 가속
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CritChance);      // 크리 증가
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthRegen);     // 지속 회복
		}
		namespace Debuff {
			namespace Status {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Bleed);       // 출혈
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Thorns);      // 가시
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Resonance);   // 공명
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mark);        // 표식
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Atonement);   // 속죄

				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shock);       // 감전 
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Burn);        // 화상
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fear);        // 공포
			}
			namespace Modifier {
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveSpeed);   // 이동속도 감소
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackSpeed); // 공격속도 감소
			}
		}
	}
}