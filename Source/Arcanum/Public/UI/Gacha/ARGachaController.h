#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARGachaController.generated.h"

class UGachaLevelHUD;
class AARGachaGameModeBase;
class AGachaHighgradeActor;
class UGachaHighgradeWidget;
class UGachaFinalResultWidget;

struct FGachaItemResult;

UCLASS()
class ARCANUM_API AARGachaController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION() void RequestSkipGacha();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UGachaLevelHUD> GachaHUDClass;
	UPROPERTY() UGachaLevelHUD* GachaHUD;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UGachaFinalResultWidget> GachaFinalResultWidgetClass;
	UPROPERTY() UGachaFinalResultWidget* GachaFinalResultWidget;

public:
	void ShowGachaHighgradeUI(const FGachaItemResult& ItemData, const FText& Dialog);

	UFUNCTION(BlueprintImplementableEvent) void StartCameraSequence();
	UFUNCTION(BlueprintCallable) void EndCameraSequence();

	void ShowFinalResultUI(TArray<FGachaItemResult> ItemRes);

	UFUNCTION() void OnClickReturnLobby();
	
	UFUNCTION(BlueprintNativeEvent) void StopGachaTimeline();
	virtual void StopGachaTimeline_Implementation();
	UFUNCTION() void HandleGachaFinished();
	UPROPERTY(BlueprintReadWrite) bool IsClicked = false;

protected:
	UFUNCTION() void HandleSilhouetteStart();
	UFUNCTION() void HandleSkipRequested();
	

	UPROPERTY(EditAnywhere, Category = "00_Global") TSubclassOf<AGachaHighgradeActor> HighgradeActorClass;
	UPROPERTY() AGachaHighgradeActor* SpawnedGachaActor;

	UPROPERTY() AARGachaGameModeBase* CachedGameMode;

	UPROPERTY(EditAnywhere, Category = "00_Global") TSubclassOf<ACameraActor> GachaCameraClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ACameraActor* GachaCamera;
};