// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleStageProgressWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void UBattleStageProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBattleStageProgressWidget::SetPlayerLocationProgress(const FVector& InAllyNexusLocation, const FVector& InEnemyNexusLocation, const FVector& InPlayerLocation)
{
	FVector2D AllyNexusLocation = FVector2D(InAllyNexusLocation);
	FVector2D EnemyNexusLocation = FVector2D(InEnemyNexusLocation);
	FVector2D PlayerLocation = FVector2D(InPlayerLocation);

	float AllyToPlayer = (AllyNexusLocation - PlayerLocation).Length();
	float AllyToEnemy = (AllyNexusLocation - EnemyNexusLocation).Length();

	float Result = FMath::Clamp(AllyToPlayer / AllyToEnemy, 0.0f, 1.0f);
	StageProgressSlider->SetValue(Result);
}

void UBattleStageProgressWidget::SetTime(int32 TimeMS)
{
	int32 Minutes = TimeMS / 60000;
	int32 Seconds = (TimeMS % 60000) / 1000;
	int32 Milli = TimeMS % 1000;

	FString ResultString = FString::Printf(TEXT("%d분 %d초 %d"), Minutes, Seconds, Milli);
	TimeText->SetText(FText::FromString(ResultString));
}
