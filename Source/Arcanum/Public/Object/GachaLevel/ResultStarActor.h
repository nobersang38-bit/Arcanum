#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/ARGameInstance.h"
#include "ResultStarActor.generated.h"

class AASplinePathActor;
class AResultStarChild;
class USceneComponent;
class USphereComponent;
class UNiagaraComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResultStarFinished, AResultStarActor*, FinishedActor);

UCLASS()
class ARCANUM_API AResultStarActor : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable) FOnResultStarFinished OnResultStarFinished;
    
    AResultStarActor();

    void SetGachaResults(const TArray<FGachaItemResult>& InResults);

    void SetSplineActor(AASplinePathActor* InSpline) { SplineActor = InSpline; }
    void StartResultSequence(float InDelay = 0.0f);

    UFUNCTION() void OnChildStarClicked(AResultStarChild* ClickedStar);

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, Category = "00-Global") TSoftClassPtr<AResultStarChild> ChildStarClass;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY(VisibleAnywhere) TObjectPtr<USphereComponent> Collision;
    
    /** 스플라인 경로 액터 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<AASplinePathActor> SplineActor;

    /** 자식 별들의 회전 피벗들을 관리하는 배열 */
    UPROPERTY()
    TArray<TObjectPtr<USceneComponent>> OrbitPivots;

    /** 생성된 자식 별동별 액터들을 직접 관리하는 배열 (부모가 쳐 넣기 위해 필요) */
    UPROPERTY()
    TArray<TObjectPtr<AResultStarChild>> StarActors;

    UPROPERTY(EditAnywhere, Category = "Gacha|Movement")
    float MoveSpeed = 3500.f;

    UPROPERTY(EditAnywhere, Category = "Gacha|Movement")
    float RotationSpeed = 360.0f; // 초당 회전 각도

    UPROPERTY(EditAnywhere, Category = "Gacha|Movement")
    float OrbitRadius = 150.0f; // 스플라인 중심으로부터의 거리

    float CurrentDistance = 0.f;
    bool bIsMoving = false;
};