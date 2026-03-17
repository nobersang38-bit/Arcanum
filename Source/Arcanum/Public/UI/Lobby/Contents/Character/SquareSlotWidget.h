#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "SquareSlotWidget.generated.h"

class UImage;
class UBorder;
class UTextBlock;
class UItemTooltipWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSlotClicked,USquareSlotWidget*, ClickedSlot,int32, SlotIndex);
/**
 * 
 */
UCLASS()
class ARCANUM_API USquareSlotWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Slot")
	FOnWeaponSlotClicked OnSlotClicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BackColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> IconImg;

	/// 260312 변경 (추가 태그)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00-Global") FGameplayTag WidgetTag;

	UFUNCTION(BlueprintCallable)
	void SetSquareBackgroundColor(FLinearColor NewColor);

	UFUNCTION(BlueprintCallable)
	void SetItemIconImage(UTexture2D* ItemIcon);

	UFUNCTION(BlueprintCallable)
	void SetItemName(FText InText);
	void SetWeaponTag(FGameplayTag InTag);
	void SetWeaponGuid(FGuid InGuid);
	FText GetItemName() const { return EquipNameTxt; }
	FGameplayTag GetWeaponTag() const { return WeaponTag; }
	FGuid GetWeaponGuid() const { return EquippedItemGuid; }
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBorder> BackgroundColor;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	int32 SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot")
	FGameplayTag WeaponTag;

	UPROPERTY()
	FText EquipNameTxt;
	UPROPERTY()
	FGuid EquippedItemGuid;

#pragma region 강화 수치 표시
public:
	UFUNCTION(BlueprintCallable)
	void SetUpgradeLevel(int32 InUpgradeLevel);

	UFUNCTION(BlueprintCallable)
	void ClearUpgradeLevel();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeLevelText;
#pragma endregion

#pragma region 장착 아이템 툴팁
protected:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/* 툴팁 갱신 */
	void RefreshTooltip();

protected:
	/* 아이템 툴팁 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tooltip")
	TSubclassOf<UItemTooltipWidget> ItemTooltipWidgetClass;
#pragma endregion
};
