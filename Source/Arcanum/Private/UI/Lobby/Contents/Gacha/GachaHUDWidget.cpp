#include "UI/Lobby/Contents/Gacha/GachaHUDWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaPullButtonWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaProbabilityWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaBannerButtonWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaProgressWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SubSystem/GameTimeSubsystem.h"

// ========================================================
// 언리얼 기본생성 / 초기화
// ========================================================
void UGachaHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitBannerList();

    if (UGameTimeSubsystem* TimeSubsystem = GetGameInstance()->GetSubsystem<UGameTimeSubsystem>()) {
        TimeSubsystem->OnTimeUpdated.RemoveDynamic(this, &UGachaHUDWidget::HandleTimeUpdated);
        TimeSubsystem->OnTimeUpdated.AddDynamic(this, &UGachaHUDWidget::HandleTimeUpdated);
    }
}
void UGachaHUDWidget::InitBannerList()
{
    if (!BannerVerticalBox || !BannerButtonClass) return;
    BannerVerticalBox->ClearChildren();

    TArray<const FDTGachaBannerDataRow*> TempRows;
    FPlayerAccountService::GetActiveGachaBannerRows(this, TempRows);
    ActiveBannerDataList.Empty();
    for (const FDTGachaBannerDataRow* RowPtr : TempRows) {
        if (RowPtr) {
            ActiveBannerDataList.Add(*RowPtr);
            FPlayerAccountService::InitGachaBannerData(this, RowPtr->BannerTag);
        }
    }

    for (int32 i = 0; i < ActiveBannerDataList.Num(); ++i) {
        UGachaBannerButtonWidget* NewButton = CreateWidget<UGachaBannerButtonWidget>(this, BannerButtonClass);
        if (NewButton) {
            NewButton->UpdateBannerData(&ActiveBannerDataList[i]);
            NewButton->BannerClicked.RemoveAll(this);
            NewButton->BannerClicked.AddUObject(this, &UGachaHUDWidget::OnBannerSelected);
            BannerButtons.Add(NewButton);

            UVerticalBoxSlot* ButtonSlot = BannerVerticalBox->AddChildToVerticalBox(NewButton);
            if (ButtonSlot) {
                ButtonSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));
                ButtonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            }

            //if (i == 0) OnBannerSelected(NewButton->BannerTag);
        }
    }

    if (SinglePullButton) {
        SinglePullButton->OnPullClicked.RemoveAll(this);
        SinglePullButton->OnPullClicked.AddUObject(this, &UGachaHUDWidget::RequestGacha);
    }
    if (MultiPullButton) {
        MultiPullButton->OnPullClicked.RemoveAll(this);
        MultiPullButton->OnPullClicked.AddUObject(this, &UGachaHUDWidget::RequestGacha);
    }

    if (EpicButton) {
        EpicButton->OnPullClicked.RemoveAll(this);
        EpicButton->OnPullClicked.AddUObject(this, &UGachaHUDWidget::RequestGachaTest);
    }
    if (ProbabilityInfoButton) {
        ProbabilityInfoButton->OnClicked.RemoveDynamic(this, &UGachaHUDWidget::HandleProbabilityButtonClicked);
        ProbabilityInfoButton->OnClicked.AddDynamic(this, &UGachaHUDWidget::HandleProbabilityButtonClicked);
    }
}
// ========================================================
// 배너 버튼 관련
// ========================================================
void UGachaHUDWidget::OnBannerSelected(FGameplayTag SelectedBannerTag)
{
    if (CurrentSelectedButton) CurrentSelectedButton->SetSelected(false);

    const FDTGachaBannerDataRow* SelectedData = nullptr;
    for (const FDTGachaBannerDataRow& Data : ActiveBannerDataList) {
        if (Data.BannerTag == SelectedBannerTag) {
            SelectedData = &Data;
            break;
        }
    }
    if (!SelectedData) return;

    TArray<UWidget*> AllButtons = BannerVerticalBox->GetAllChildren();
    for (UWidget* Widget : AllButtons) {
        if (UGachaBannerButtonWidget* Btn = Cast<UGachaBannerButtonWidget>(Widget)) {
            if (Btn->BannerTag == SelectedBannerTag) {
                Btn->SetSelected(true);
                CurrentSelectedButton = Btn;
                break;
            }
        }
    }
    UpdateDetailedView(SelectedData);

    const FGachaBannerState* StatePtr = ParentLobby->CachedPlayerData.GachaState.BannerStates.Find(SelectedBannerTag);

    int32 CurrentPity = 0;
    if (StatePtr) CurrentPity = StatePtr->PityCount;
    else {
        FGachaBannerState NewState = FPlayerAccountService::InitGachaBannerData(this, SelectedBannerTag);
        CurrentPity = NewState.PityCount;
    }
    UpdateGachaProgressWidget(CurrentPity, SelectedData->FiveStarPityCount);
}
void UGachaHUDWidget::UpdateDetailedView(const FDTGachaBannerDataRow* InData)
{
    if (!InData) return;
    if (DetailedBannerImage) {
        TSoftObjectPtr<UTexture2D> SoftImg = InData->DescriptionImage;
        if (SoftImg.IsValid()) DetailedBannerImage->SetBrushFromTexture(SoftImg.Get());
        else DetailedBannerImage->SetBrushFromSoftTexture(SoftImg);
    }
    CurrencyCost = InData->PullCost;
}
void UGachaHUDWidget::UpdateDetailedImage(TSoftObjectPtr<UTexture2D> NewTexture)
{
    if (DetailedBannerImage) {
        UTexture2D* LoadedTexture = NewTexture.LoadSynchronous();
        if (LoadedTexture) DetailedBannerImage->SetBrushFromTexture(LoadedTexture);
    }
}
// ========================================================
// 픽업 선택 버튼 관련
// ========================================================
void UGachaHUDWidget::RequestGacha(int32 InPullCount)
{
    if (ParentLobby->CachedPlayerData.Mailbox.Num() >= ParentLobby->CachedPlayerData.MailboxCapacity) {
        UE_LOG(LogTemp, Warning, TEXT("Mailbox Full! Cannot execute gacha."));
        return;
    }


    FPlayerAccountService::UpdateCurrency(this, ParentLobby->CachedPlayerData, Arcanum::PlayerData::Currencies::NonRegen::Soul::Value, 10000);
    ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
    FPlayerAccountService::UpdateCurrency(this, ParentLobby->CachedPlayerData, Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, 10000);
    ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);

    if (!CurrentSelectedButton) {
        UE_LOG(LogTemp, Warning, TEXT("No Banner Selected!"));
        return;
    }

    FGameplayTag SelectedTag = CurrentSelectedButton->BannerTag;
    bool bSuccess = FPlayerAccountService::ExecuteGacha(this, ParentLobby->CachedPlayerData, SelectedTag, CurrencyCost, InPullCount);
     
    /// Test : 가챠 연출 출력 부분
    if (bSuccess) {
        UE_LOG(LogTemp, Log, TEXT("Gacha Request Success: %d Times"), InPullCount);
        FPlayerAccountService::SetHUDIndex(this, EHUDIndex::GachaMenu);
        if (GachaMap.IsNull() == false) UGameplayStatics::OpenLevelBySoftObjectPtr(this, GachaMap);
    }
    else UE_LOG(LogTemp, Error, TEXT("Gacha Request Failed (Insufficient Currency?)"));

}
// ========================================================
// 픽업 선택 버튼 관련 (테스트용 : Epic 확정)
// ========================================================
void UGachaHUDWidget::RequestGachaTest(int32 InPullCount)
{
    if (ParentLobby->CachedPlayerData.Mailbox.Num() >= ParentLobby->CachedPlayerData.MailboxCapacity) {
        return;
    }
    FPlayerAccountService::UpdateCurrency(this, ParentLobby->CachedPlayerData, Arcanum::PlayerData::Currencies::NonRegen::Soul::Value, 10000);
    ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);
    FPlayerAccountService::UpdateCurrency(this, ParentLobby->CachedPlayerData, Arcanum::PlayerData::Currencies::NonRegen::Gold::Value, 10000);
    ParentLobby->CachedPlayerData = FPlayerAccountService::GetPlayerDataCopy(this);

    if (!CurrentSelectedButton) {
        return;
    }

    FGameplayTag SelectedTag = CurrentSelectedButton->BannerTag;
    bool bSuccess = FPlayerAccountService::ExecuteGachaTest(this, ParentLobby->CachedPlayerData, SelectedTag, CurrencyCost, InPullCount);

    if (bSuccess) {
        FPlayerAccountService::SetHUDIndex(this, EHUDIndex::GachaMenu);
        if (GachaMap.IsNull() == false) UGameplayStatics::OpenLevelBySoftObjectPtr(this, GachaMap);
    }

}


