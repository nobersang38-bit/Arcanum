#include "UI/Lobby/Contents/Gacha/SubLayout/GachaProbabilityRowWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UGachaProbabilityRowWidget::SetAsHeader(FText GradeName, float Probability, FLinearColor HeaderColor)
{
    if (CharacterIcon) CharacterIcon->SetVisibility(ESlateVisibility::Collapsed);
    if (PickupBadge) PickupBadge->SetVisibility(ESlateVisibility::Collapsed);

    if (Background) Background->SetColorAndOpacity(HeaderColor);

    if (DisplayName) DisplayName->SetText(GradeName);

    // 확률 표시 (예: 4.0%)
    FString ProbStr = FString::SanitizeFloat(Probability * 100.f) + TEXT("%");
    if (ProbabilityText) ProbabilityText->SetText(FText::FromString(ProbStr));
}

void UGachaProbabilityRowWidget::SetAsCharacter(UTexture2D* Icon, FText Name, float Probability, bool bIsPickup)
{
    if (CharacterIcon) {
        CharacterIcon->SetVisibility(ESlateVisibility::Visible);
        CharacterIcon->SetBrushFromTexture(Icon);
    }

    if (PickupBadge) {
        PickupBadge->SetVisibility(bIsPickup ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }

    if (Background) Background->SetColorAndOpacity(FLinearColor(0, 0, 0, 0.2f)); // 약간 어두운 배경

    if (DisplayName) DisplayName->SetText(Name);

    // 상세 확률 (예: 0.024%) - 소수점 아래가 길 수 있으므로 포맷팅 신경써야함
    FString ProbStr = FString::Printf(TEXT("%.3f%%"), Probability * 100.f);
    if (ProbabilityText) ProbabilityText->SetText(FText::FromString(ProbStr));
}