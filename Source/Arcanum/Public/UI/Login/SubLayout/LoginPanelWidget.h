#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "LoginPanelWidget.generated.h"

class UImage;
class UOverlay;

class UCommonBtnWidget;
class UCommonDialog;

class ULoginUserWidget;
class USyncLoginUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginStateChanged, bool, bIsLoading);

UCLASS()
class ARCANUM_API ULoginPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnLoginStateChanged OnLoginStateChanged;
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region 공통
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UOverlay> LoginOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonDialog> ErrCommonDialog;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USyncLoginUserWidget> SyncLoginWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BackgroundImg;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> ExitBtn;
private:
	UFUNCTION()
	void ClickExitBtn();
	// ID/PW 로그인이 성공했을 때 불릴 콜백
	UFUNCTION()
	void HandleIDLoginState(bool IsState);
#pragma endregion
	
#pragma region 게스트 로그인
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<ULoginUserWidget> LoginUserWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> GuestBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SyncText;
private:
	UFUNCTION() 
	void OnGuestLoginClicked();

	UFUNCTION() void HandleLoginOK(const FString& ID, const FString& PW);
	UFUNCTION() void HandleLoginCancel();
	UFUNCTION() void OnErrCommonDialog(EDialogResult res);

	void StartPostLogin(const FString& ID, const FString& PW);

	void OnPostLoginFinished(bool bSuccess, const FString& ErrorMessage);
	
#pragma endregion

#pragma region Google 로그인(W.I.P)
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> GoogleBtn;
private:
	UFUNCTION() 
	void OnGoogleLoginClicked();
#pragma endregion


};
