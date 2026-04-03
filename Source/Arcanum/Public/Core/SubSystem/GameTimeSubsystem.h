#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStageSecondChanged, int32, RemainingSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBannerSecondChanged, int32, RemainingSeconds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdated, FDateTime, CurrentTime);

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

	UPROPERTY(BlueprintAssignable, Category = "Time|Banner")
	FOnTimeUpdated OnTimeUpdated;

	float AccumulatedTime = 0.f;
	bool bBannerActive = false;
	// ---------------------------
	// Stage Timer (DeltaTime 기반)
	// ---------------------------

	UFUNCTION(BlueprintCallable, Category = "Time|Stage")
	void StartStage(float DurationSeconds);

	UFUNCTION(BlueprintCallable, Category = "Time|Stage")
	void StopStage();

	UPROPERTY(BlueprintAssignable, Category = "Time|Stage")
	FOnStageSecondChanged OnStageSecondChanged;

	UFUNCTION(BlueprintCallable, Category = "Time|Stage")
	void PauseStage();

	UFUNCTION(BlueprintCallable, Category = "Time|Stage")
	void ResumeStage();

private:
	// Banner
	FDateTime BannerEndTime;
	int32 LastBannerSecond = -1;

	// Stage
	bool bIsBattleActive = false;
	float CurrentStageTime = 0.f;
	int32 LastStageSecond = -1;
	bool bStagePaused = false;

#pragma region 상점 타이머
	// ---------------------------
	// Shop Timer (UTC 기반)
	// ---------------------------
public:
	/* 상점 카운트다운 시작 (절대 종료 시각 전달) */
	UFUNCTION(BlueprintCallable, Category = "Time|Shop")
	void StartShop(FDateTime InEndTime);

	/* 상점 카운트다운 중지 */
	UFUNCTION(BlueprintCallable, Category = "Time|Shop")
	void StopShop();

public:
	/* 상점 남은 초 변경 알림 (UI 표시용) */
	UPROPERTY(BlueprintAssignable, Category = "Time|Shop")
	FOnBannerSecondChanged OnShopSecondChanged;

	/* 상점 시간 주기 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time|Shop")
	int32 ShopRefreshSeconds = 6;

private:
	bool bShopActive = false;
	FDateTime ShopEndTime;
	int32 LastShopSecond = -1;
#pragma endregion
};
