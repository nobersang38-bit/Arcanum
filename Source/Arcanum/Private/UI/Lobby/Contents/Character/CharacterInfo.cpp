// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Lobby/Contents/Character/CharacterInfo.h"
#include "Components/TextBlock.h"
#include "UI/Common/CommonBtnWidget.h"

void UCharacterInfo::NativePreConstruct()
{
	Super::NativePreConstruct();

    FText DefaultName = FText::FromString(TEXT("캐릭터 이름"));
    FText DefaultInfo = FText::FromString(TEXT("등급\n성급\n체력\n마나"));

    if (CharacterNameText)
    {
        CharacterNameText->SetText(CharactNameTxt.IsEmpty() ? DefaultName : CharactNameTxt);
    }

    if (CharacterInfoText)
    {
        CharacterInfoText->SetText(CharacInfoTxt.IsEmpty() ? DefaultInfo : CharacInfoTxt);
    }
}

void UCharacterInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (CharacterEnhanceBtn) {
		CharacterEnhanceBtn->OnClicked.RemoveDynamic(this, &UCharacterInfo::ClickCharacterEnhanceBtn);
		CharacterEnhanceBtn->OnClicked.AddDynamic(this, &UCharacterInfo::ClickCharacterEnhanceBtn);
	}
}

void UCharacterInfo::ClickCharacterEnhanceBtn()
{
	OnEnhanceBtnClicked.Broadcast();
}
