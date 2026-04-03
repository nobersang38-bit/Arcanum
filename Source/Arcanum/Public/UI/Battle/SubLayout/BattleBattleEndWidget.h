// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/StageData/StageInfo/DataTable/FDTStageDataRow.h"
#include "BattleBattleEndWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInBattleLobbyButtonClick);

/**
 * 김도현
 */
class UTextBlock;
class UBorder;
class UImage;
class UButton;
class UWidgetAnimation;
class UHorizontalBox;


UCLASS()
class ARCANUM_API UBattleBattleEndWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	FOnInBattleLobbyButtonClick OnInBattleLobbyButtonClick;

	// -1이면 백그라운드도 숨기기
	UFUNCTION()
	void SetStar(int32 StarNum);

	UFUNCTION()
	void SetVictoryText(bool IsVictory);

	UFUNCTION()
	void SetClearTimeText(int32 InSec);
	
	UFUNCTION()
	void GetClearReward();

	UFUNCTION()
	void SetClearReward(int64 InGold, int64 InGem, int64 InSoul);

protected:
	UFUNCTION()
	void OnLobbyButtonClick();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> Star1Background = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> Star2Background = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> Star3Background = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Star1 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Star2 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Star3 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> Defeat = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> BlackBackground = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> BattleEndResultText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ClearTimeText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ClearGold = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ClearGem = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ClearSoul = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> LobbyButton = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> RetryButton = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> RewardHorizontalBox = nullptr;

	UPROPERTY(meta = (BindWidgetAnim), Transient) 
	TObjectPtr<UWidgetAnimation> StarAnim = nullptr;
	UPROPERTY(meta = (BindWidgetAnim), Transient) 
	TObjectPtr<UWidgetAnimation> FadeIn = nullptr;

	UPROPERTY()
	TArray<UBorder*> StarBackgrounds;

	UPROPERTY()
	TArray<UImage*> Stars;

	FGameplayTag ThisStageTag;

private:
	TArray<FDTStageDataRow*> StageDatas;
};
