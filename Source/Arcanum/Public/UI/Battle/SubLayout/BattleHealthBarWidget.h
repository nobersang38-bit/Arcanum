// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleHealthBarWidget.generated.h"

/**
 * 김도현
 */
class UImage;
class UTextBlock;
class UProgressBar;
UCLASS()
class ARCANUM_API UBattleHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion
public:
	UFUNCTION()
	void SetHealthProgress(float CurrentHealth, float MaxHealth);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgress = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthText = nullptr;
};
