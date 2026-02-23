#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/BattleCharacter/CharacterInfo/Data/BattleCharacterInfo.h"
#include "DTCharacterBaseInfo.generated.h"

USTRUCT(BlueprintType)
struct FDTCharacterBaseInfoRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    FBattleCharacterDef BattleCharacterInfo;
};