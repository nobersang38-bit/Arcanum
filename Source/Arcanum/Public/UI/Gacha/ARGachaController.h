#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARGachaController.generated.h"

class UGachaLevelHUD;

UCLASS()
class ARCANUM_API AARGachaController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UGachaLevelHUD> GachaHUDClass;

	UPROPERTY()
	UGachaLevelHUD* GachaHUD;
};