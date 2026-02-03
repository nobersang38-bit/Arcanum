// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * 
 */
class ARCANUM_API ArcanumTags
{

};

// 김도현
// 설명
// 플레이어 캐릭터가 아닌 
// 플레이어가 가지고 있어야할 정보의 태그 모음
namespace PlayerTags
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

// 김도현
// 설명
// 플레이어 캐릭터들 계속 추가
namespace PlayerCharacterTags
{
	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	namespace TestCharacter // KDH 테스트 캐릭터 태그입니다 구조체를 만들어 그것을 가져와야합니다
	{
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
	}
}

// 김도현
// 설명
// 무기들 계속 추가
namespace Weapons
{
	ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Root);
	namespace TestWeapon // KDH 테스트 무기 태그입니다 구조체를 만들어 그것을 가져와야합니다
	{
		ARCANUM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Value);
	}
}
