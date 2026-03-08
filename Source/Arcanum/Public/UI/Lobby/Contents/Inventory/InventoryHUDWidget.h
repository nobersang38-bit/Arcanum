#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/InventoryData/Data/InventoryViewSlot.h"
#include "InventoryHUDWidget.generated.h"

class ULobbyHUD;
class UWrapBox;
class UCommonBtnWidget;
class UInventoryItemSlotWidget;

UENUM(BlueprintType)
enum class EInventoryCategoryFilter : uint8
{
	All,
	Equipment,
	Consumable
};

/* 장비/포션 공용 선택된 아이템 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotSelected, const FInventoryViewSlot&, InSlot);

/* 카테고리 변경 알림 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryCategoryChanged, EInventoryCategoryFilter, InFilter);

/**
 * 추영호 
 * - 인벤토리 HUD
 * - 슬롯 생성 + 표시(ViewSlot 기반)
 */

UCLASS()
class ARCANUM_API UInventoryHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

#pragma region 부모 로비 참조
public:
	void SetParentLobby(ULobbyHUD* InLobby) { ParentLobby = InLobby; }
private:
	UPROPERTY()
	TObjectPtr<ULobbyHUD> ParentLobby;
#pragma endregion

#pragma region 인벤 슬롯 생성
public:
	/* 인벤 UI 갱신 */
	void RefreshInventoryUI();

	/* 장비 전용 표시 슬롯 생성 */
	void RefreshEquipmentInventory();
	/* 스택 전용 표시 슬롯 생성 */
	void RefreshStackInventory();

	/* 현재 카테고리 강제 설정 */
	void SetCurrentFilter(EInventoryCategoryFilter InFilter) { CurrentFilter = InFilter; }

private:
	/* 표시용 슬롯 생성 */
	void BuildInventoryViewSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const;

	/* 스택형 아이템 슬롯 추가 */
	void AppendStackItemSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const;

	/* Guid 아이템 슬롯 추가 */
	void AppendGuidSlots(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const;

	/* Guid 아이템 정렬 추가 */
	void AppendGuidSlotsSorted(TArray<FInventoryViewSlot>& OutSlots, int32 InSlotLimit) const;
#pragma endregion

#pragma region 인벤 정렬 버튼
private:
	/* 정렬 버튼 클릭 토글 */
	UFUNCTION()
	void ClickInventorySortBtn();

protected:
	/* 인벤 정렬 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> InventorySortBtn;

private:
	/* 정렬 토글 상태 (true 정렬)*/
	UPROPERTY()
	bool bInventorySortedView = false;
#pragma endregion

public:
	/* 공용 슬롯 리스트로 화면 갱신 */
	void ApplyInventorySlots(const TArray<FInventoryViewSlot>& InSlots);

	/* 선택 초기화 */
	void ClearSelection();

	/* 슬롯 생성 초기화  */
	void InitInventorySlots(int32 InSlotCount);

	/* 현재 선택 장비 Guid */
	FGuid GetSelectedInventoryItemGuid() const { return SelectedInventoryItemGuid; }

	/* 현재 선택 스택 태그 */
	FGameplayTag GetSelectedStackItemTag() const { return SelectedStackItemTag; }

	/* 현재 선택 스택 수량 */
	int32 GetSelectedStackItemCount() const { return SelectedStackItemCount; }

	/* 현재 필터 Getter */
	EInventoryCategoryFilter GetCurrentFilter() const { return CurrentFilter; }

protected:
	/* SlotCount만큼 슬롯 위젯 생성 (컨테이너) */
	void CreateInventorySlots(int32 InSlotCount);

	/* 슬롯 클릭 바인딩 */
	void BindSlotEvents();

	/* 슬롯 클릭 처리 (선택 유지) */
	UFUNCTION()
	void HandleSlotClicked(int32 InSlotIndex);

	/* 선택 강조 갱신 */
	void RefreshSelection();

	// 카테고리 버튼 클릭 함수
	UFUNCTION()
	void HandleAllCategoryClicked();
	UFUNCTION()
	void HandleEquipmentCategoryClicked();
	UFUNCTION()
	void HandleConsumableCategoryClicked();

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventorySlotSelected OnInventorySlotSelected;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryCategoryChanged OnCategoryChanged;

protected:
	/* 슬롯 배치 컨테이너 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UWrapBox> SlotContainer;

	/* 슬롯 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventoryItemSlotWidget> InventoryItemSlotWidgetClass;

	/* 생성된 슬롯들 */
	UPROPERTY()
	TArray<TObjectPtr<UInventoryItemSlotWidget>> Slots;

	/* UI 표시용 데이터 캐시 */
	UPROPERTY()
	TArray<FInventoryViewSlot> CachedViewSlots;

	/* 현재 선택 슬롯 인덱스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 SelectedSlotIndex = INDEX_NONE;

	/* 현재 선택된 장비 Guid */
	UPROPERTY()
	FGuid SelectedInventoryItemGuid;

	/* 현재 선택된 스택 태그 */
	UPROPERTY()
	FGameplayTag SelectedStackItemTag;

	/* 현재 선택된 스택 수량 */
	UPROPERTY()
	int32 SelectedStackItemCount = 0;

	// 카테고리 버튼 3개
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UCommonBtnWidget> AllCategoryBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UCommonBtnWidget> EquipmentCategoryBtn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UCommonBtnWidget> ConsumableCategoryBtn;

private:
	// 현재 카테고리 필터
	UPROPERTY()
	EInventoryCategoryFilter CurrentFilter = EInventoryCategoryFilter::All;
};
