#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginUserWidget.generated.h"

class UEditableTextBox;
class UButton;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOKClicked, const FString&, ID, const FString&, Password);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCancelClicked);

/**
 * Version : 1.0.0.0 2026/02/04
 * 역할 :
 * 1. 로그인 화면 UI
 * 2. ID/Password 입력, 로그인 버튼
 * 3. 필요하면 배경 이미지, 로고 등 WBP에서 세팅
 */

UCLASS()
class ARCANUM_API ULoginUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 델리게이트
public:
	UPROPERTY(BlueprintAssignable, Category = "Login")
	FOnOKClicked OnOKClicked;
	UPROPERTY(BlueprintAssignable, Category = "Login")
	FOnCancelClicked OnCancelClicked;
#pragma endregion

	
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

protected:
	/** ID 입력 텍스트박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> IDTextBox;

	/** Password 입력 텍스트박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> PasswordTextBox;

	/** 로그인 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LoginButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CancelButton;

	/** 배경 이미지, 로고 등 (선택) */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> BackgroundImage;

public:
	/** 로그인 버튼 클릭 이벤트 */
	UFUNCTION()
	void OnLoginClicked();
	
	UFUNCTION()
	void OnCancelButtonClicked();

	/** ID와 Password 가져오기 */
	UFUNCTION(BlueprintCallable, Category = "Login")
	void GetLoginInfo(FString& OutID, FString& OutPassword) const;
};
