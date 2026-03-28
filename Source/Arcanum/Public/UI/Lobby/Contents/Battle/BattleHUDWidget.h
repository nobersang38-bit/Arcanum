#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/StageData/StageInfo/Data/FStageDataInfo.h"
#include "DataInfo/StageData/StageInfo/DataTable/FDTStageDataRow.h"
#include "DataInfo/InventoryData/Data/FInventoryViewSlot.h"
#include "BattleHUDWidget.generated.h"

class ULobbyHUD;
class UCommonBtnWidget;
class UStageList;
class UScrollBox;
class UItemSlot;
class UInventoryItemSlotWidget;
class USquareSlotWidget;

/* 배틀 슬롯 클릭 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowUnitList, int32, InIndex);

UCLASS()
class ARCANUM_API UBattleHUDWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region 기본
public:
	void SetParentLobby(ULobbyHUD* InLobby);
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY()
	TObjectPtr<ULobbyHUD> ParentLobby;
#pragma endregion


#pragma region 전투 처리
private:
	/* 전투 시작 */
	UFUNCTION()
	void EnterGame();

protected:
	/* 게임 시작 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> EnterGameBtn;
#pragma endregion

#pragma region 물약 아이템 장착
public:
	/* 장작 슬롯 화면 갱신 */
	void RefreshBattlePotionSlots();

	/* 물약 인벤토리 닫기 */
	void HidePotionInventory();
private:
	/* 물약 슬롯 클릭 */
	UFUNCTION()
	void HandleEquippedPotionSlot1Clicked(int32 InIgnoredIndex);
	UFUNCTION()
	void HandleEquippedPotionSlot2Clicked(int32 InIgnoredIndex);
	void HandleEquippedPotionSlotClicked(int32 InSlotIndex);

	/* 인벤 슬롯 선택 */
	UFUNCTION()
	void HandleInventorySlotSelected(const FInventoryViewSlot& InSlot);

	/* 장착 버튼 */
	UFUNCTION()
	void HandleSetItemBtnClicked();

	/* 해제 버튼 */
	UFUNCTION()
	void HandleRemoveItemBtnClicked();

	/* 물약 인벤토리 열기 */
	void ShowPotionInventory();

	/* 장착 슬롯 선택 강조 갱신 */
	void RefreshEquippedPotionSlotSelection();

protected:
	/* 전투에 들고 갈 슬롯당 물약 개수 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
	int32 EquippedPotionCount = 5;

	/* 물약 장착 슬롯 1 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryItemSlotWidget> EquippedItemSlot1;

	/* 물약 장착 슬롯 2 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryItemSlotWidget> EquippedItemSlot2;

	/* 아이템 선택 패널 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlot> ItemListSlot;

private:
	/* 화면 슬롯 위젯 배열 */
	UPROPERTY()
	TArray<TObjectPtr<UInventoryItemSlotWidget>> EquippedPotionSlots;

	/* 실제 장착 데이터 배열 */
	UPROPERTY()
	TArray<FInventoryViewSlot> EquippedPotionSlotDatas;

	/* 현재 선택된 물약 슬롯 인덱스 */
	UPROPERTY()
	int32 SelectedPotionSlotIndex = INDEX_NONE;

	/* 현재 인벤에서 선택된 물약 슬롯 데이터 */
	UPROPERTY()
	FInventoryViewSlot SelectedInventorySlot;
#pragma endregion

#pragma region 유닛 슬롯
protected:
	UFUNCTION()
	void SetUnit();

	UFUNCTION()
	void OnBattleSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex);

public:
	UPROPERTY(BlueprintAssignable)
	FOnShowUnitList OnShowUnitList;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedUnitSlot1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedUnitSlot2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedUnitSlot3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedUnitSlot4;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlot> UnitListSlot;
#pragma endregion

#pragma region 배틀 배너
protected:
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UScrollBox> StageScrollBox;
	UPROPERTY(EditAnywhere) TSubclassOf<UStageList> StageListClass;
private:
	TArray<FDTStageDataRow*> StageDatas;
	UFUNCTION() void OnStageClicked(UStageList* ClickedStage);
	TArray<UStageList*> StageWidgets;
	UStageList* CurrentSelectedStage = nullptr;
#pragma endregion
};
