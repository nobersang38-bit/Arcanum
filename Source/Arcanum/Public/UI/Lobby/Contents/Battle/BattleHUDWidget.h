// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/StageData/StageInfo/DataTable/FDTStageDataRow.h"
#include "DataInfo/StageData/StageInfo/Data/FStageDataInfo.h"
#include "BattleHUDWidget.generated.h"

class UItemSlot;
class USquareSlotWidget;
class UCommonBtnWidget;
class UStageList;
class UScrollBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowItemUnitList,int32, Index);
/**
 * 
 */
UCLASS()
class ARCANUM_API UBattleHUDWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedItemSlot1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedItemSlot2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedUnitSlot1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedUnitSlot2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedUnitSlot3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> EquippedUnitSlot4;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> EnterGameBtn;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlot> ItemListSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UItemSlot> UnitListSlot;

	UPROPERTY(BlueprintAssignable)
	FOnShowItemUnitList OnShowItemUnitList;

	UFUNCTION()
	void SetItemUnit();

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

private:
	UFUNCTION()
	void OnBattleSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex);

	UFUNCTION()
	void EnterGame();
};
