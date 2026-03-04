// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/ItemData/Data/InventoryViewSlot.h"
#include "InventoryHUDWidget.generated.h"

class UScrollBox;
class UWrapBox;
class UCommonBtnWidget;
class UInventoryItemSlotWidget;

/* 장비/포션 공용 선택된 아이템 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotSelected, const FInventoryViewSlot&, InSlot);

/**
 * 추영호 
 * - 인벤토리 HUD
 * - 슬롯 생성 + 표시(ViewSlot 기반)
 */

UCLASS()
class ARCANUM_API UInventoryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;


public:
	/* 공용 슬롯 리스트로 화면 갱신 */
	void ApplyInventorySlots(const TArray<FInventoryViewSlot>& InSlots);

	/* 선택 초기화 */
	void ClearSelection();

	/* 슬롯 생성 초기화  */
	void InitInventorySlots(int32 InSlotCount);

protected:
	/* SlotCount만큼 슬롯 위젯 생성 (컨테이너) */
	void CreateInventorySlots(int32 InSlotCount);

	/* 슬롯 클릭 바인딩 */
	void BindSlotEvents();

	/* 슬롯 클릭 처리 (선택 유지) */
	UFUNCTION()
	void HandleSlotClicked(int32 InSlotIndex);

	/* 선택 강조 갱신 */
	void RefreshSelection();

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventorySlotSelected OnInventorySlotSelected;

protected:
	/* 슬롯 배치 컨테이너 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> SlotContainer;

	/* 슬롯 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItemSlotWidget> InventoryItemSlotWidgetClass;

	/* 생성된 슬롯들 */
	UPROPERTY()
	TArray<TObjectPtr<UInventoryItemSlotWidget>> Slots;

	/* UI 표시용 데이터 캐시 */
	UPROPERTY()
	TArray<FInventoryViewSlot> CachedViewSlots;

	/* 현재 선택 슬롯 인덱스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SelectedSlotIndex = INDEX_NONE;
};
