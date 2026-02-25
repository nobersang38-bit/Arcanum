#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTags/ArcanumTags.h"
#include "DataInfo/BattleCharacter/FBattleCharacterData.h"
#include "ArcanumSaveGame.generated.h"

/**
 * Version : 1.0.0.0 2026/02/04
 * 역할 :
 * 1. 모든 Load/Save할 데이터
 */

UCLASS()
class ARCANUM_API UArcanumSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	/** ID와 PW를 저장할 맵 */
	UPROPERTY()
	TMap<FString, FString> UserDatabase;

	/** 캐릭터 태그를 키로 하여 현재 진행 상태(등급, 조각 등)를 저장 */
	UPROPERTY()
	TMap<FGameplayTag, FBattleCharacterData> SavedCharacters;

#pragma region 상점 아이템 저장
public:
	/* 상점 다음 갱신 시각 */
	UPROPERTY()
	FDateTime NextShopRefreshTime;
	/* 현재 상점 슬롯 아이템 RowName 목록 */
	UPROPERTY()
	TArray<FName> CurrentShopRowNames;
	/* 현재 상점 슬롯 품절 여부 목록 */
	UPROPERTY()
	TArray<bool> CurrentShopSoldOutStates;
#pragma endregion
};
