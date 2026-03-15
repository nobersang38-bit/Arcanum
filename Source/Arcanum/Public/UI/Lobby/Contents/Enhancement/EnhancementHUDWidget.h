#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancementHUDWidget.generated.h"

class ULobbyHUD;
class UInventoryHUDWidget;
class UInventoryItemSlotWidget;
class UItemTitlePanelWidget;
class UItemStatPanelWidget;
class UTextBlock;
class UCommonBtnWidget;
struct FInventoryViewSlot;
struct FEquipmentInfo;

/**
 * 추영호
 * 강화 UI
 */

UCLASS()
class ARCANUM_API UEnhancementHUDWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region 초기화
protected:
	virtual void NativeConstruct() override;

private:
	/* 선택된 장비 슬롯 UI/강화 정보 갱신 */
	void RefreshSelectedItemSlot();
#pragma endregion

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
	/* 선택 장비 슬롯 */
	UFUNCTION()
	void HandleEnhancementItemSelected(const FInventoryViewSlot& InSlot);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryItemSlotWidget> SelectedItemSlotWidget;
#pragma endregion

#pragma region 장비 강화
private:
	/* 선택된 장비 강화 버튼 클릭 */
	UFUNCTION()
	void HandleEnhanceButtonClicked();

protected:
	/* 강화 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> EnhanceButton;

	/* 현재 선택된 장비 Guid */
	UPROPERTY()
	FGuid SelectedItemGuid;
#pragma endregion

#pragma region 장비 리롤
private:
	/* 선택된 장비 리롤 버튼 클릭 */
	UFUNCTION()
	void HandleRerollButtonClicked();

protected:
	/* 리롤 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> RerollButton;
#pragma endregion

#pragma region 장비 분해
private:
	/* 선택된 장비 분해 버튼 클릭 */
	UFUNCTION()
	void HandleDisassembleButtonClicked();

protected:
	/* 분해 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> DisassembleButton;
#pragma endregion

#pragma region 상세 확률표
private:
	/* 상세 확률표 버튼 클릭 */
	UFUNCTION()
	void HandleDetailChanceButtonClicked();

	/* 상세 확률표 텍스트 갱신 */
	void RefreshDetailChanceText();

protected:
	/* 상세 확률표 열기 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> DetailChanceButton;

	/* 상세 확률표 패널 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidget> DetailChancePanel;

	/* 상세 확률표 텍스트 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DetailChanceText;
#pragma endregion

#pragma region 강화 정보 표시
private:
	/* 가운데 강화 정보 갱신 */
	void RefreshEnhancementInfo();

	/* 강화 정보창 전체 초기화 */
	void ClearEnhancementInfo();

protected:
	/* 가운데 아이템 이름 패널 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemTitlePanelWidget> ItemTitlePanelWidget;

	/* 가운데 현재 능력치 패널 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemStatPanelWidget> CurrentStatPanelWidget;

	/* 가운데 강화 후 능력치 패널 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemStatPanelWidget> NextStatPanelWidget;

	/* 강화 성공 확률 텍스트 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> EnhanceChanceText;

#pragma endregion
};
