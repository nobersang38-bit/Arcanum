// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicAttackWidget.generated.h"

/**
 * 추영호
 * - 기본공격 버튼 전용 위젯
 */

class UButton;

/* 기본공격 클릭 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBasicAttackClickedRequested);

UCLASS()
class ARCANUM_API UBasicAttackWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	/* [버튼 바인딩 */
	void BindButtonEvents();

private:
	/* 버튼 클릭 */
	UFUNCTION()
	void HandleBasicAttackClicked();

public:
	UPROPERTY(BlueprintAssignable, Category = "BasicAttack")
	FOnBasicAttackClickedRequested OnBasicAttackClickedRequested;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ButtonBasicAttack = nullptr;
};
