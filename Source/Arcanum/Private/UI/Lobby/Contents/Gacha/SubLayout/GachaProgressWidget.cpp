#include "UI/Lobby/Contents/Gacha/SubLayout/GachaProgressWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGachaProgressWidget::UpdateGachaProgress(int32 CurrentCount, int32 MaxPity)
{
    if (Text_Pickup) Text_Pickup->SetText(FText::AsNumber(CurrentCount));
    if (Text_Pity) Text_Pity->SetText(FText::AsNumber(MaxPity));
}
