#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/ARGameInstance.h"
#include "ResultStarChild.generated.h"

class UNiagaraComponent;
class USceneComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStarClicked, AResultStarChild*, TargetStar);

UCLASS()
class ARCANUM_API AResultStarChild : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable) FOnStarClicked OnStarClicked;

    AResultStarChild();

    UFUNCTION() void OnNotifyClicked(AActor* TouchedActor, FKey ButtonPressed);

    UFUNCTION() void OpenStar();

    void SetResultData(const FGachaItemResult& InData);

    UFUNCTION(BlueprintImplementableEvent) void PlayOpenAnimation(const FGachaItemResult& Data);
    UFUNCTION(BlueprintCallable) void EndOpenAnimation();

    const FGachaItemResult& GetResultData() const { return ResultData; }

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite) TObjectPtr<UStaticMeshComponent> LeftPart;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite) TObjectPtr<UStaticMeshComponent> RightPart;

    UFUNCTION() void ApplyGradeMaterial(UMaterialInterface* InMat);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere) TObjectPtr<USceneComponent> DefaultRoot;

    UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> NiagaraComp;
    UPROPERTY(VisibleAnywhere) TObjectPtr<AActor> ResultActor;
    UPROPERTY(VisibleAnywhere) FText ResultDialog;

    UPROPERTY() FGachaItemResult ResultData;
    UPROPERTY(EditAnywhere, Category = "00-Global") TArray<FGameplayTag> HighGradeTags;

    UPROPERTY(EditAnywhere) float NiagaraTimerDelay = 6.f;

private:
    UPROPERTY(EditAnywhere) bool IsClicked = false; // 클릭 시 true

    FTimerHandle ResultNiagaraTimer;
    UFUNCTION() void DeactivateResultNiagara();

    void SpawnHighGradeWidget();

    bool IsHighGrade() const;

    bool TryLoadAndSpawnResult();
    void HideStarParts();
    void PlayResultFX();
};