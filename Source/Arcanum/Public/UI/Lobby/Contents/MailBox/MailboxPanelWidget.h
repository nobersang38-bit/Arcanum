#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "MailboxPanelWidget.generated.h"

class ULobbyHUD;
class UWrapBox;
class UCommonBtnWidget;
class UInventoryItemSlotWidget;
class UCommonDialog;

/**
 * 메일 박스 패널
 */
UCLASS()
class ARCANUM_API UMailboxPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetParentLobby(ULobbyHUD* InLobby) { ParentLobby = InLobby; }

	void RefreshMailboxUI();

private:
	/* 메일 슬롯 생성 */
	void CreateMailSlots(int32 InSlotCount);

	/* 슬롯 클릭 바인딩 */
	void BindSlotEvents();

	/* 선택 강조 갱신 */
	void RefreshSelection();

	/* 선택 초기화 */
	void ClearSelection();

	/* 메일 슬롯 클릭 */
	UFUNCTION()
	void HandleMailSlotClicked(int32 InSlotIndex);

	/* 선택 메일 수령 */
	UFUNCTION()
	void ClickReceiveMailBtn();

	/* 전체 메일 수령 */
	UFUNCTION()
	void ClickReceiveAllMailBtn();

protected:
	UPROPERTY()
	TObjectPtr<ULobbyHUD> ParentLobby = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWrapBox> MailSlotContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mailbox")
	TSubclassOf<UInventoryItemSlotWidget> InventoryItemSlotWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UInventoryItemSlotWidget>> MailSlots;

	/* 선택 수령 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> ReceiveMailBtn;

	/* 모두 수령 버튼 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> ReceiveAllMailBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UCommonDialog> MailboxCommonDialog;

private:
	/* 현재 선택 메일 인덱스 */
	UPROPERTY()
	int32 SelectedMailIndex = INDEX_NONE;
};
