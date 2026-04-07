// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleCostBarWidget.generated.h"

/**
 * 김도현
 */
class UImage;
class UTextBlock;
class UProgressBar;
UCLASS()
class ARCANUM_API UBattleCostBarWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;
#pragma endregion

	
public:
	UFUNCTION()
	void SetCostProgress(float CurrentHealth, float MaxHealth);


#pragma region 위젯 바인딩
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Icon1 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Icon2 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> CostProgress = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> CostText = nullptr;
#pragma endregion

#pragma region 에디터변경용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<UTexture2D> EditIcon1Texture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FLinearColor EditIcon1Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<UTexture2D> EditIcon2Texture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FLinearColor EditIcon2Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FLinearColor EditProgressColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
#pragma endregion
};
