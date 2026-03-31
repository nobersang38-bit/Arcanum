#include "Core/SubSystem/GameTimeSubsystem.h"

void UGameTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGameTimeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UGameTimeSubsystem::Tick(float DeltaTime)
{
	AccumulatedTime += DeltaTime;
	const FDateTime CurrentTimeKST = FDateTime::UtcNow() + FTimespan(9, 0, 0);
	if (bBannerActive && AccumulatedTime >= 1.f) {
		AccumulatedTime = 0.f;
		OnTimeUpdated.Broadcast(CurrentTimeKST);
	}

	if (bIsBattleActive && !bStagePaused) {
		CurrentStageTime -= DeltaTime;
		int32 DisplaySecond = FMath::Max(0, FMath::CeilToInt(CurrentStageTime));

		if (DisplaySecond != LastStageSecond) {
			LastStageSecond = DisplaySecond;
			OnStageSecondChanged.Broadcast(DisplaySecond);

			if (CurrentStageTime <= 0.f) {
				StopStage();
			}
		}
	}

	// 상점 타이머
	if (bShopActive) {
		FTimespan Diff = ShopEndTime - CurrentTimeKST;

		const int32 RemainingAfter = static_cast<int32>(Diff.GetTotalSeconds());
		const int32 RemainingSeconds = FMath::Max(0, RemainingAfter);

		if (RemainingSeconds != LastShopSecond)
		{
			LastShopSecond = RemainingSeconds;
			OnShopSecondChanged.Broadcast(RemainingSeconds);
		}
	}
}

TStatId UGameTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UGameTimeSubsystem, STATGROUP_Tickables);
}

// ---------------------------
// Banner Logic
// ---------------------------
void UGameTimeSubsystem::StartBanner(FDateTime InEndTime)
{
	BannerEndTime = InEndTime;
	bBannerActive = true;
	LastBannerSecond = -1;
}
void UGameTimeSubsystem::StopBanner()
{
	bBannerActive = false;
}
// ---------------------------
// Stage Logic
// ---------------------------
void UGameTimeSubsystem::StartStage(float DurationSeconds)
{
	CurrentStageTime = DurationSeconds;
	bIsBattleActive = true;
	LastStageSecond = -1;
}
void UGameTimeSubsystem::StopStage()
{
	bIsBattleActive = false;
}

void UGameTimeSubsystem::PauseStage()
{
	bStagePaused = true;
}

void UGameTimeSubsystem::ResumeStage()
{
	bStagePaused = false;
}

void UGameTimeSubsystem::StartShop(FDateTime InEndTime)
{
	ShopEndTime = InEndTime;
	bShopActive = true;
	LastShopSecond = -1;
}

void UGameTimeSubsystem::StopShop()
{
	bShopActive = false;
}
