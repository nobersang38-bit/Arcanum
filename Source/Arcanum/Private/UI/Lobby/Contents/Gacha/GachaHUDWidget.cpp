#include "UI/Lobby/Contents/Gacha/GachaHUDWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaPullButtonWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaProbabilityWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaBannerButtonWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

#include "Core/ARGameInstance.h"
#include "Core/ARPlayerAccountService.h"
#include "Kismet/GameplayStatics.h"

// ========================================================
// 언리얼 기본생성 / 초기화
// ========================================================
void UGachaHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    InitBannerList();
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
        }
    }

    for (int32 i = 0; i < ActiveBannerDataList.Num(); ++i) {
        UGachaBannerButtonWidget* NewButton = CreateWidget<UGachaBannerButtonWidget>(this, BannerButtonClass);
        if (NewButton) {
            NewButton->UpdateBannerData(&ActiveBannerDataList[i]);
            NewButton->BannerClicked.RemoveAll(this);
            NewButton->BannerClicked.AddUObject(this, &UGachaHUDWidget::OnBannerSelected);

            UVerticalBoxSlot* ButtonSlot = BannerVerticalBox->AddChildToVerticalBox(NewButton);
            if (ButtonSlot) {
                ButtonSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 20.f));
                ButtonSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
            }

            if (i == 0) OnBannerSelected(NewButton->BannerTag);
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

    // 여기서 선택된 배너에 맞춰 오른쪽 메인 화면(보상 리스트 등) 갱신 로직 실행
    UE_LOG(LogTemp, Log, TEXT("Selected Banner Tag: %s"), *SelectedBannerTag.ToString());
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

    if (bSuccess) {
        UE_LOG(LogTemp, Log, TEXT("Gacha Request Success: %d Times"), InPullCount);
        FPlayerAccountService::SetHUDIndex(this, EHUDIndex::GachaMenu);
        if (GachaMap.IsNull() == false) UGameplayStatics::OpenLevelBySoftObjectPtr(this, GachaMap);
    }
    else UE_LOG(LogTemp, Error, TEXT("Gacha Request Failed (Insufficient Currency?)"));

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