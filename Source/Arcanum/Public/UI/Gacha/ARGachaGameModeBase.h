#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Object/GachaLevel/GachaCamera.h"
#include "ARGachaGameModeBase.generated.h"

class AShootingStarActor;
class AASplinePathActor;
class AResultStarActor;
struct FGachaItemResult;
enum class ESplinePathType : uint8;

UENUM(BlueprintType)
enum class EGachaSequenceState : uint8
{
	None,
	Intro,
	Result,
	Finished,
	OpenAllActor
};

UCLASS()
class ARCANUM_API AARGachaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()


public:
	AARGachaGameModeBase();

protected:
	virtual void BeginPlay() override;

	// ========================================================
	// 가챠 데이터
	// ========================================================
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gacha")
	TArray<FGachaItemResult> GachaItemResult;

	// ========================================================
	// 상태 관리
	// ========================================================
private:
	UPROPERTY()
	EGachaSequenceState CurrentState = EGachaSequenceState::None;

	void EnterIntroState();
	void EnterResultState();
	void EnterFinishedState();

	// ========================================================
	// 연출 시작
	// ========================================================
public:
	UFUNCTION()
	void StartGachaSequence();

	UFUNCTION() void SkipGacha();
	UFUNCTION() void SkipToFinal();

	UFUNCTION() void OpenChildActorAll();

private:
	UFUNCTION() void OnShootingStarShrink();
	UFUNCTION() void OnShootingStarImpact();

	UFUNCTION()	void OnSingleResultFinished(AResultStarActor* FinishedActor);

	UFUNCTION()	void OnAllResultFinished();

	void BindResultActorDelegates(AResultStarActor* ResultActor);

	// ========================================================
	// Spawn / 관리
	// ========================================================

public:
	AActor* GetActiveGachaCamera() { return Cast<AActor>(ActiveGachaCamera.Get()); }
	AResultStarActor* GetActiveResultStar() { return ActiveResultStar; }

private:
	UPROPERTY() TObjectPtr<AShootingStarActor> ActiveShootingStar;
	UPROPERTY() TObjectPtr<AResultStarActor> ActiveResultStar;
	UPROPERTY() TObjectPtr<AGachaCamera> ActiveGachaCamera;

	AShootingStarActor* SpawnShootingStar();
	AResultStarActor* SpawnResultActor();

	void CleanupAllActors();

	// ========================================================
	// 즉시 결과 (Skip용)
	// ========================================================
private:
	void SpawnFinalResultInstant();

	// ========================================================
	// 카메라
	// ========================================================
public:
	void SetupGachaCamera(AActor* Target, float DistanceScale = 1.0f);

	void SetupStaticSplineCamera(AASplinePathActor* SplineActor);

protected:
	UPROPERTY(EditAnywhere, Category = "Gacha|Class")
	TSubclassOf<AShootingStarActor> ShootingStarClass;

	UPROPERTY(EditAnywhere, Category = "Gacha|Class")
	TSubclassOf<AResultStarActor> ResultStarClass;

	UPROPERTY(EditAnywhere, Category = "Gacha|Class")
	TSoftClassPtr<AGachaCamera> GachaCameraClass;

	// ========================================================
	// 스플라인
	// ========================================================
private:
	UPROPERTY()
	TMap<ESplinePathType, AASplinePathActor*> SplineMap;

	AASplinePathActor* GetSpline(ESplinePathType Type);

	// ========================================================
	// 로비 복귀
	// ========================================================
public:
	UFUNCTION() void ReturnLobby();

	UPROPERTY(EditAnywhere, Category = "Map")
	TSoftObjectPtr<UWorld> LobbyMap;
};
