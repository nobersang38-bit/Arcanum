// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Lobby/Contents/Character/CharacterInfo.h"
#include "Components/TextBlock.h"
#include "UI/Common/CommonBtnWidget.h"

void UCharacterInfo::NativePreConstruct()
{
	Super::NativePreConstruct();

    //FText DefaultName = FText::FromString(TEXT("캐릭터 이름"));
    //FText DefaultInfo = FText::FromString(TEXT("등급\n성급\n체력\n마나"));

    if (CharacterNameText)
    {
        CharacterNameText->SetText(CharactNameTxt.IsEmpty() ? FText::GetEmpty() : CharactNameTxt);
    }

    if (CharacterInfoText)
    {
        CharacterInfoText->SetText(CharacInfoTxt.IsEmpty() ? FText::GetEmpty() : CharacInfoTxt);
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

void UCharacterInfo::SetCharacterName(FName CharacterName)
{
    if (CharacterNameText)
    {
        CharacterNameText->SetText(FText::FromName(CharacterName));
    }
}

void UCharacterInfo::SetStarCharcterInfo(int32 StarGrade)
{
    if (GradeStarsText)
    {
        FString Stars;

        // 1부터 시작함 (1일때 보유만, 2 - 1성)
        for (int32 i = 1; i < StarGrade; i++)
        {
            Stars += TEXT("★");
        }

        GradeStarsText->SetText(FText::FromString(Stars));
    }
}

void UCharacterInfo::SetGradeCharcterInfo(int32 Grade)
{
    FText TargetText;
    FLinearColor TargetColor = FLinearColor::Black;

    switch (Grade)
    {
    case 0:  
        TargetText = FText::FromString(TEXT("Common")); 
        TargetColor = FLinearColor::White; 
        break;
    case 1:  
        TargetText = FText::FromString(TEXT("Rare"));   
        TargetColor = FLinearColor(0.7f, 0.3f, 1.0f, 1.0f); 
        break;
    case 2:  
        TargetText = FText::FromString(TEXT("Epic"));   
        TargetColor = FLinearColor::Yellow; 
        break;
    default: return; 
    }

    if (GradeText)
    {
        GradeText->SetText(TargetText);

        FSlateFontInfo NewFontInfo = GradeText->GetFont();
        NewFontInfo.OutlineSettings.OutlineColor = TargetColor;
        GradeText->SetFont(NewFontInfo);
    }
}

void UCharacterInfo::SetCharcterInfo(const FText& InText)
{
    if (CharacterInfoText)
    {
        CharacterInfoText->SetText(InText);
    }
}

void UCharacterInfo::SetEnhanceButtonEnabled(bool bIsCharacterOwned,int32 RequiredSoul)
{
    if (CharacterEnhanceBtn)
    {
        CharacterEnhanceBtn->SetIsEnabled(bIsCharacterOwned);
    }
}

void UCharacterInfo::SetEnhanceBtnText(const FText& InText)
{
    if (CharacterEnhanceBtn)
    {
        CharacterEnhanceBtn->SetButtonText(InText);
    }
}

void UCharacterInfo::ClickCharacterEnhanceBtn()
{
	OnEnhanceBtnClicked.Broadcast();
}
