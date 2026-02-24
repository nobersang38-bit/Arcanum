// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BattlePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/Battle/Contents/InBattleHUDWidget.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UInBattleHUDWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
}
