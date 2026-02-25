// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/Contents/InBattleHUDWidget.h"
#include "UI/Battle/SubLayout/BattleActionButtonWidget.h"
#include "UI/Battle/SubLayout/BattleAllyUnitPanelWidget.h"
#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "UI/Battle/SubLayout/BattleCostBarWidget.h"
#include "UI/Battle/SubLayout/BattleHealthBarWidget.h"
#include "UI/Battle/SubLayout/BattleStageProgressWidget.h"
#include "UI/Battle/SubLayout/BattleToggleWidget.h"

// ========================================================
// 언리얼 기본 생성 및 초기화
// ========================================================
void UInBattleHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindCallbacks();
}

// ========================================================
// 플레이어, 보스 체력바(건물아님)
// ========================================================
void UInBattleHUDWidget::SetPlayerCharacterHealthBarProgress(float CurrentHealth, float MaxHealth)
{
	if (!PlayerCharacterHealthBar) return;
	PlayerCharacterHealthBar->SetHealthProgress(CurrentHealth, MaxHealth);
}

void UInBattleHUDWidget::SetBossHealthBarProgress(float CurrentHealth, float MaxHealth)
{
	if (!BossHealthBar) return;
	BossHealthBar->SetHealthProgress(CurrentHealth, MaxHealth);
}

void UInBattleHUDWidget::ShowBosHealthPBar(bool bIsShow)
{
	if (!BossHealthBar) return;
	BossHealthBar->SetVisibility(bIsShow ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

// ========================================================
// 스테이지 진행도
// ========================================================
void UInBattleHUDWidget::SetPlayerLocationProgress(const FVector& InAllyNexusLocation, const FVector& InEnemyNexusLocation, const FVector& InPlayerLocation)
{
	if (!StageProgress) return;
	StageProgress->SetPlayerLocationProgress(InAllyNexusLocation, InEnemyNexusLocation, InPlayerLocation);
}

void UInBattleHUDWidget::SetTime(int32 TimeMS)
{
	if (!StageProgress) return;
	StageProgress->SetTime(TimeMS);
}

// ========================================================
// 전투 관련 버튼들
// ========================================================
void UInBattleHUDWidget::ClickBasicAttack()
{
	OnClickBasicAttack.Broadcast();
}

void UInBattleHUDWidget::ClickUltimateSkill()
{
	OnClickUltimateSkill.Broadcast();
}

void UInBattleHUDWidget::ClickBasicSkill()
{
	OnClickBasicSkill.Broadcast();
}

void UInBattleHUDWidget::ClickWeaponSwap()
{
	OnClickWeaponSwap.Broadcast();
}

void UInBattleHUDWidget::ClickItem1()
{
	OnClickItem1.Broadcast();
}

void UInBattleHUDWidget::ClickItem2()
{
	OnClickItem2.Broadcast();
}

void UInBattleHUDWidget::ToggleAutoManualMode(bool bIsChecked)
{
	OnToggleAutoManualMode.Broadcast(bIsChecked);
}

// ========================================================
// 바인딩
// ========================================================
void UInBattleHUDWidget::BindCallbacks()
{
	BasicAttack->OnButtonClick.AddUObject(this, &UInBattleHUDWidget::ClickBasicAttack);
	UltimateSkill->OnButtonClick.AddUObject(this, &UInBattleHUDWidget::ClickUltimateSkill);
	BasicSkill->OnButtonClick.AddUObject(this, &UInBattleHUDWidget::ClickBasicSkill);
	WeaponSwap->OnButtonClick.AddUObject(this, &UInBattleHUDWidget::ClickWeaponSwap);
	Item1->OnButtonClick.AddUObject(this, &UInBattleHUDWidget::ClickItem1);
	Item2->OnButtonClick.AddUObject(this, &UInBattleHUDWidget::ClickItem2);
	AutoManualMode->OnToggle.AddDynamic(this, &UInBattleHUDWidget::ToggleAutoManualMode);
}


