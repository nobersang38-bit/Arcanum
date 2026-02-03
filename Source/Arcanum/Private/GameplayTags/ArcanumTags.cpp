// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTags/ArcanumTags.h"

#pragma region KDH
// 김도현
// 설명
// 플레이어 캐릭터가 아닌 
// 플레이어가 가지고 있어야할 정보의 태그 모음
namespace PlayerTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Player", "플레이어가 가지고 있어야할 정보의 모음");
	namespace Currencies // 재화들
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Player.Currencies", "플레이어가 가지고 있는 재화들");
		namespace Gold // 골드
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Player.Currencies.Gold",			"골드");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Player.Currencies.Gold.Value",		"골드 현재값");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,	"Player.Currencies.Gold.MaxValue",	"골드 최대값");
		}
		namespace Shard // 조각
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Player.Currencies.Shard",			"조각");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Player.Currencies.Shard.Value",	"조각 현재값");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,	"Player.Currencies.Shard.MaxValue", "조각 최대값");
		}
		namespace Soul // 소울
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root,		"Player.Currencies.Soul",			"소울"		);
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value,		"Player.Currencies.Soul.Value",		"소울 현재값"	);
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MaxValue,	"Player.Currencies.Soul.MaxValue",	"소울 최대값"	);
		}
	}

	namespace Inventories
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Player.Inventories", "현재 플레이어가 가지고 있는 것들");
		namespace ConsumedItems // 사용 아이템들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.Inventories.ConsumedItems","사용 아이템들");
		}

		namespace Weapons // 무기들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.Inventories.Weapons",		"무기들");
		}

		namespace Equipments // 장비들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.Inventories.Equipments",	"장비들");
		}

		namespace Heroes // 영웅들(플레이어 캐릭터)
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.Inventories.Heroes",		"영웅들");
		}

		namespace Units // 아군 유닛들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.Inventories.Units",		"아군 유닛들");
		}
	}

	namespace StageInfo // 클리어한 스테이지 정보
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.StageInfo", "클리어한 스테이지 정보") // 나중에 구조체에서는 배열같은 값을 불러올 것 같습니다
	}

	namespace LastEquips // 마지막으로 장착한 객체들
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Root, "Player.LastEquips", "마지막으로 장착한 객체들(게임시작시 불러옴)");
		namespace Hero // 영웅(플레이어 캐릭터)
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.LastEquips.Hero",			"영웅(플레이어 캐릭터)");
		}

		namespace ConsumedItems // 사용 아이템들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.LastEquips.ConsumedItems","사용 아이테들");
		}

		namespace Equipments // 장비들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.LastEquips.Equipments",	"장비들");
		}

		namespace Weapons // 무기들
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Value, "Player.LastEquips.Weapons",		"무기들");
		}
	}
}
#pragma endregion
