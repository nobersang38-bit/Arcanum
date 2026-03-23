// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/Contents/InBattleHUDWidget.h"
#include "UI/Battle/SubLayout/BattleActionButtonWidget.h"
#include "UI/Battle/SubLayout/BattleAllyUnitPanelWidget.h"
#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "UI/Battle/SubLayout/BattleCostBarWidget.h"
#include "UI/Battle/SubLayout/BattleHealthBarWidget.h"
#include "UI/Battle/SubLayout/BattleStageProgressWidget.h"
#include "UI/Battle/SubLayout/BattleToggleWidget.h"
#include "Object/Operation/UnitDragDropOperation.h"
#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"

// ========================================================
// 언리얼 기본 생성 및 초기화
// ========================================================
void UInBattleHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindCallbacks();
}

//bool UInBattleHUDWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
//{
//	// 1. 들어온 Operation이 우리가 만든 UnitDragDropOperation인지 확인(캐스팅)
//	UUnitDragDropOperation* UnitDragOp = Cast<UUnitDragDropOperation>(InOperation);
//
//	if (APlayerController* PC = GetOwningPlayer())
//	{
//		if (UnitDragOp && UnitDragOp->UnitTag.IsValid())
//		{
//			FHitResult HitResult;
//			// 2. 마우스 커서 위치에서 아래로 Raycast (채널은 상황에 맞게 변경: 예: ECC_Visibility)
//			bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
//
//			if (bHit)
//			{
//				SelectedUnitDropLocation = HitResult.Location;
//
//				SelectedUnitTag = UnitDragOp->UnitTag;
//
//				UE_LOG(LogTemp, Error, TEXT("Tag : %s\tLocation : %.0f, %.0f, %.0f"), *SelectedUnitTag.ToString(), SelectedUnitDropLocation.X, SelectedUnitDropLocation.Y, SelectedUnitDropLocation.Z);
//				if (UnitDragOp->Slot.IsValid())
//				{
//					UnitDragOp->Slot->OnReleasedUnitSlot.Broadcast(SelectedUnitTag);
//				}
//				// 드롭 처리를 성공적으로 마쳤으므로 true 반환
//				return true;
//			}
//		}
//	}
//
//	// 처리에 실패했거나 다른 종류의 DragDrop일 경우 부모 함수 호출
//	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
//}

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
	StageProgress->SetTimeSecond(TimeMS);
}

// ========================================================
// 전투 관련 버튼들
// ========================================================
void UInBattleHUDWidget::ClickBasicAttack()
{
	OnClickBasicAttack.Broadcast();
}

void UInBattleHUDWidget::PressedUltimateSkill()
{
	OnPressedUltimateSkill.Broadcast();
}

void UInBattleHUDWidget::ReleasedUltimateSkill()
{
	OnReleasedUltimateSkill.Broadcast();
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
	BasicAttack->OnButtonClick.AddDynamic(this, &UInBattleHUDWidget::ClickBasicAttack);
	UltimateSkill->OnButtonPressed.AddDynamic(this, &UInBattleHUDWidget::PressedUltimateSkill);
	UltimateSkill->OnButtonReleased.AddDynamic(this, &UInBattleHUDWidget::ReleasedUltimateSkill);
	BasicSkill->OnButtonClick.AddDynamic(this, &UInBattleHUDWidget::ClickBasicSkill);
	WeaponSwap->OnButtonClick.AddDynamic(this, &UInBattleHUDWidget::ClickWeaponSwap);
	Item1->OnButtonClick.AddDynamic(this, &UInBattleHUDWidget::ClickItem1);
	Item2->OnButtonClick.AddDynamic(this, &UInBattleHUDWidget::ClickItem2);
	AutoManualMode->OnToggle.AddDynamic(this, &UInBattleHUDWidget::ToggleAutoManualMode);
}

void UInBattleHUDWidget::RefreshWeaponSkillIcons(UTexture2D* InCurrentWeaponIcon, UTexture2D* InBasicSkillIcon, UTexture2D* InLegendaryWeaponIcon)
{
	if (BasicAttack)
	{
		BasicAttack->SetImage(InCurrentWeaponIcon);
	}

	if (BasicSkill)
	{
		BasicSkill->SetImage(InBasicSkillIcon);
	}

	if (UltimateSkill)
	{
		UltimateSkill->SetImage(InLegendaryWeaponIcon);
	}
}

void UInBattleHUDWidget::SetLegendaryButtonIcon(UTexture2D* InIcon)
{
	if (UltimateSkill)
	{
		UltimateSkill->SetImage(InIcon);
	}
}

void UInBattleHUDWidget::SetBasicAttackCooldown(float InPercent)
{
	if (BasicAttack)
	{
		BasicAttack->SetSkillCooldownPercent(InPercent);
	}
}

void UInBattleHUDWidget::SetBasicSkillCooldown(float InPercent)
{
	if (BasicSkill)
	{
		BasicSkill->SetSkillCooldownPercent(InPercent);
	}
}

void UInBattleHUDWidget::SetUltimateCooldown(float InPercent)
{
	if (UltimateSkill)
	{
		UltimateSkill->SetSkillCooldownPercent(InPercent);
	}
}
