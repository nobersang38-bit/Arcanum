// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 *
 */
 // KYJ : 플레이어 캐릭터
namespace ArcanumTags {
	namespace Player {
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
		namespace ID {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(LILLY);
		}
		namespace Class {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Warrior);			// 전사
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Knight);				// 기사
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Mage);				// 마법사
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Archer);				// 궁수
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Priest);				// 사제
		}
		namespace Role {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Melee);				// 근딜
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ranged);				// 원딜
		}
		// 전투 타입
		namespace CombatType {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Breaker);			// 돌파형
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Defender);			// 보호형
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Magic);				// 마법형
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Physical);			// 물리형
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Support);			// 지원형
		}
		namespace Grade {
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common);				// 일반
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Rare);				// 레어
			ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Epic);				// 영웅
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
