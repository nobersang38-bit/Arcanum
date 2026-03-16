#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

class UCommonBtnWidget;
class UInventoryHUDWidget;
class UCommonBtnWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetItemBtnClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRemoveItemBtnClicked);

/**
 * 추영호
 * 전투 물약 선택 패널
 */

UCLASS()
class ARCANUM_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/* 인벤토리 */
	void ShowBattleInventory();

	/* 전투 인벤토리 위젯 반환 */
	UInventoryHUDWidget* GetBattleInventoryWidget() const { return BattleInventoryWidget; }

private:
	/* 장착 버튼 */
	UFUNCTION()
	void ClickSetItemBtn();

	/* 해제 버튼 */
	UFUNCTION()
	void ClickRemoveBtn();

	UFUNCTION()
	void ClickCloseBtn();
public:
	UPROPERTY(BlueprintAssignable)
	FOnSetItemBtnClicked OnSetItemBtnClicked;

	UPROPERTY(BlueprintAssignable)
	FOnRemoveItemBtnClicked OnRemoveItemBtnClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SetItemBtn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> RemoveBtn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> CloseBtn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryHUDWidget> BattleInventoryWidget;
};