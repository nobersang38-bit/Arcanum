#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

// KYJ : 플레이어 캐릭터
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

		//namespace Class {
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Warrior);			// 전사
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Knight);				// 기사
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mage);				// 마법사
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Archer);				// 궁수
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Priest);				// 사제
		//}
		//namespace Role {
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Melee);				// 근딜
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ranged);				// 원딜
		//}
		// 전투 타입
		//namespace CombatType {
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Breaker);			// 돌파형
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Defender);			// 보호형
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Magic);				// 마법형
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Physical);			// 물리형
		//	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Support);			// 지원형
		//}
		namespace Grade {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common);				// 일반
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rare);				// 레어
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Epic);				// 영웅
		}
	}
	// KYJ : 자원
	namespace Resource {
		namespace Health {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxHealth);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
		}
		namespace Mana {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxMana);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ValueMana);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Regen);

		}
		// 게임 진행 시 필요한 고기
		namespace Supply {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxSupply);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ValueSupply);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Regen);
		}
	}
	// KYJ : 전투 상태
	namespace Combat {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		namespace State {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Alive);			// 살아있음
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Dead);			// 죽음
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Stunned);		// 스턴
		}
	}
	// KYJ : 아군, 적군 유닛
	namespace Unit {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		// 아군인지 적군인지 체크
		namespace Faction {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ally);				// 아군
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy);				// 적군
		}
		// 강화 상태
		namespace Enhance {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enhanced);			// 강화됨
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Max);				// 최대로 강화됨
		}
		namespace Role {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Tank);				// 방어
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Melee);				// 근딜
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ranged);				// 원딜
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AoEDPS);				// 광딜
		}
		namespace SpawnType {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Manual);				// 수동 배치 (아군)
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Auto);				// 자동 스폰 (아군)
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Wave);				// 웨이브 스폰 (적군)
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Summon);				// 소환 (적군)
		}
		namespace EnemyType {
			// 적 유닛 - 일반형
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Runner);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BigHeavy);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shooter);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spawner);

			// 적 유닛 - 보스
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Buffer);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Assassin);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Area);
		}
	}

	// KYJ : AI 타겟팅 로직
	namespace AI {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		namespace TargetPriority {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Nearest);		// 가장 가까운 적
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weakest);		// HP 낮은 적
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Strongest);		// 공격력 높은 적
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boss);			// 보스 우선
		}
	}

	// KYJ : 전투 제어
	namespace Battle {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		namespace Mode {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Manual);		// 수동 조작
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Auto);		// 자동 전투
		}

		namespace State {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Active);		// 전투 중
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Paused);		// 일시정지
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ended);		// 전투 종료
		}
	}
}

/**
 * TODO : 추영호 태그
 */
namespace Arcanum 
{
	/* 상태 */
	namespace State 
	{
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PreAwakened); // 각성 전
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Awakened);    // 각성 후
		/* 각성 오라 */
		namespace Aura 
		{
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FuryActive);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ResolveActive);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ArcaneActive);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(FocusActive);
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HolyActive);
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
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GreatSword); // 대검
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Staff);      // 지팡이
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Bow);        // 활
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shield);     // 방패
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Wand);       // 완드
				}
				namespace Legendary 
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spear);      // 번개창
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Blade);      // 블레이드
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scepter);    // 셉터
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Scythe);     // 낫
				}
			}
			namespace Armor
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Helmet); // 투구 
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Chest);  // 갑옷
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Glove);  // 장갑
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Boot);   // 신발
			}
			namespace SetItem 
			{
				namespace Armor
				{
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
	/* 능력치 */
	namespace Stat 
	{
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BaseAttack);        // 기본 공격력
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SkillPower);        // 스킬 공격력 계수
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(LifeSteal);         // 생명력 흡수
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackSpeed);       // 공격 속도
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Evasion);           // 회피율
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CooldownReduction); // 쿨다운 감소
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveSpeed);         // 이동속도
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CritChance);        // 치명타 확률
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Spirit);            // 정신력
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mana);              // 마나
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Health);            // 체력
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageReduction);   // 피해 감소
	}
	/* 행동 */
	namespace Ability 
	{
		namespace Passive 
		{
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fury);    // 분노
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Resolve); // 결의
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Arcane);  // 비전
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Focus);   // 집중
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Holy);    // 신성
		}
		/* 스킬 */
		namespace Skill
		{
			namespace Weapon 
			{
				namespace Common 
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Slash);         // 베기
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MissileArc);    // 궤적 미사일
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(PiercingShot);  // 관통 사격
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ShockWave);     // 충격파
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Penance);       // 참회
				}
				namespace Legendary 
				{  
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MjolnirStrike); // 묠니르
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(VoidRift);      // 보이드 블레이드
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SolarFlare);    // 솔라
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Lantern);       // 랜턴
				}
			}
		}
		/* 발동 */
		namespace Event	
		{
			namespace Trigger
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BloodBurst);    // 혈폭
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Retribution);   // 응징
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Collapse);      // 붕괴
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Execution);     // 처단
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Purification);  // 정화
			}
		}	    
	}
	/* 효과 */
	namespace Effect
	{
		namespace Buff 
		{
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackSpeed);     // 공격속도 증가
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DamageReduction); // 피해 감소
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SkillHaste);      // 스킬 가속
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CritChance);      // 크리 증가
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(HealthRegen);     // 지속 회복
		}
		namespace Debuff 
		{
			namespace Status 
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Bleed);       // 출혈
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Thorns);      // 가시
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Resonance);   // 공명
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mark);        // 표식
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Atonement);   // 속죄

				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shock);       // 감전 
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Burn);        // 화상
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Fear);        // 공포
			}
			namespace Modifier
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MoveSpeed);   // 이동속도 감소
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(AttackSpeed); // 공격속도 감소
			}
		}
	}
}

// 김도현
namespace Arcanum
{
	// 저장 데이터
	namespace SaveData
	{
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		namespace PlayerDataTags
		{
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
			namespace Currencies // 재화들
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace Gold // 골드
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);

					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
				}
				namespace Shard // 조각
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);

					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
				}
				namespace Soul // 소울
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);

					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(MaxValue);
				}
			}

			namespace Inventories
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace ConsumedItems // 사용 아이템들
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}

				namespace Weapons // 무기들
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}

				namespace Equipments // 장비들
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}

				namespace Heroes // 영웅들(플레이어 캐릭터)
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}

				namespace Units // 아군 유닛들
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}
			}

			namespace StageInfo // 클리어한 스테이지 정보
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value) // 나중에 구조체에서는 배열같은 값을 불러올 것 같습니다
			}

			namespace LastEquips // 마지막으로 장착한 객체들
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
				namespace Hero // 영웅(플레이어 캐릭터)
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}

				namespace ConsumedItems // 사용 아이템들
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}

				namespace Equipments // 장비들
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}

				namespace Weapons // 무기들
				{
					ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
				}
			}
		}
	}

	// 고정 데이터
	namespace StaticData
	{
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		namespace Stage
		{
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);

			// 테스트 스테이지
			namespace TestStage01
			{
				ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
			}
		}
	}
	
}