// ========================================================
// 확률 버튼
// ========================================================
void UGachaHUDWidget::HandleProbabilityButtonClicked()
{
    if (ProbabilityWidget) {
        ESlateVisibility CurrentVis = ProbabilityWidget->GetVisibility();
        ProbabilityWidget->SetVisibility(CurrentVis == ESlateVisibility::Visible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
        if (ProbabilityWidget->GetVisibility() == ESlateVisibility::Visible) {
            //ProbabilityWidget->UpdateData(CurrentBannerData);
        }
    }
}
void UGachaHUDWidget::HandleTimeUpdated(FDateTime CurrentTime)
{
    bool bNeedRefresh = false;
    for (int32 i = 0; i < ActiveBannerDataList.Num(); ++i) {
        const FDTGachaBannerDataRow& BannerData = ActiveBannerDataList[i];

        if (BannerData.GachaTypeTag == Arcanum::Gacha::Type::Standard::Standard) {
            BannerButtons[i]->UpdateRemainingTimeText(FText::FromString(TEXT("상시")));
            continue;
        }

        FDateTime EndTime;
        if (!FDateTime::Parse(BannerData.EndTime, EndTime)) continue;

        FTimespan Remaining = EndTime - CurrentTime;
        int32 RemainingSeconds = FMath::Max(0, (int32)Remaining.GetTotalSeconds());

        if (RemainingSeconds <= 0) {
            bNeedRefresh = true;
            continue;
        }

        int32 Hours = RemainingSeconds / 3600;
        int32 Minutes = (RemainingSeconds % 3600) / 60;

        FText TimeText = FText::Format(FText::FromString(TEXT("{0}시간 {1}분")), Hours, Minutes);
        BannerButtons[i]->UpdateRemainingTimeText(TimeText);
    }

    if (bNeedRefresh) InitBannerList();
}
// ========================================================
// 가챠 진행 위젯
// ========================================================
void UGachaHUDWidget::UpdateGachaProgressWidget(int32 curr, int32 max)
{
    if (!GachaProgressWidget) return;

    GachaProgressWidget->UpdateGachaProgress(curr, max);
}