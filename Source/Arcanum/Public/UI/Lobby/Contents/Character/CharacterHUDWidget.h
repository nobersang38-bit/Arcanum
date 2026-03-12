// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "GameplayTags/ArcanumTags.h"
#include "Core/ARGameInstance.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "CharacterHUDWidget.generated.h"

class URoundedSlotWidget;
class USquareSlotWidget;
class UCommonDialog;
class UCharacterInfo;
class UWidgetSwitcher;
class UInventorySlot;
class UWrapBox;
class ULobbyHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnhanceOKClicked,int32, RequiredSoul);

/**
 *  김유진
 *  역할 : 캐릭터 탭 (캐릭터 선택, 캐릭터 강화, 장비 장착)
 */
UCLASS()
class ARCANUM_API UCharacterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;
#pragma endregion

#pragma region 바인딩
	// 캐릭터창, 유닛창, 장비창, 캐릭터 설명창, 캐릭터 강화창, 장비 인벤토리
public:
	void SetParentLobby(ULobbyHUD* InLobby) { ParentLobby = InLobby; }

private:
	UPROPERTY()
	TObjectPtr<ULobbyHUD> ParentLobby;
	//ParentLobby->CachedPlayer
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> CharacterGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> UnitGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterInfo> CharacterInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventorySlot> WeaponList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonDialog> SetPlayerConfirm;

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
	TObjectPtr<USquareSlotWidget> BootSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher>CharacterSwitcher;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Slot")
	TSubclassOf<URoundedSlotWidget> RoundedSlotWidgetClass;

	UPROPERTY()
	TArray<URoundedSlotWidget*> CreatedCharacterSlots;
	UPROPERTY() TArray<URoundedSlotWidget*> CreatedServantCharacterSlots;

	UFUNCTION()
	void CharacterEnhancement(FText CharacterName,int32 InRequiredSoul);
	UFUNCTION()
	void SetPlayerCharacter(FText CharacterName);

	UFUNCTION()
	void UpdateCharacterInfo(FName CharacterName, bool bSetCharacter, bool SlotCharacterOwned, FText InFinalText, FText ButtonText, int64 soulAmount);

	UFUNCTION()
	//void SetupEquipment(bool bNewEquipped);
	void SetupEquipment(USquareSlotWidget* ClickedSlot, int32 SlotIndex);
	
	int GetCurrentGrade;
	int32 RequiredSoul;
	// 무기, 장비 슬롯
	UPROPERTY()
	TArray<USquareSlotWidget*> EquipmentSlots;


public:
	UPROPERTY(BlueprintAssignable)
	FOnEnhanceOKClicked OnEnhanceOKClicked;

	UPROPERTY()
	UARGameInstance* GI;

	UPROPERTY()
	UGameDataSubsystem* DataSubsystem;

	void InitCharacterHUD();
	void InitServantCharacter();

	//void InitWeaponInventory(int32 SlotIndex);
	void InitWeaponInventory(FGameplayTag SlotTag);
	void InitEquipment(FName CharacterName);


private:
	UFUNCTION()
	void OnSquareSlotClicked(USquareSlotWidget* ClickedSlot, int32 SlotIndex);
	UFUNCTION()
	void OnCharacterSlotSelected(URoundedSlotWidget* ClickedSlot, FName CharacterName, bool SlotCharacterOwned);

	int32 CharacterStar = 0;
	int32 CharacterGrade = 0;
	int32 TargetGradeIndex = 0;
	FText FinalText;
	FString CombinedInfoString;
	FText ButtonText;

	int32 SelectedIndex = INDEX_NONE;
	FName CurrentSelectedCharacterName; // 선택된 캐릭터

	UPROPERTY()
	UTexture2D* WeaponSlotItemIcon = nullptr;

	void UpdateSlotVisuals(const TMap<FGameplayTag, FGuid>& InEquipmentMap);
#pragma endregion

};
