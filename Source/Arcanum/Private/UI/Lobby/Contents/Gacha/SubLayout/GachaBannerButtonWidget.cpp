#include "UI/Lobby/Contents/Gacha/SubLayout/GachaBannerButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Border.h"

// ========================================================
// 델리게이트
// ========================================================
void UGachaBannerButtonWidget::HandleClicked()
{
    BannerClicked.Broadcast(BannerTag);
}
// ========================================================
// 언리얼 기본생성
// ========================================================
void UGachaBannerButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BannerButton) {
        BannerButton->OnClicked.RemoveDynamic(this, &UGachaBannerButtonWidget::HandleClicked);
        BannerButton->OnClicked.AddDynamic(this, &UGachaBannerButtonWidget::HandleClicked);

        BannerButton->SetBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f));
        
        FButtonStyle NewStyle = BannerButton->WidgetStyle;
        if (NormalTexture.IsValid()) NewStyle.Normal.SetResourceObject(NormalTexture.Get());
        if (HoverTexture.IsValid())  NewStyle.Hovered.SetResourceObject(HoverTexture.Get());
        if (ClickTexture.IsValid())  NewStyle.Pressed.SetResourceObject(ClickTexture.Get());

        BannerButton->SetStyle(NewStyle);
    }

    if (SelectionBorder) SelectionBorder->SetVisibility(ESlateVisibility::Hidden);
    SetSelected(false);
}
// ========================================================
// 버튼 설정
// ========================================================
void UGachaBannerButtonWidget::SetSelected(bool bSelected)
{
    if (SelectionBorder) {
        SelectionBorder->SetVisibility(bSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        SetRenderScale(bSelected ? FVector2D(1.05f, 1.05f) : FVector2D(1.0f, 1.0f));
    }
}
void UGachaBannerButtonWidget::SetBannerTexture(UTexture2D* InTexture)
{
    if (!BannerButton || !InTexture) return;

    SetBannerTexture(InTexture, InTexture, InTexture);
}
void UGachaBannerButtonWidget::SetBannerTexture(UTexture2D* InNormal, UTexture2D* InHover, UTexture2D* InClick)
{
    if (!BannerButton) return;

    FButtonStyle NewStyle = BannerButton->WidgetStyle;

    if (InNormal) NewStyle.Normal.SetResourceObject(InNormal);
    if (InHover)  NewStyle.Hovered.SetResourceObject(InHover);
    if (InClick)  NewStyle.Pressed.SetResourceObject(InClick);

    BannerButton->SetStyle(NewStyle);
}