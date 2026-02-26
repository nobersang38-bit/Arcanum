#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "IPlayerAccountService.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerAccountService : public UInterface { GENERATED_BODY() };

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerDataReceived, const FPlayerData&);

class ARCANUM_API IPlayerAccountService
{
    GENERATED_BODY()

public:
    virtual void GetIPlayerDataCopy(const UObject* WorldContextObject, TFunction<void(bool)> OptionalCallback) = 0;
    virtual FOnPlayerDataReceived& OnPlayerDataReceived() = 0;
};