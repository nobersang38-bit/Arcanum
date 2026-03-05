// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleBattleEndWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInBattleLobbyButtonClick);

/**
 * 김도현
 */
class UTextBlock;
class UBorder;
class UImage;
class UButton;
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
	TObjectPtr<UTextBlock> BattleEndResultText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> ClearTimeText = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> LobbyButton = nullptr;

	UPROPERTY()
	TArray<UBorder*> StarBackgrounds;

	UPROPERTY()
	TArray<UImage*> Stars;
};
