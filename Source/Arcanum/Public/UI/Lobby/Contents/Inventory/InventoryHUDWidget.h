// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryHUDWidget.generated.h"

class UScrollBox;
class UWrapBox;
class UCommonBtnWidget;
class UInventoryItemSlotWidget;
struct FEquipmentInfo;
struct FDTEquipmentInfoRow;

/* 선택된 아이템 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotSelected, FGuid, InItemGuid);

/**
 * 추영호 
 * - 인벤토리 HUD
 * - 슬롯 생성
 */

UCLASS()
class ARCANUM_API UInventoryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;


public:
	/* 인벤 목록/RowPtr로 화면 갱신 */
	void ApplyInventoryData(const TArray<FEquipmentInfo>& InItems, const TArray<const FDTEquipmentInfoRow*>& InRowPtrs);

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
	void HandleSlotClicked(FGuid InItemGuid);

	/* 선택 강조 갱신 */
	void RefreshSelection();

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventorySlotSelected OnInventorySlotSelected;

protected:
	/* 스크롤 컨테이너 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UScrollBox> ScrollBox;

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
	TArray<FGuid> CachedItemGuids;

	/* 현재 선택 슬롯 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGuid SelectedGuid;
};
