// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayHUD.generated.h"

class UBasementHealthBar;
class ABasement;

/*
 * 역할 : 김유진
 * 1. 게임 플레이 HUD
 * 2. 아군/ 적군 기지 HP 바 표시
 * 3. 중단 버튼
 * // 스킬아이콘, 아군스폰아이콘, 수동/자동전투, 아이템 아이콘
 */
UCLASS()
class ARCANUM_API UGamePlayHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBasementHealthBar> PlayerBasementHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBasementHealthBar> EnemyBasementHealthBar;
};
