// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "CharacterHUDWidget.generated.h"

class URoundedSlotWidget;
class USquareSlotWidget;
class UUniformGridPanel;
class UCommonDialog;
class UCharacterInfo;
class UWidgetSwitcher;
class UInventorySlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnhanceOKClicked);

/**
 * 
 */
UCLASS()
class ARCANUM_API UCharacterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;
#pragma endregion

#pragma region 바인딩
	// 캐릭터창, 유닛창, 장비창, 캐릭터 설명창, 캐릭터 강화창, 장비 인벤토리
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> CharacterGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> UnitGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterInfo> CharacterInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlot> WeaponList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlot> EquipmentList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonDialog> EnhancementConfirm;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> Weapon1Slot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> Weapon2Slot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> LegendaryWeaponSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> HelmetSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> ChestSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> GloveSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USquareSlotWidget> BootsSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher>CharacterSwitcher;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Slot")
	TSubclassOf<URoundedSlotWidget> RoundedSlotWidgetClass;

	UFUNCTION()
	void ShowEnhancementConfirm();

	UFUNCTION()
	void SetupEquipment();

public:
	UPROPERTY(BlueprintAssignable)
	FOnEnhanceOKClicked OnEnhanceOKClicked;


private:
	UFUNCTION()
	void OnEnhancementCommonDialog(EDialogResult res);
	
	UFUNCTION()
	void OnSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex);

#pragma endregion

};
