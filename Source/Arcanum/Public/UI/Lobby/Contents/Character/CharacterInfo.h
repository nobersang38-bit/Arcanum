// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInfo.generated.h"

class UCommonBtnWidget;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnhanceBtnClicked);

/*
 * 역할 : 김유진
 * 1. 캐릭터 설명창 (이름, 설명, 강화버튼)
 */

UCLASS()
class ARCANUM_API UCharacterInfo : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 언리얼 기본 생성
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
#pragma endregion
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharactNameTxt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CharacInfoTxt;

	UFUNCTION(BlueprintCallable)
	void SetCharacterName(FName CharacterName);

	UFUNCTION(BlueprintCallable)
	void SetStarCharcterInfo(int32 StarGrade);

	UFUNCTION(BlueprintCallable)
	void SetGradeCharcterInfo(int32 Grade);
	
	//UFUNCTION(BlueprintCallable)
	//void SetCharcterInfo();

	UFUNCTION(BlueprintCallable)
	void SetEnhanceButtonEnabled(bool bIsCharacterOwned);


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GradeStarsText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GradeText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterInfoText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> CharacterEnhanceBtn;

public:
	UPROPERTY(BlueprintAssignable)
	FOnEnhanceBtnClicked OnEnhanceBtnClicked;

private:
	UFUNCTION()
	void ClickCharacterEnhanceBtn();

};
