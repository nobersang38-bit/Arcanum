#include "Core/ARGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UARGameInstance::Init()
{
    Super::Init();

    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) {
        ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    }

    if (!ArSaveGame) {
        ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::CreateSaveGameObject(UArcanumSaveGame::StaticClass()));
        AddIDPW(TEXT("Admin"), TEXT("12345"));
    }
}

bool UARGameInstance::AddIDPW(FString ID, FString PW)
{
    if (ArSaveGame) {
        if (ArSaveGame->UserDatabase.Contains(ID)) return false;

        ArSaveGame->UserDatabase.Add(ID, PW);
        SaveGameData();
        return true;
    }
    return false;
}

bool UARGameInstance::CheckLogin(FString ID, FString PW)
{
    if (ArSaveGame && ArSaveGame->UserDatabase.Contains(ID)) return ArSaveGame->UserDatabase[ID] == PW;
    return false;
}

void UARGameInstance::SaveGameData()
{
    if (ArSaveGame) UGameplayStatics::SaveGameToSlot(ArSaveGame, SaveSlotName, 0);
}