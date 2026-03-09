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
    
    void SetResultData(const FGachaItemResult& InData);

    UFUNCTION(BlueprintImplementableEvent)
    void PlayOpenAnimation(const FGachaItemResult& Data);
    const FGachaItemResult& GetResultData() const { return ResultData; }

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere) TObjectPtr<USceneComponent> DefaultRoot;
    
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> LeftPart;
    UPROPERTY(VisibleAnywhere) TObjectPtr<UStaticMeshComponent> RightPart;

    UPROPERTY(VisibleAnywhere) TObjectPtr<UNiagaraComponent> NiagaraComp;

    UPROPERTY(VisibleAnywhere) TObjectPtr<AActor> ResultActor;

    UPROPERTY() FGachaItemResult ResultData;
};