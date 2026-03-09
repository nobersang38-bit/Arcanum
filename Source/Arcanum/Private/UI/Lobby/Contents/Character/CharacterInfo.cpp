// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/Lobby/Contents/Character/CharacterInfo.h"
#include "Components/TextBlock.h"
#include "UI/Common/CommonBtnWidget.h"

//void UCharacterInfo::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//
//    //FText DefaultName = FText::FromString(TEXT("캐릭터 이름"));
//    //FText DefaultInfo = FText::FromString(TEXT("등급\n성급\n체력\n마나"));
//
//    if (CharacterNameText)
//    {
//        CharacterNameText->SetText(CharactNameTxt.IsEmpty() ? FText::GetEmpty() : CharactNameTxt);
//    }
//
//    if (CharacterInfoText)
//    {
//        CharacterInfoText->SetText(CharacInfoTxt.IsEmpty() ? FText::GetEmpty() : CharacInfoTxt);
//    }
//}

void UCharacterInfo::NativeConstruct()
{
	Super::NativeConstruct();

	if (CharacterEnhanceBtn) {
		CharacterEnhanceBtn->OnClicked.RemoveDynamic(this, &UCharacterInfo::ClickCharacterEnhanceBtn);
		CharacterEnhanceBtn->OnClicked.AddDynamic(this, &UCharacterInfo::ClickCharacterEnhanceBtn);
	}
    
	if (SetPlayerBtn) {
        SetPlayerBtn->OnClicked.RemoveDynamic(this, &UCharacterInfo::ClickSetPlayerBtn);
        SetPlayerBtn->OnClicked.AddDynamic(this, &UCharacterInfo::ClickSetPlayerBtn);
	}
}

// ========================================================
// 캐릭터 정보창 - 이름 입력
// ========================================================
void UCharacterInfo::SetCharacterName(FName CharacterName)
{
    if (CharacterNameText)
    {
        CharacterNameText->SetText(FText::FromName(CharacterName));
        CharactNameTxt = (FText::FromName(CharacterName));
    }
}

// ========================================================
// 캐릭터 정보창 - 별 등급 입력
// ========================================================
void UCharacterInfo::SetStarCharcterInfo(int32 StarGrade)
{
    if (GradeStarsText)
    {
        FString Stars;

       
        for (int32 i = 0; i < StarGrade; i++)
        {
            Stars += TEXT("★");
        }

        GradeStarsText->SetText(FText::FromString(Stars));
    }
}

// ========================================================
// 캐릭터 정보창 - 등급 입력 (일반, 레어, 영웅)
// ========================================================
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

// ========================================================
// 캐릭터 정보창 - 체력, 마나 등등 정보 입력
// ========================================================
void UCharacterInfo::SetCharcterInfo(const FText& InText)
{
    if (CharacterInfoText)
    {
        CharacterInfoText->SetText(InText);
    }
}

// ========================================================
// 캐릭터에 따른 강화 버튼 활성화/비활성화
// ========================================================
void UCharacterInfo::SetEnhanceButtonEnabled(bool bIsCharacterOwned,int32 InRequiredSoul, int64 CurrentSoul, int EnhancementLevel)
{
    RequiredSoul = InRequiredSoul;
    // 일단은 최대 강화 (성급) 3으로 제한
    if (CharacterEnhanceBtn)
    {
        bool bShouldEnable = bIsCharacterOwned && (EnhancementLevel < 3) && (CurrentSoul >= RequiredSoul);
        CharacterEnhanceBtn->SetIsEnabled(bShouldEnable);
    }
}

// ========================================================
// 캐릭터 보유에 따른 장착 버튼 활성화/비활성화
// ========================================================
void UCharacterInfo::SetPlayerButtonEnabled(bool bSetCharacter,bool SlotCharacterOwned)
{
    if (SetPlayerBtn)
    {
        // 보유중이고 장착되지 않은 경우에만 true, 나머지는 false
        SetPlayerBtn->SetIsEnabled(!bSetCharacter && SlotCharacterOwned);
    }
}

// ========================================================
// 강화 버튼 텍스트 설정
// ========================================================
void UCharacterInfo::SetEnhanceBtnText(const FText& InText)
{
    if (CharacterEnhanceBtn)
    {
        CharacterEnhanceBtn->SetButtonText(InText);
    }
}

// ========================================================
// 강화 버튼 클릭 처리 함수
// ========================================================
void UCharacterInfo::ClickCharacterEnhanceBtn()
{
	OnEnhanceBtnClicked.Broadcast(CharactNameTxt, RequiredSoul);
}

// ========================================================
// 장착 버튼 클릭 처리 함수
// ========================================================
void UCharacterInfo::ClickSetPlayerBtn()
{
    OnSetPlayerBtnClicked.Broadcast(CharactNameTxt);
}
