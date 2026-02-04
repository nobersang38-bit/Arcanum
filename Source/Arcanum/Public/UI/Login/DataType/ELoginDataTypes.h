#pragma once
#include "CoreMinimal.h"
#include "ELoginDataTypes.generated.h"

/** Sync 단계 구분 */
UENUM(BlueprintType)
enum class ESyncPhase : uint8
{
	PreLogin	UMETA(DisplayName = "Pre Login Sync"),
	PostLogin	UMETA(DisplayName = "Post Login Sync")
};