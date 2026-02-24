// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUDWidget.generated.h"

class URoundedSlotWidget;
class UUniformGridPanel;

/**
 * 
 */
UCLASS()
class ARCANUM_API UCharacterHUDWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region 바인딩
	// 캐릭터창, 유닛창, 장비창, 캐릭터 설명창, 캐릭터 강화창, 장비 인벤토리
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel>CharacterGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel>UnitGridPanel;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Slot")
	TSubclassOf<URoundedSlotWidget> RoundedSlotWidgetClass;
};
