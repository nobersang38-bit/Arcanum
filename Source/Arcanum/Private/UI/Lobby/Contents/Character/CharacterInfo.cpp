// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Lobby/Contents/Character/CharacterInfo.h"
#include "Components/TextBlock.h"
#include "UI/Common/CommonBtnWidget.h"

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
