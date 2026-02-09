#pragma once
#include "CoreMinimal.h"
#include "EDialogResult.generated.h"

UENUM(BlueprintType)
enum class EDialogType : uint8
{
    OK,
    OKCancel,
};


/** 다이어로그 반환 값*/
UENUM(BlueprintType)
enum class EDialogResult : uint8
{
    OK,
    Cancel,
};