#include "UI/Lobby/Contents/Gacha/SubLayout/GachaPullButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

// ========================================================
// 델리게이트
// ========================================================
void UGachaPullButtonWidget::HandleClicked()
{
	OnPullClicked.Broadcast(PullCount);
}
// ========================================================
// 언리얼 기본생성
// ========================================================
void UGachaPullButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (PullButton) {
        PullButton->OnClicked.RemoveDynamic(this, &UGachaPullButtonWidget::HandleClicked);
        PullButton->OnClicked.AddDynamic(this, &UGachaPullButtonWidget::HandleClicked);
    }
}

void UGachaPullButtonWidget::SetTextCurrency(FText InText)
{
    CurrencyText->SetText(InText);
}
