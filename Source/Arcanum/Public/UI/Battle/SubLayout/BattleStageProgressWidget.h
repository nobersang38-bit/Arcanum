// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleStageProgressWidget.generated.h"

/**
 * 김도현
 */
class USlider;
class UTextBlock;
UCLASS()
class ARCANUM_API UBattleStageProgressWidget : public UUserWidget
{
	GENERATED_BODY()
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion


public:
	UFUNCTION()
	void SetPlayerLocationProgress(const FVector& InAllyNexusLocation, const FVector& InEnemyNexusLocation, const FVector& InPlayerLocation);

	UFUNCTION()
	void SetTime(int32 TimeMS);

	UFUNCTION()
	void SetTimeSecond(int32 TimeSecond);


#pragma region 위젯 바인딩
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> StageProgressSlider = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText = nullptr;
#pragma endregion
};
