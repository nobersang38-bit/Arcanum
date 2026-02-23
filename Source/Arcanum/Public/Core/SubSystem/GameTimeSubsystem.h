#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageSecondChanged, int32, RemainingSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBannerSecondChanged, int32, RemainingSeconds);

/**
 * 전역 게임 시간 관리 서브시스템
 * UTC 기준 +9시간(KST)으로 현재 시간을 관리
 */
UCLASS()
class ARCANUM_API UGameTimeSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
	public:

	// ---------------------------
	// Subsystem
	// ---------------------------
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Tick
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	// ---------------------------
	// Banner (UTC 기반)
	// ---------------------------

	UFUNCTION(BlueprintCallable, Category = "Time|Banner")
	void StartBanner(FDateTime InEndTime);

	UFUNCTION(BlueprintCallable, Category = "Time|Banner")
	void StopBanner();

	UPROPERTY(BlueprintAssignable, Category = "Time|Banner")
	FOnBannerSecondChanged OnBannerSecondChanged;

	// ---------------------------
	// Stage Timer (DeltaTime 기반)
	// ---------------------------

	UFUNCTION(BlueprintCallable, Category = "Time|Stage")
	void StartStage(float DurationSeconds);

	UFUNCTION(BlueprintCallable, Category = "Time|Stage")
	void StopStage();

	UPROPERTY(BlueprintAssignable, Category = "Time|Stage")
	FOnStageSecondChanged OnStageSecondChanged;

private:

	// Banner
	bool bBannerActive = false;
	FDateTime BannerEndTime;
	int32 LastBannerSecond = -1;

	// Stage
	bool bIsBattleActive = false;
	float CurrentStageTime = 0.f;
	int32 LastStageSecond = -1;
};
