// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "LobbyHUD.generated.h"

/*
 * 역할 : 김유진
 * 1. 메뉴 버튼 (전투, 캐릭터, 강화, 상점, 가챠)
 * 2. 설정, 종료 버튼 관리
 */

/**
 * 
 */
class UCommonBtnWidget;
class UCommonDialog;
class UHorizontalBox;
class UBackgroundBlur;

UCLASS()
class ARCANUM_API ULobbyHUD : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region 바인딩 메뉴 버튼
	// 상단 메뉴 : 전투, 캐릭터, 강화, 상점, 가챠
	// 설정, 종료

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> BattleMenuBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> CharacterMenuBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> EnhancementMenuBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> ShopMenuBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> GachaMenuBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SettingBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> QuitBtn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> MenuHorizontalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SettingUHorizontalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBackgroundBlur> BackgroundBlur;
	
private:
	UFUNCTION()
	void ClickBattleMenuBtn();

	UFUNCTION()
	void ClickCharacterMenuBtn();

	UFUNCTION()
	void ClickEnhancementMenuBtn();

	UFUNCTION()
	void ClickShopMenuBtn();

	UFUNCTION()
	void ClickGachaMenuBtn();

	UFUNCTION()
	void ClickSettingBtn();

	UFUNCTION()
	void ClickQuitBtn();

#pragma endregion

#pragma region 재화

#pragma endregion

#pragma region 전투

#pragma endregion

#pragma region 캐릭터

#pragma endregion

#pragma region 강화

#pragma endregion

#pragma region 상점

#pragma endregion

#pragma region 가챠

#pragma endregion

#pragma region 설정

#pragma endregion

#pragma region 종료
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonDialog> ExitCommonDialog;
private:
	UFUNCTION()
	void OnExitCommonDialog(EDialogResult res);
#pragma endregion

};
