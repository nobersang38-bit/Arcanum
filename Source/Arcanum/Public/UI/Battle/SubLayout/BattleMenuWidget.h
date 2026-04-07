#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "BattleMenuWidget.generated.h"

class UCommonBtnWidget;
class UCommonDialog;
class UCommonOptionWindow;

/**
 * 인게임 메뉴
 */
UCLASS()
class ARCANUM_API UBattleMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	/* 로비로 나가기 버튼 */
	UFUNCTION()
	void ClickReturnLobbyBtn();

	/* 게임 종료 버튼 */
	UFUNCTION()
	void ClickQuitGameBtn();

	/* 옵션 버튼 */
	UFUNCTION()
	void ClickOptionBtn();

	/* 로비 이동 확인 */
	UFUNCTION()
	void OnReturnLobbyDialogResult(EDialogResult Result);

	/* 게임 종료 확인  */
	UFUNCTION()
	void OnQuitGameDialogResult(EDialogResult Result);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> ReturnLobbyBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> QuitGameBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> OptionBtn;
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCommonDialog> CommonDialogClass;

	UPROPERTY()
	TObjectPtr<UCommonDialog> CommonDialog;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCommonOptionWindow> OptionWindowClass;

	UPROPERTY()
	TObjectPtr<UCommonOptionWindow> OptionWindow;
};
