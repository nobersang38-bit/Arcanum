// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "PlayerHUD.generated.h"

/**
 * 추영호
 * - 플레이어 HUD
 */

class USkillComponent;
class UEquipmentComponent;
class UBasicAttackWidget;
class UAwakenWidget;
enum class EEquipSlot : uint8;

UCLASS()
class ARCANUM_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

#pragma region 오버라이드
protected:
	/* 위젯 생성 시 초기 바인딩 */
	virtual void NativeConstruct() override;

	/* 위젯 제거 시 델리게이트 정리 */
	virtual void NativeDestruct() override;
#pragma endregion

#pragma region 초기화
private:
	/* 소유 Pawn에서 스킬/장비 컴포넌트 찾아 캐시 */
	void BindOwnerComponents();

	/*	하위 위젯 클릭 이벤트 바인딩 */
	void BindWidgetEvents();

	/* 버튼 입력 이벤트 바인딩 */
	void BindButtonEvents();

	/* 컴포넌트 델리게이트 바인딩 */
	void BindComponentDelegates();

	/* 시작할 때 UI 한 번 갱신 */
	void RefreshAllUI();
#pragma endregion

#pragma region 하위 위젯 이벤트
private:
	/* BasicAttackWidget 클릭 요청 */
	UFUNCTION()
	void HandleBasicAttackClicked();

	/* AwakenWidget 클릭 요청 */
	UFUNCTION()
	void HandleAwakenClicked();
#pragma endregion

#pragma region 컴포넌트 콜백
private:
	/* 장비 변경 시(무기 스왑 포함) 기본공격 UI 갱신용 */
	UFUNCTION()
	void HandleEquipChanged(EEquipSlot InSlot, FGameplayTag InNewEquipTag);

	/* 게이지 퍼센트 변경 (0~1) */
	UFUNCTION()
	void HandleGaugePercentChanged(float InPercent);

	/* 각성 시작/종료 상태 변경 */
	UFUNCTION()
	void HandleAwakenStateChanged(bool bInAwakenActive, FGameplayTag InAwakenTag);
#pragma endregion

#pragma region 위젯 바인딩
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBasicAttackWidget> BasicAttackWidget = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UAwakenWidget> AwakenWidget = nullptr;
#pragma endregion

private:
	UPROPERTY(Transient)
	TObjectPtr<USkillComponent> SkillComponent = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UEquipmentComponent> EquipmentComponent = nullptr;
};
