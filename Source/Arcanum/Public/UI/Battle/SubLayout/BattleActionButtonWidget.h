// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleActionButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClick);

/* 궁극기 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonReleased);

/**
 * 김도현
 */
class UTextBlock;
class UButton;
class UProgressBar;
class UImage;
class UMaterialInstanceDynamic;

UCLASS()
class ARCANUM_API UBattleActionButtonWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#if WITH_EDITOR
	virtual void SynchronizeProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
#pragma endregion


public:
	FOnButtonClick OnButtonClick;
	FOnButtonPressed OnButtonPressed;
	FOnButtonReleased OnButtonReleased;

	UFUNCTION()
	void SetActivateCost(bool InIsDisable);

	UFUNCTION()
	void SetCoolTimeProgress(float CurrentProgress, float MaxProgress);

	UFUNCTION()
	void SetImage(UTexture2D* InImage);

	UFUNCTION()
	void SetCostText(FText InText);

#pragma region 내부 함수
protected:
	UFUNCTION()
	void OnActionButtonClick();
	UFUNCTION()
	void OnActionButtonPressed();
	UFUNCTION()
	void OnActionButtonReleased();

	UFUNCTION()
	void SetProgressesVisible(bool IsVisible);
#pragma endregion


#pragma region 위젯 바인딩
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ActionButton = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> DisabledImage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> CoolTimeProgress = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> StackCountText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CostText = nullptr;
#pragma endregion


#pragma region 에디터 변경 및 런타임 참조용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "에디터 변경용")
	FText IconText = FText::FromString(TEXT("버튼"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "에디터 변경용")
	FText EditCostText = FText::FromString(TEXT("100"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "에디터 변경용")
	bool bUseDisableImage = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "에디터 변경용")
	bool bUseCoolTimeProgressBar = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "에디터 변경용")
	bool bUseDefaultIcon = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "에디터 변경용")
	TObjectPtr<UTexture2D> EditLockIcon = nullptr;
#pragma endregion


#pragma region 디버그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|CoolTime")
	bool bIsDebugCoolTimeStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|CoolTime")
	float DebugCoolTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|CoolTime")
	float DebugMaxCoolTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|Cost")
	bool bIsDebugCostDisableImage = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug|Cost")
	bool bIsSetDebugCostDisableImage = false;
#pragma endregion


#pragma region 물약, 스킬
public:
	/* 스킬 쿨타임 머티리얼 진행도 갱신 */
	void SetSkillCooldownPercent(float InPercent);

	void SetStackCount(int32 InCount);

	void ClearStackCount();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> SkillCooldownImage = nullptr;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> SkillCooldownMID = nullptr;
#pragma endregion

};

