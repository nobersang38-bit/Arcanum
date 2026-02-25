#include "UI/Lobby/Contents/Gacha/GachaHUDWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaPullButtonWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaProbabilityWidget.h"
#include "UI/Lobby/Contents/Gacha/SubLayout/GachaBannerButtonWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"

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
    if (!BannerScrollBox || !BannerButtonClass) return;

    BannerScrollBox->ClearChildren();

    // TODO: 배너 데이터 테이블을 순회하며 버튼 생성
    // 임시로 루프 예시만 작성
    for (int32 i = 0; i < 3; ++i)
    {
        UGachaBannerButtonWidget* NewButton = CreateWidget<UGachaBannerButtonWidget>(this, BannerButtonClass);
        if (NewButton) {
            NewButton->BannerClicked.RemoveAll(this);
            NewButton->BannerClicked.AddUObject(this, &UGachaHUDWidget::OnBannerSelected);

            BannerScrollBox->AddChild(NewButton);
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

    TArray<UWidget*> AllButtons = BannerScrollBox->GetAllChildren();
    for (UWidget* Widget : AllButtons) {
        if (UGachaBannerButtonWidget* Btn = Cast<UGachaBannerButtonWidget>(Widget)) {
            if (Btn->BannerTag == SelectedBannerTag) {
                Btn->SetSelected(true);
                CurrentSelectedButton = Btn;
                break;
            }
        }
    }

    // 여기서 선택된 배너에 맞춰 오른쪽 메인 화면(보상 리스트 등) 갱신 로직 실행
    UE_LOG(LogTemp, Log, TEXT("Selected Banner Tag: %s"), *SelectedBannerTag.ToString());
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
    // 1. 현재 선택된 배너 태그 확인 (CurrentSelectedButton->BannerTag)
    // 2. 해당 배너에 필요한 재화가 충분한지 확인
    // 3. GachaManager에게 가챠 실행 요청
    UE_LOG(LogTemp, Log, TEXT("Request Gacha: %d Times"), InPullCount);
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
            // ProbabilityWidget->UpdateData(CurrentBannerData);
        }
    }
}