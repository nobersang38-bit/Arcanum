#include "UI/Lobby/Contents/Battle/StageList.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UStageList::NativePreConstruct()
{
    if (StageImage && StageImg) StageImage->SetBrushFromTexture(StageImg);

    FText DefaultName = FText::FromString(DefaultStrName);
    FText DefaultInfo = FText::FromString(DefaultStrInfo);

    if (StageNameText) StageNameText->SetText(StgName.IsEmpty() ? DefaultName : StgName);
    if (StageInfoText) StageInfoText->SetText(StgName.IsEmpty() ? DefaultInfo : StgInfo);

    if (BtnClick) {
        BtnClick->OnClicked.RemoveDynamic(this, &UStageList::HandleClick);
        BtnClick->OnClicked.AddDynamic(this, &UStageList::HandleClick);
    }
}

void UStageList::SetSelected(bool bInSelected)
{
    if (!SelectBorder) return;

    bSelected = bInSelected;
    if (bSelected) {
        FSlateBrush NewBrush = SelectBorder->Background;
        NewBrush.DrawAs = ESlateBrushDrawType::Border;
        NewBrush.Margin = FMargin(1.0f);
        FLinearColor Color = FLinearColor(FColor::White);
        NewBrush.TintColor = Color;
        SelectBorder->SetBrush(NewBrush);

        // 깜빡이는 애니메이션
        if (BlinkAnim)
        {
            PlayAnimation(BlinkAnim, 0.f, 0);// 0은 무한 반복
        }
    }
    else {
        FSlateBrush NewBrush = SelectBorder->Background;
        NewBrush.DrawAs = ESlateBrushDrawType::Border;
        NewBrush.Margin = FMargin(1.0f);
        FLinearColor Color = FLinearColor(FColor::FromHex("FF781FFF"));
        NewBrush.TintColor = Color;
        SelectBorder->SetBrush(NewBrush);

        if (BlinkAnim)
        {
            StopAnimation(BlinkAnim);
        }

        SelectBorder->SetRenderOpacity(1.f);
    }
}

void UStageList::SetStageImg(TObjectPtr<UTexture2D> InStageImg)
{
    if (StageImage && InStageImg) StageImage->SetBrushFromTexture(InStageImg);
}

void UStageList::SetText(FString StageName, FString StageInfo)
{
    if (StageNameText) StageNameText->SetText(FText::FromString(StageName));
    if (StageInfoText) StageInfoText->SetText(FText::FromString(StageInfo));
}
void UStageList::HandleClick()
{
    OnStageClicked.Broadcast(this);
}