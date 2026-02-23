#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "DataTableSettings.generated.h"

UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "Arcanum DataTable Settings"))
class ARCANUM_API UDataTableSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "DataTables")
    TMap<FGameplayTag, TSoftObjectPtr<UDataTable>> DataTableConfig;
};
