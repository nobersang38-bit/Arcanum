#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DataInfo/Stats/Data/FPlayerCurrencies.h"
#include "DTPlayerCurrencies.generated.h"

USTRUCT(BlueprintType)
struct FDTRegenCurrenciesRow : public FTableRowBase
{
    GENERATED_BODY()

    // -----------------------------
    // ParentTag : Supply => Stage 입장 소모성 value
    // -----------------------------
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FRegenCurrencies Supply;
};