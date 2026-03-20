// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/SubLayout/BattleBattleEndWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"

#include "Core/ARPlayerAccountService.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"

void UBattleBattleEndWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UARGameInstance* GameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		UGameDataSubsystem* GameDataSubSystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
		if (GameDataSubSystem)
		{
			ThisStageTag = GameInstance->CurrentStageTag;
		}
	}

	if (FadeIn)
	{
		PlayAnimation(FadeIn, 0.f, 1);
	}
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
	if (StarNum == -1)
	{
		for (int i = 0; i < StarBackgrounds.Num(); i++)
		{
			StarBackgrounds[i]->SetVisibility(ESlateVisibility::Hidden);
		}
		return;
	}

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
			StarBackgrounds[i]->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBattleBattleEndWidget::SetVictoryText(bool IsVictory)
{
	if (IsVictory)
	{
		BattleEndResultText->SetText(FText::FromString(TEXT("승리")));
		GetClearReward();
		if (StarAnim)
		{
			PlayAnimation(StarAnim, 0.f, 1);
		}
	}
	else
	{
		Defeat->SetVisibility(ESlateVisibility::Visible);
		BattleEndResultText->SetText(FText::FromString(TEXT("패배")));
		BattleEndResultText->SetColorAndOpacity(FSlateColor(FLinearColor(0.17f, 0.0f, 0.03f, 1.0f)));
		RewardHorizontalBox->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UBattleBattleEndWidget::SetClearTimeText(int32 InSec)
{
	int32 Min = InSec / 60;
	int32 Sec = InSec % 60;
	FString Result = FString::Printf(TEXT("%02d:%02d초"), Min, Sec);
	ClearTimeText->SetText(FText::FromString(Result));
}

void UBattleBattleEndWidget::GetClearReward()
{
	int64 GoldReward = 0;
	int64 GemReward =0;
	int64 SoulReward = 0;

	if (FPlayerAccountService::GetStageData(this, StageDatas)) {

		for (FDTStageDataRow* RowPtr : StageDatas) {
			if (!RowPtr) continue;
			
			if(ThisStageTag ==  RowPtr->StageData.StageTag)
			{
				GoldReward = RowPtr->StageData.Reward.Gold;
				GemReward = RowPtr->StageData.Reward.Gem;
				SoulReward = RowPtr->StageData.Reward.Soul;

				break;
			}
		}
	}
	SetClearReward(GoldReward,GemReward,SoulReward);
}

void UBattleBattleEndWidget::SetClearReward(int64 InGold, int64 InGem, int64 InSoul)
{
	ClearGold->SetText(FText::AsNumber(InGold));
	ClearGem->SetText(FText::AsNumber(InGem));
	ClearSoul->SetText(FText::AsNumber(InSoul));
}

void UBattleBattleEndWidget::OnLobbyButtonClick()
{
	OnInBattleLobbyButtonClick.Broadcast();
}
