// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleBattleEndWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UBattleBattleEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LobbyButton)
	{
		LobbyButton->OnClicked.RemoveDynamic(this, &UBattleBattleEndWidget::OnLobbyButtonClick);
		LobbyButton->OnClicked.AddDynamic(this, &UBattleBattleEndWidget::OnLobbyButtonClick);
	}

	StarBackgrounds = { Star1Background, Star2Background, Star3Background };
	Stars = { Star1, Star2, Star3 };
}

void UBattleBattleEndWidget::SetStar(int32 StarNum)
{
	for (int i = 0; i < StarBackgrounds.Num(); i++)
	{
		StarBackgrounds[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	for (int i = 0; i < Stars.Num(); i++)
	{
		if (i < StarNum)
		{
			Stars[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			Stars[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBattleBattleEndWidget::SetVictoryText(bool IsVictory)
{
	if (IsVictory)
	{
		ClearTimeText->SetText(FText::FromString(TEXT("승리")));
	}
	else
	{
		ClearTimeText->SetText(FText::FromString(TEXT("패배")));
	}
}

void UBattleBattleEndWidget::SetClearTimeText(int32 InSec)
{
	int32 Min = InSec / 60;
	int32 Sec = InSec % 60;
	FString Result = FString::Printf(TEXT("%d:%d초"), Min, Sec);
	ClearTimeText->SetText(FText::FromString(Result));
}

void UBattleBattleEndWidget::OnLobbyButtonClick()
{
	OnInBattleLobbyButtonClick.Broadcast();
}
