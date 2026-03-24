#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "CharacterEquipWidget.generated.h"

class UInventoryHUDWidget;
class UCommonBtnWidget;
class UTextBlock;
class UBorder;
class UItemStatPanelWidget;
class USetEffectPanelWidget;
struct FInventoryViewSlot;

/* 장착 버튼 클릭 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterEquipRequested, const FGameplayTag&, InEquipSlotTag, const FGuid&, InItemGuid);

/* 해제 버튼 클릭 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterUnequipRequested, const FGameplayTag&, InEquipSlotTag);

UCLASS()
class ARCANUM_API UCharacterEquipWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/* 현재 장착 대상 슬롯 설정 + 내부 인벤 필터 갱신 */
	void SetEquipSlotTag(const FGameplayTag& InEquipSlotTag);

	/* 방어구 전체 인벤토리 표시 */
	void ShowEquipmentInventory();

	/* 인벤 선택 초기화 */
	void ClearSelectedItem();

	/* 장착 버튼 활성화 갱신 */
	void RefreshButtonState();

	UItemStatPanelWidget* GetArmorStatPanelWidget() const { return ArmorStatPanelWidget; }
	UInventoryHUDWidget* GetInventoryHUDWidget() const { return InventoryHUDWidget; }
	USetEffectPanelWidget* GetSetEffectPanelWidget() const { return SetEffectPanelWidget; }

private:
	UFUNCTION()
	void HandleInventorySlotSelected(const FInventoryViewSlot& InSlot);

	UFUNCTION()
	void ClickEquipBtn();

	UFUNCTION()
	void ClickUnequipBtn();

public:
	UPROPERTY(BlueprintAssignable, Category = "CharacterEquip")
	FOnCharacterEquipRequested OnCharacterEquipRequested;

	UPROPERTY(BlueprintAssignable, Category = "CharacterEquip")
	FOnCharacterUnequipRequested OnCharacterUnequipRequested;

protected:
	/* 내부 장비 인벤 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryHUDWidget> InventoryHUDWidget;

	/* 장착 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> EquipBtn;

	/* 해제 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> UnequipBtn;

	/* TODO : 선택 아이템 이름 표시용 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> EquipNameText;

	/* TODO : 선택 아이템 옵션 표시용 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> EquipInfoText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemStatPanelWidget> ArmorStatPanelWidget;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<USetEffectPanelWidget> SetEffectPanelWidget;

private:
	/* 현재 장착 대상 슬롯 태그 */
	UPROPERTY()
	FGameplayTag CurrentEquipSlotTag;

	/* 현재 선택된 인벤 장비 Guid */
	UPROPERTY()
	FGuid SelectedItemGuid;
};