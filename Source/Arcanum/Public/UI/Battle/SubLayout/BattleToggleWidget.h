// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleToggleWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToggle, bool, bIsChecked);

/**
 * 김도현
 */
class UCheckBox;
class UWidgetSwitcher;
class UImage;
class UWidgetAnimation;

UCLASS()
class ARCANUM_API UBattleToggleWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion


public:
	FOnToggle OnToggle;

	UFUNCTION()
	bool IsAutoPlay();


#pragma region 내부 함수
protected:
	UFUNCTION()
	void OnChangeToggle(bool IsChecked);
#pragma endregion


#pragma region 위젯 바인딩
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> Toggle = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> TextSwitcher = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> OffIcon = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> OnIcon = nullptr;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Glow = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> GlowAnimation = nullptr;


#pragma endregion
};
