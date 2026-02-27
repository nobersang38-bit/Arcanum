#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "DataInfo\BattleCharacter\Equipment\DataTable\DTEquipment.h"
#include "ShopItemSlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UBorder;
struct FDTEquipmentInfoRow;

/* 슬롯 클릭 시 슬롯 인덱스 전달 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShopItemSlotClicked, int32, InSlotIndex);

/**
 * 추영호
 * - 아이템 구입 슬롯 UI
 */
UCLASS()
class ARCANUM_API UShopItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	/* 로비HUD가 만든 DT RowPtr로 슬롯 표시만 세팅 */
	void SetViewData(int32 InSlotIndex, FName InRowName, const FDTEquipmentInfoRow* InRowPtr, bool InSoldOut);

	/* 장비 슬롯 표시 데이터 세팅 */
	UFUNCTION(BlueprintCallable)
	void SetEquipmentData(const FDTEquipmentInfoRow& InRow, int32 InSlotIndex, FName InRowName);

	/* 빈 슬롯 초기화 */
	UFUNCTION(BlueprintCallable)
	void ClearSlot();

	/* 선택 강조 표시 */
	UFUNCTION(BlueprintCallable)
	void SetSelected(bool InSelected);

	/* 품절 표시 */
	UFUNCTION(BlueprintCallable)
	void SetSoldOut(bool InSoldOut);

	/* 구매 가능한 슬롯인지 확인 */
	UFUNCTION(BlueprintCallable)
	bool IsPurchasable() const;

	/* 슬롯 인덱스 반환 */
	UFUNCTION(BlueprintCallable)
	int32 GetSlotIndex() const { return SlotIndex; }

	/* 원본 RowName 반환 (구매 시 사용) */
	UFUNCTION(BlueprintCallable)
	FName GetRowName() const { return RowName; }

	/* 장비 태그 반환 */
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetItemTag() const { return EquipmentRowPtr ? EquipmentRowPtr->ItemTag : FGameplayTag(); }

private:
	/* 슬롯 버튼 클릭 처리 */
	UFUNCTION()
	void HandleSlotButtonClicked();

	/* 슬롯 UI 갱신 */
	void RefreshSlotUI();


public:
	UPROPERTY(BlueprintAssignable, Category = "Shop")
	FOnShopItemSlotClicked OnShopItemSlotClicked;

protected:
	/* 불투명도 */
	UPROPERTY(EditAnywhere, Category = "shop")
	float SoldOutOpacity = 0.5f;

private:
	/* 슬롯 클릭 버튼 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> SlotButton;

	/* 아이콘 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> ItemIconImage;

	/* 아이템명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> ItemNameText;

	/* 설명 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> DescText;

	/* 가격 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> PriceText;

	/* 슬롯 보더 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UBorder> SlotBorder;

	/* 선택 강조 보더 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	TObjectPtr<UBorder> SelectedBorder;

private:
	/* 장비 데이터 캐시 */
	const FDTEquipmentInfoRow* EquipmentRowPtr = nullptr;

	/* 슬롯 번호 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex = INDEX_NONE;

	/* DT RowName */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName RowName = NAME_None;

	/* 빈 슬롯 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bEmpty = true;

	/* 품절 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bSoldOut = false;

	/* 선택 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bSelected = false;
};
