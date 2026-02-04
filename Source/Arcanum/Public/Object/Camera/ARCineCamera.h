#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "GameplayTags/ArcanumTags.h"
#include "ARCineCamera.generated.h"

UCLASS()
class ARCANUM_API AARCineCamera : public ACineCameraActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FGameplayTag CameraRole;
};
