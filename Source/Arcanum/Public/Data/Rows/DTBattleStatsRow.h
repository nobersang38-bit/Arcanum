#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "DTBattleStatsRow.generated.h"

USTRUCT(BlueprintType)
struct FDTCharacterBattleRegenStatsRow : public FTableRowBase
{
    GENERATED_BODY()

    // -----------------------------
    // ParentTag : Health
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRegenStat Health;
    // -----------------------------
    // ParentTag : Mana
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRegenStat Mana;
};

USTRUCT(BlueprintType)
struct FDTCharacterBattleNonRegenStatsRow : public FTableRowBase
{
    GENERATED_BODY()

    // -----------------------------
    // ParentTag : AttackPower
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FNonRegenStat AttackPower;

    // -----------------------------
    // ParentTag : AttackSpeed
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FNonRegenStat AttackSpeed;

    // -----------------------------
    // ParentTag : Evasion
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FNonRegenStat Evasion;

    // -----------------------------
    // ParentTag : MoveSpeed
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FNonRegenStat MoveSpeed;

    // -----------------------------
    // ParentTag : CritChance
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FNonRegenStat CritChance;

    // -----------------------------
    // ParentTag : DamageReduction
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FNonRegenStat DamageReduction;
};


USTRUCT(BlueprintType)
struct FDTPlayerBattleRegenStatsRow : public FTableRowBase
{
    GENERATED_BODY()

    // -----------------------------
    // ParentTag : Meat
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRegenStat Meat;
};
USTRUCT(BlueprintType)
struct FDTPlayerBattleNonRegenStatsRow : public FTableRowBase
{
    GENERATED_BODY()

    // -----------------------------
    // ParentTag : CommandCenterStamina
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FNonRegenStat CommandCenterStamina;
};