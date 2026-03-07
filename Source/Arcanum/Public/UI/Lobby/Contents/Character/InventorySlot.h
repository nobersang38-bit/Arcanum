// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/BattleCharacter/Equipment/Data/FEquipmentData.h"
#include "InventorySlot.generated.h"

class UCommonBtnWidget;
class UTextBlock;
class USquareSlotWidget;
class UWrapBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetupBtnClicked);

/**
 * 
 */
UCLASS()
class ARCANUM_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EquipNameTxt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText EquipInfoTxt;

	UPROPERTY(BlueprintAssignable)
	FOnSetupBtnClicked OnSetupBtnClicked;

	UFUNCTION()
	void CreateWeaponItems(TArray<FEquipmentInfo> WeaponList, const FString& TargetPath);
private:
	UFUNCTION()
	void ClickEquipSetupBtn();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SetupBtn;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EquipNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EquipInfoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> EquipGridPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	TSubclassOf<USquareSlotWidget> USquareSlotWidgetClass;

	UPROPERTY()
	UTexture2D* WeaponInventoryItemIcon = nullptr;

	UFUNCTION()
	bool IsSpecificSlotType(const FGameplayTag& InTag, const FString& TargetPath);

protected:
	UPROPERTY()
	class UGameDataSubsystem* DataSubsystem;
};
