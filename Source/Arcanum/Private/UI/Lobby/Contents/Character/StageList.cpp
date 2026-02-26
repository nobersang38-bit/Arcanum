// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/Contents/Character/StageList.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UStageList::NativePreConstruct()
{
	if (StageImage && StageImg) StageImage->SetBrushFromTexture(StageImg);

    FText DefaultName = FText::FromString(TEXT("스테이지 0"));
    FText DefaultInfo = FText::FromString(TEXT("스테이지 설명"));

    if (StageNameText)
    {
        StageNameText->SetText(StgName.IsEmpty() ? DefaultName : StgName);
    }

    if (StageInfoText)
    {
        StageInfoText->SetText(StgName.IsEmpty() ? DefaultInfo : StgInfo);
    }
}
