#include "UI/Battle/BattleGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h"
#include "Core/ARGameInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABattleGameMode::BeginPlay()
{
    UARGameInstance* GI = Cast<UARGameInstance>(GetGameInstance());
    if (GI && !GI->PendingStageLevel.IsNull()) {

        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            if (ACharacter* Character = PC->GetCharacter())
            {
                Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
            }
        }

        FLatentActionInfo LatentInfo;
        LatentInfo.UUID = FMath::Rand();
        LatentInfo.Linkage = 0;
        LatentInfo.CallbackTarget = this;

        LatentInfo.ExecutionFunction = FName("OnStageLevelLoaded");

        UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, GI->PendingStageLevel, true, true, LatentInfo);
        GI->PendingStageLevel = nullptr;
    }
}

void ABattleGameMode::OnStageLevelLoaded()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (ACharacter* Character = PC->GetCharacter())
        {
            Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
        }
    }
}
