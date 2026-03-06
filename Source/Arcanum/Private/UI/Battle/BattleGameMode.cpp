// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BattleGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Core/ARGameInstance.h"

void ABattleGameMode::BeginPlay()
{
    UARGameInstance* GI = Cast<UARGameInstance>(GetGameInstance());
    if (GI && !GI->PendingStageLevel.IsNull()) {
        FLatentActionInfo LatentInfo;
        LatentInfo.UUID = FMath::Rand();
        LatentInfo.Linkage = 0;
        LatentInfo.CallbackTarget = this;
        UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, GI->PendingStageLevel, true, true, LatentInfo);
        GI->PendingStageLevel = nullptr;
    }
}
