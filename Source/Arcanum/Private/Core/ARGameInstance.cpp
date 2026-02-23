#include "Core/ARGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Core/SubSystem/GameDataSubsystem.h"

#include "DataInfo/BattleCharacter/BattleStats/DataTable/DTBattleStats.h"

void UARGameInstance::Init()
{
    Super::Init();

    if (!ArSaveGame) {
        ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::CreateSaveGameObject(UArcanumSaveGame::StaticClass()));
        AddIDPW(TEXT("Admin"), TEXT("12345"));
    }

    FGameplayTag RootTag = Arcanum::Player::ID::Root;
    UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
    FGameplayTagContainer AllCharacterTags = TagManager.RequestGameplayTagChildren(RootTag);
    for (const FGameplayTag& CharacterTag : AllCharacterTags) {
        InitializeCharacter(CharacterTag);
    }

    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) {
        ArSaveGame = Cast<UArcanumSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    }
}

bool UARGameInstance::AddIDPW(FString ID, FString PW)
{
    if (ArSaveGame) {
        if (ArSaveGame->UserDatabase.Contains(ID)) return false;

        ArSaveGame->UserDatabase.Add(ID, PW);
        if (ArSaveGame) UGameplayStatics::SaveGameToSlot(ArSaveGame, SaveSlotName, 0);
        return true;
    }
    return false;
}

bool UARGameInstance::CheckLogin(FString ID, FString PW)
{
    if (ArSaveGame && ArSaveGame->UserDatabase.Contains(ID)) return ArSaveGame->UserDatabase[ID] == PW;
    return false;
}

void UARGameInstance::SavePlayerData()
{
    if (ArSaveGame) UGameplayStatics::SaveGameToSlot(ArSaveGame, SaveSlotName, 0);
}


void UARGameInstance::InitializeCharacter(FGameplayTag CharacterTag)
{
    if (UserCharacterRegistry.Contains(CharacterTag)) return;

    const FName RowName = GetLeafNameFromTag(CharacterTag);

    FBattleCharacterData NewData;
    NewData.Character = CharacterTag;

    UGameDataSubsystem* DataSubsystem = GetSubsystem<UGameDataSubsystem>();
    if (!DataSubsystem) return;

    //FDTCharacterBattleRegenStatsRow* RegenRowPtr = DataSubsystem->GetRow<FDTCharacterBattleRegenStatsRow>(Arcanum::DataTable::BattleRegenStats, RowName);
    //if (RegenRowPtr) {
    //    for (TFieldIterator<FProperty> It(FDTCharacterBattleRegenStatsRow::StaticStruct()); It; ++It) {
    //        if (FStructProperty* StructProp = CastField<FStructProperty>(*It)) {
    //            if (StructProp->Struct == FRegenStat::StaticStruct()) {
    //                FRegenStat* StatPtr = StructProp->ContainerPtrToValuePtr<FRegenStat>(RegenRowPtr);
    //                if (StatPtr) {
    //                    FRegenStat TempStat = *StatPtr;
    //                    TempStat.InitializeTags();
    //                    NewData.BattleRegenStat.Add(TempStat);
    //                }
    //            }
    //        }
    //    }
    //}
    //FDTCharacterBattleNonRegenStatsRow* NonRegenRowPtr = DataSubsystem->GetRow<FDTCharacterBattleNonRegenStatsRow>(Arcanum::DataTable::BattleNonRegenStats, RowName);
    //if (NonRegenRowPtr) {
    //    for (TFieldIterator<FProperty> It(FDTCharacterBattleNonRegenStatsRow::StaticStruct()); It; ++It) {
    //        if (FStructProperty* StructProp = CastField<FStructProperty>(*It)) {
    //            if (StructProp->Struct == FNonRegenStat::StaticStruct()) {
    //                FNonRegenStat* StatPtr = StructProp->ContainerPtrToValuePtr<FNonRegenStat>(NonRegenRowPtr);
    //                if (StatPtr) {
    //                    NewData.BattleNonRegenStat.Add(*StatPtr);
    //                }
    //            }
    //        }
    //    }
    //}

    UserCharacterRegistry.Add(CharacterTag, NewData);
}
void UARGameInstance::SaveAllData()
{
    UArcanumSaveGame* SaveInstance = Cast<UArcanumSaveGame>(UGameplayStatics::CreateSaveGameObject(UArcanumSaveGame::StaticClass()));
    if (!SaveInstance) return;

    SaveInstance->SavedCharacters = UserCharacterRegistry;

    UGameplayStatics::SaveGameToSlot(SaveInstance, TEXT("ArcanumSaveSlot"), 0);
    UE_LOG(LogTemp, Log, TEXT("Game Saved Successfully."));
}
void UARGameInstance::LoadAllData()
{
    UArcanumSaveGame* LoadInstance = Cast<UArcanumSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ArcanumSaveSlot"), 0));

    FGameplayTag RootTag = Arcanum::Player::ID::Root;
    UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
    FGameplayTagContainer AllCharacterTags = TagManager.RequestGameplayTagChildren(RootTag);
    for (const FGameplayTag& CharacterTag : AllCharacterTags) {
        InitializeCharacter(CharacterTag);
    }

    if (LoadInstance) {
        for (auto& It : LoadInstance->SavedCharacters) {
            FGameplayTag CharTag = It.Key;
            const FBattleCharacterData& SavedData = It.Value;

            if (UserCharacterRegistry.Contains(CharTag)) {
                UserCharacterRegistry[CharTag].CharacterInfo.CurrGrade = SavedData.CharacterInfo.CurrGrade;
                UserCharacterRegistry[CharTag].CharacterInfo.CurrShardCount = SavedData.CharacterInfo.CurrShardCount;
                UserCharacterRegistry[CharTag].bSelection = SavedData.bSelection;

                // 등급에 따른 스탯 재계산이 필요하다면 여기서 호출
                // UpdateStatsByGrade(CharTag);
            }
        }
        UE_LOG(LogTemp, Log, TEXT("Game Loaded and Merged with DT."));
    }
}
