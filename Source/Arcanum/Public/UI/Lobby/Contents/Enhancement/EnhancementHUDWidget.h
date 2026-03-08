#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancementHUDWidget.generated.h"

class ULobbyHUD;
class UInventoryHUDWidget;
class UInventoryItemSlotWidget;
class UTextBlock;
class UButton;
struct FInventoryViewSlot;
struct FEquipmentInfo;
struct FDTItemCatalogRow;
struct FDTEquipmentInfoRow;

/**
 * 추영호
 * 강화 UI
 */

UCLASS()
class ARCANUM_API UEnhancementHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

#pragma region 부모 로비 참조
public:
	/* 로비가 생성/바인딩 후 주입 */
	void SetParentLobby(ULobbyHUD* InLobby);

private:
	UPROPERTY()
	TObjectPtr<ULobbyHUD> ParentLobby;
#pragma endregion

#pragma region 강화 장비 인벤토리
public:
	/* 장비 인벤토리 */
	void RefreshEquipmentInventory();

protected:
	/* 기존 인벤 위젯 재사용 (강화창에 배치) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryHUDWidget> EnhancementInventoryWidget;
#pragma endregion

#pragma region 선택 장비 표시
public:
	/* 선택 장비 표시 슬롯 */
	UFUNCTION()
	void HandleEnhancementItemSelected(const FInventoryViewSlot& InSlot);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryItemSlotWidget> SelectedItemSlotWidget;
#pragma endregion

#pragma region 강화 정보 표시
private:
	/* 실제 장비 데이터 조회 */
	const FEquipmentInfo* FindSelectedEquipment(const FGuid& InItemGuid) const;

	/* 강화 정보창 전체 갱신 */
	void RefreshEnhancementInfo(const FInventoryViewSlot& InSlot);

	/* 강화 정보 텍스트 채움 */
	void ApplyEnhancementInfo(const FDTItemCatalogRow* InCatalogRow, const FDTEquipmentInfoRow* InEquipRow, const FEquipmentInfo* InEquipInfo);

	/* 정보창 초기화 */
	void ClearEnhancementInfo();

protected:
	/* 장비 이름 + 현재 강화 수치 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentUpgradeText;

	/* 현재 구간 제목 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTitleText;

	/* 현재 스탯 1줄 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentStatText1;

	/* 현재 스탯 2줄 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentStatText2;

	/* 강화 후 구간 제목 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NextTitleText;

	/* 강화 후 스탯 1줄 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NextStatText1;

	/* 강화 후 스탯 2줄 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NextStatText2;

	/* 강화 성공 확률 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> EnhanceChanceText;
#pragma endregion


#pragma region 장비 강화
private:
	/* 선택된 장비 강화 버튼 클릭 */
	UFUNCTION()
	void HandleEnhanceButtonClicked();

protected:
	/* 강화 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> EnhanceButton;

	/* 현재 선택된 장비 Guid */
	UPROPERTY()
	FGuid SelectedItemGuid;
#pragma endregion
};
