#include "UI/Battle/SubLayout/BattleMenuWidget.h"
#include "UI/Common/CommonDialog.h"
#include "UI/Common/CommonOptionWindow.h"
#include "UI/Common/CommonBtnWidget.h"
#include "Core/ARPlayerAccountService.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UBattleMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OptionBtn)
	{
		OptionBtn->OnClicked.RemoveDynamic(this, &UBattleMenuWidget::ClickOptionBtn);
		OptionBtn->OnClicked.AddDynamic(this, &UBattleMenuWidget::ClickOptionBtn);
	}

	if (ReturnLobbyBtn)
	{
		ReturnLobbyBtn->OnClicked.RemoveDynamic(this, &UBattleMenuWidget::ClickReturnLobbyBtn);
		ReturnLobbyBtn->OnClicked.AddDynamic(this, &UBattleMenuWidget::ClickReturnLobbyBtn);
	}

	if (QuitGameBtn)
	{
		QuitGameBtn->OnClicked.RemoveDynamic(this, &UBattleMenuWidget::ClickQuitGameBtn);
		QuitGameBtn->OnClicked.AddDynamic(this, &UBattleMenuWidget::ClickQuitGameBtn);
	}

	if (CommonDialog)
	{
		CommonDialog->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UBattleMenuWidget::ClickReturnLobbyBtn()
{
	if (!CommonDialogClass) return;

	CommonDialog = CreateWidget<UCommonDialog>(this, CommonDialogClass);
	if (CommonDialog)
	{
		CommonDialog->DialogType = EDialogType::OKCancel;
		CommonDialog->TitleTxt = FText::FromString(TEXT("알림"));
		CommonDialog->MessageTxt = FText::FromString(TEXT("로비로 나가시겠습니까?"));
		CommonDialog->OnResult.RemoveDynamic(this, &UBattleMenuWidget::OnReturnLobbyDialogResult);
		CommonDialog->OnResult.AddDynamic(this, &UBattleMenuWidget::OnReturnLobbyDialogResult);
		CommonDialog->AddToViewport();
	}
}

void UBattleMenuWidget::ClickQuitGameBtn()
{
	if (!CommonDialogClass) return;

	CommonDialog = CreateWidget<UCommonDialog>(this, CommonDialogClass);
	if (CommonDialog)
	{
		CommonDialog->DialogType = EDialogType::OKCancel;
		CommonDialog->TitleTxt = FText::FromString(TEXT("알림"));
		CommonDialog->MessageTxt = FText::FromString(TEXT("게임을 종료하시겠습니까?"));
		CommonDialog->OnResult.RemoveDynamic(this, &UBattleMenuWidget::OnQuitGameDialogResult);
		CommonDialog->OnResult.AddDynamic(this, &UBattleMenuWidget::OnQuitGameDialogResult);
		CommonDialog->AddToViewport();
	}
}

void UBattleMenuWidget::ClickOptionBtn()
{
	if (!OptionWindowClass) return;

	if (OptionWindow)
	{
		OptionWindow->RemoveFromParent();
		OptionWindow = nullptr;
	}

	OptionWindow = CreateWidget<UCommonOptionWindow>(this, OptionWindowClass);
	if (OptionWindow)
	{
		OptionWindow->AddToViewport();
	}
}

void UBattleMenuWidget::OnReturnLobbyDialogResult(EDialogResult Result)
{
	if (CommonDialog)
	{
		CommonDialog->RemoveFromParent();
		CommonDialog = nullptr;
	}

	if (Result != EDialogResult::OK) return;

	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		FPlayerAccountService::FinalizeBattlePotionSlots(this, battleSubsystem->GetInBattleData().PlayerData.BattlePotionSlots);
	}

	UGameplayStatics::OpenLevel(this, TEXT("LobbyMap"));
}

void UBattleMenuWidget::OnQuitGameDialogResult(EDialogResult Result)
{
	if (CommonDialog)
	{
		CommonDialog->RemoveFromParent();
		CommonDialog = nullptr;
	}

	if (Result != EDialogResult::OK) return;

	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		FPlayerAccountService::FinalizeBattlePotionSlots(this, battleSubsystem->GetInBattleData().PlayerData.BattlePotionSlots);
	}

	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}