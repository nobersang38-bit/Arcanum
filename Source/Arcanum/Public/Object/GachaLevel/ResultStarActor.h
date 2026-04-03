#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/ARGameInstance.h"
#include "ResultStarActor.generated.h"

class AASplinePathActor;
class AResultStarChild;
class USceneComponent;
class USphereComponent;
class USplineComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResultStarFinished, AResultStarActor*, FinishedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllResultFinished);

USTRUCT(BlueprintType)
struct FGradeMaterialSet
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FGameplayTag, UMaterialInterface*> Materials;
};

UCLASS()
class ARCANUM_API AResultStarActor : public AActor
{
    GENERATED_BODY()

public:
    AResultStarActor();

    // ========================================================
    // Delegate
    // ========================================================
public:
    UPROPERTY(BlueprintAssignable) FOnResultStarFinished OnResultStarFinished;
    UPROPERTY(BlueprintAssignable) FOnAllResultFinished OnAllResultFinished;

    // ========================================================
    // 외부 인터페이스
    // ========================================================
public:
    void SetGachaResults(const TArray<FGachaItemResult>& InResults);

    void SetSplineActor(AASplinePathActor* InSpline);

    /** 정상 연출 시작 */
    void StartResultSequence(float InDelay = 0.0f);

    /** 스킵용 */
    void SpawnAllAtEnd();

    /** 차일드 액터 전부 클릭 했는지 세는 함수 */
    void NotifyOneResultFinished();
    int32 FinishedCount = 0;


    /** Skip 강제 종료 */
    void ForceFinish();

    UFUNCTION()
    void OnChildStarClicked(AResultStarChild* ClickedStar);

    virtual void Tick(float DeltaTime) override;

    TArray<AResultStarChild*> GetStarActors() { return StarActors; }

protected:
    virtual void BeginPlay() override;

    // ========================================================
    // 설정
    // ========================================================
protected:
    UPROPERTY(EditAnywhere, Category = "Gacha|Class")
    TSoftClassPtr<AResultStarChild> ChildStarClass;

    UPROPERTY(EditAnywhere, Category = "Gacha|Material")
    TMap<FGameplayTag, FGradeMaterialSet> GradeMatMap;

    UPROPERTY(EditAnywhere, Category = "Gacha|Movement")
    float MoveSpeed = 3500.f;

    UPROPERTY(EditAnywhere, Category = "Gacha|Movement")
    float RotationSpeed = 360.0f;

    UPROPERTY(EditAnywhere, Category = "Gacha|Movement")
    float OrbitRadius = 150.0f;

    // ========================================================
    // 컴포넌트
    // ========================================================
private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> Collision;

    // ========================================================
    // 스플라인
    // ========================================================
private:
    UPROPERTY()
    TObjectPtr<AASplinePathActor> SplineActor;

    UPROPERTY()
    TObjectPtr<USplineComponent> Spline;

    // ========================================================
    // 자식 관리
    // ========================================================
private:
    UPROPERTY() TArray<TObjectPtr<USceneComponent>> OrbitPivots;
    UPROPERTY() TArray<TObjectPtr<AResultStarChild>> StarActors;

    // ========================================================
    // 런타임 상태
    // ========================================================
private:
    float CurrentDistance = 0.f;
    bool bIsMoving = false;

    // Delay Timer (스킵 안정성)
    FTimerHandle DelayTimerHandle;
};