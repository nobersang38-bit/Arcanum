#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

class UCommonBtnWidget;
class UInventoryHUDWidget;

/* 장착 버튼 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSetItemBtnClicked);

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

public:
	UPROPERTY(BlueprintAssignable, Category = "ItemSlot")
	FOnSetItemBtnClicked OnSetItemBtnClicked;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SetItemBtn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryHUDWidget> BattleInventoryWidget;
};